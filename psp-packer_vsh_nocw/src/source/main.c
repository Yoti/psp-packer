#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zlib.h>
#include <time.h>

#define u8	unsigned char
#define u16	unsigned short
#define u32	unsigned int

u8 sce_header[64] = 
{
	0x7E, 0x53, 0x43, 0x45,	0x40, 0x00, 0x00, 0x00, 0x5C, 0x79, 0x72, 0x3D, 0x6B, 0x68, 0x5A, 0x30, 
	0x5C, 0x7D, 0x34, 0x67, 0x57, 0x59, 0x34, 0x78, 0x79, 0x8A, 0x4E, 0x3D, 0x47, 0x4B, 0x44, 0x44, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

typedef struct
{
	u32	signature;	// 000
	u16	attribute;	// 004 (modinfo)
	u16	comp_attribute;	// 006
	u8	module_ver_lo;	// 008
	u8	module_ver_hi;	// 009
	char	modname[28];	// 00A
	u8	version;	// 026
	u8	nsegments;	// 027
	int	elf_size;	// 028
	int	psp_size; 	// 02C
	u32	entry;		// 030
	u32	modinfo_offset;	// 034
	int	bss_size;	// 038
	u16	seg_align[4];	// 03C
	u32	seg_address[4];	// 044
	int	seg_size[4];	// 054
	u32	reserved[5];	// 064
	u32	devkitversion;	// 078
	u32	decrypt_mode;	// 07C 
	u8	key_data0[0x30];// 080
	int	comp_size;	// 0B0
	int	_80;		// 0B4
	int	reserved2[2];	// 0B8
	u8	key_data1[0x10];// 0C0
	u32	tag;		// 0D0
	u8	scheck[0x58];	// 0D4
	u32	key_data2;	// 12C
	u32	oe_tag;		// 130
	u8	key_data3[0x1C];// 134
} __attribute__((packed)) PSP_Header;

typedef struct 
{ 
	u32	e_magic;
	u8	e_class;
	u8	e_data;
	u8	e_idver;
	u8	e_pad[9];
	u16	e_type; 
	u16	e_machine; 
	u32	e_version; 
	u32	e_entry; 
	u32	e_phoff; 
	u32	e_shoff; 
	u32	e_flags; 
	u16	e_ehsize; 
	u16	e_phentsize; 
	u16	e_phnum; 
	u16	e_shentsize; 
	u16	e_shnum; 
	u16	e_shstrndx; 
} __attribute__((packed)) Elf32_Ehdr;

typedef struct 
{ 
	u32	p_type; 
	u32	p_offset; 
	u32	p_vaddr; 
	u32	p_paddr; 
	u32	p_filesz; 
	u32	p_memsz; 
	u32	p_flags; 
	u32	p_align; 
} __attribute__((packed)) Elf32_Phdr;

typedef struct 
{ 
	u32 sh_name; 
	u32 sh_type; 
	u32 sh_flags; 
	u32 sh_addr; 
	u32 sh_offset; 
	u32 sh_size; 
	u32 sh_link; 
	u32 sh_info; 
	u32 sh_addralign; 
	u32 sh_entsize; 
} __attribute__((packed)) Elf32_Shdr;

typedef struct 
{
	u16	attribute;
	u8	module_ver_lo;	
	u8	module_ver_hi;
	char	modname[28];
} __attribute__((packed)) PspModuleInfo;

typedef struct tagInfo
{
	u32 psp_tag;	// 0x00D0 in ~PSP
	u32 oe_tag;	// 0x0130 in ~PSP
} tagInfo;

enum moduleType
{
	MODULE_KERNEL,
	MODULE_USER,
	MODULE_PBP,
	MODULE_VSH	// MODULE_YOTI =)
};

tagInfo tags[] =
{
	// psp_tag	oe_tag
	{ 0xDADADAF0, 0x55668D96 },
	{ 0x457B06F0, 0x8555ABF2 },
	{ 0xADF305F0, 0x7316308C },
	{ 0x38020AF0, 0x28796DAA },	// Yoti
					// from recovery.prx ver. 5.00 M33-6
					// all offsets in "tagInfo" comments
};

int ReadFile(char *file, void *buf, int size)
{
	FILE *f = fopen(file, "rb");
	
	if (!f)
		return -1;

	int rd = fread(buf, 1, size, f);
	fclose(f);

	return rd;
}

int WriteFile(char *file, void *buf, int size)
{
	FILE *f = fopen(file, "wb");
	
	if (!f)
		return -1;	

	int wt = fwrite(buf, 1, size, f);
	fclose(f);

	return wt;
}

void GenerateRandom(u8 *buf, int size)
{
	if (ReadFile("/dev/urandom", buf, size) != size)
	{
		int i;

		for (i = 0; i < 0x10; i++)
		{
			buf[i] = (rand() & 0xFF);
		}
	}
}

int PspPack(u8 *in, int size, u8 *out, int pbp, int use_sce_header)
{
	PSP_Header header;
	Elf32_Ehdr *elf_header;
	Elf32_Phdr *segments;
	Elf32_Shdr *sections;
	char *strtab;
	PspModuleInfo *modinfo;
	int i;

	memset(&header, 0, sizeof(header));
	
	// Fill simple fields
	header.signature = 0x5053507E;
	header.comp_attribute = 1;
	header.version = 1;
	header.elf_size = size;

	header._80 = 0x80;

	elf_header = (Elf32_Ehdr *)in;
	if (elf_header->e_magic != 0x464C457F)
	{
		if (elf_header->e_magic == 0x5053507E || elf_header->e_magic == 0x4543537E)
		{
			printf("Already packed.\n");
			return 0;
		}
		
		printf("Not a PRX.\n");
		return -1;
	}
	
	// Fill fields from elf header
	header.entry = elf_header->e_entry;
	header.nsegments = (elf_header->e_phnum > 2) ? 2 : elf_header->e_phnum;

	if (header.nsegments == 0)
	{
		printf("There are no segments.\n");
		return -1;
	}

	// Fill segements
	segments = (Elf32_Phdr *)&in[elf_header->e_phoff];

	for (i = 0; i < header.nsegments; i++)
	{
		header.seg_align[i] = segments[i].p_align;
		header.seg_address[i] = segments[i].p_vaddr;
		header.seg_size[i] = segments[i].p_memsz;
	}

	// Fill module info fields
	header.modinfo_offset = segments[0].p_paddr;
	modinfo = (PspModuleInfo *)&in[header.modinfo_offset&0x7FFFFFFF];
	header.attribute = modinfo->attribute;
	header.module_ver_lo = modinfo->module_ver_lo;
	header.module_ver_hi = modinfo->module_ver_hi;
	strncpy(header.modname, modinfo->modname, 28);

	sections = (Elf32_Shdr *)&in[elf_header->e_shoff];
	strtab = (char *)(sections[elf_header->e_shstrndx].sh_offset + in);

	header.bss_size = segments[0].p_memsz - segments[0].p_filesz;
	
	for (i = 0; i < elf_header->e_shnum; i++)
	{
		if (strcmp(strtab+sections[i].sh_name, ".bss") == 0)
		{
			header.bss_size = sections[i].sh_size;			
			break;
		}
	}

	if (i == elf_header->e_shnum)
	{
		printf("Error: .bss section not found.\n");
		return -1;
	}	

	if (header.attribute & 0x1000)
	{
		if (pbp)
		{
			printf("No PBP Kernel support!\n");
			return -1;
		}

		header.devkitversion = 0x03070010;
		header.oe_tag = tags[MODULE_KERNEL].oe_tag;
		header.tag = tags[MODULE_KERNEL].psp_tag;
		header.decrypt_mode = 2;
	}

	else if (header.attribute & 0x800)
	{
		if (pbp)
			header.decrypt_mode = 0x0C; // added by neur0n
		else
			header.decrypt_mode = 3;

		header.devkitversion = 0x03070010;	// "1.50" works too
		header.oe_tag = tags[MODULE_VSH].oe_tag;
		header.tag = tags[MODULE_VSH].psp_tag;
	}
	else
	{
		if (pbp)
		{
			if (header.attribute != 0x200)
			{
				char ans;
				
				printf("PBP modules attribute have to be 0x200, do you want it to be automatically changed? (y/n) ");
				
				do
				{
					scanf("%c", &ans);
					ans = tolower(ans);
				} while (ans != 'n' && ans != 'y');

				if (ans == 'n')
				{
					printf("Aborted.\n");
					return -1;
				}			
			}

			header.attribute = modinfo->attribute = 0x200;
			header.oe_tag = tags[MODULE_PBP].oe_tag;
			header.tag = tags[MODULE_PBP].psp_tag;
			header.decrypt_mode = 0x0D;
		}
		else
		{
			header.devkitversion = 0x03050010;
			header.oe_tag = tags[MODULE_USER].oe_tag;
			header.tag = tags[MODULE_USER].psp_tag;
			header.decrypt_mode = 4;
		}
	}
	
	// Fill key data with random bytes
	GenerateRandom(header.key_data0, 0x30);
	GenerateRandom(header.key_data1, 0x10);
	GenerateRandom((u8 *)&header.key_data2, 4);
	GenerateRandom(header.key_data3, 0x1C);

	gzFile comp = gzopen("temp.bin", "wb");
	if (!comp)
	{
		printf("Cannot create temp file.\n");
		return -1;
	}

	if (gzwrite(comp, in, size) != size)
	{
		printf("Error in compression.\n");
		return -1;
	}

	gzclose(comp);

	if (use_sce_header)
	{
		memcpy(out, sce_header, 0x40);
		out += 0x40;
	}

	header.comp_size = ReadFile("temp.bin", out+0x150, 6*1024*1024);
	remove("temp.bin");

	header.psp_size = header.comp_size+0x150;
	memcpy(out, &header, 0x150);

	if (use_sce_header)
		header.psp_size += 0x40;

	return header.psp_size;
}

void usage(char *prog)
{
	printf("Usage: %s [-s] input [output]\n", prog);
}

int main(int argc, char *argv[])
{
	u8 *input, *output;
	u32 *header;
	FILE *f;
	int size, res;
	int sce_header = 0;
	char *outfile;

	printf("psp-packer by Dark_AleX / uo update by Yoti and neur0n\n");

	if (argc < 2 || argc > 4)
	{
		usage(argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "-s") == 0)
	{
		if (argc == 2)
		{
			usage(argv[0]);
			return -1;
		}
		
		sce_header = 1;
		argv++;
		argc--;
	}
	else
	{
		if (argc == 4)
		{
			usage(argv[0]);
			return -1;
		}
	}
	
	srand(time(0));

	f = fopen(argv[1], "rb");
	if (!f)
	{
		printf("Cannot open %s\n", argv[1]);
		return -1;
	}
	else printf("File: %s\n", argv[1]);	// Yoti

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	input = malloc(size);
	if (!input)
	{
		printf("Cannot allocate memory for input buffer.\n");
		return -1;
	}
	output = malloc(size+0x200); 
	if (!output)
	{
		free(input);
		printf("Cannot allocate memory for output buffer.\n");
		return -1;
	}

	fread(input, 1, size, f);
	fclose(f);
	header = (u32 *)input;

	res = 0;
	outfile = (argc == 2) ? argv[1] : argv[2];

	if (header[0] == 0x50425000)
	{
		int prxpos = header[8];
		int prxsize = header[9] - header[8];
		int psarsize = size - header[9];
		
		if (input[prxpos] == 0x7F && memcmp(input+prxpos+1, "ELF", 3) == 0)
		{
			size = PspPack(input+prxpos, prxsize, output, 1, sce_header);
			if (size < 0)
			{
				printf("Error in PspPack.\n");
				res = -1;
			}
			else if (size != 0)
			{
				header[9] = prxpos + size;
				f = fopen(outfile, "wb");
				if (!f)
				{
					printf("Error opening %s for writing.\n", outfile);
					res = -1;
				}
				else
				{
					fwrite(input, 1, prxpos, f);
					fwrite(output, 1, size, f);
					fwrite(input+prxpos+prxsize, 1, psarsize, f);
					fclose(f);
				}
			}
		}
		else if (memcmp(input+prxpos, "~PSP", 4) == 0 || memcmp(input+prxpos, "~SCE", 4) == 0)
		{
			printf("Already packed.\n");		
		}
		else
		{
			printf("Unknown file type in DATA.PSP: 0x%08X\n", header[0]);
			res = -1;
		}
	}
	else if (header[0] == 0x464C457F)
	{
		size = PspPack(input, size, output, 0, sce_header);
		
		if (size < 0)
		{
			printf("Error in PspPack.\n");
			res = -1;
		}
		else if (size != 0)
		{
			if (WriteFile(outfile, output, size) != size)
			{
				printf("Error writing file %s.\n", outfile);
				res = -1;
			}
		}
	}
	else if (header[0] == 0x5053507E || header[0] == 0x4543537E)
	{
		printf("Already packed.\n");		
	}
	else
	{
		printf("Unknown file type: 0x%08X\n", header[0]);
		res = -1;
	}

	free(input);
	free(output);

	return res;
}

