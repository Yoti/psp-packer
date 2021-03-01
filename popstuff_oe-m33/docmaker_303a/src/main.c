#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

unsigned char data1[132] = 
{
	0x44, 0x4F, 0x43, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x53, 0x43, 0x55, 0x53, 
	0x39, 0x34, 0x34, 0x37, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xFF, 0xFF, 0xFF, 0xFF
};

void ErrorExit(char *fmt, ...)
{
	va_list list;
	char msg[256];	

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	printf(msg);
	exit(-1);
}

int FileExists(char *file)
{
	FILE *f = fopen(file, "rb");

	if (!f)
		return 0;

	fclose(f);
	return 1;
}


typedef struct __attribute__((packed))
{
	unsigned int offset_low;
	unsigned int offset_high;
	unsigned int unknown;
	unsigned int size_low;
	unsigned int size_high;
	unsigned int dummy[0x6C/4];
} DocumentEntry;

int getsize(FILE *f)
{
	int size;

	fseek(f, 0, SEEK_END);
	size = ftell(f);

	fseek(f, 0, SEEK_SET);
	return size;
}

char buffer[2*1048576];

void generate_document(char *directory, char *output, char *code)
{
	FILE *out, *in;
	char file[1024];
	int i, n;
	DocumentEntry *entries;

	printf("Checking number of images...\n");

	for (n = 0; n < 9999; n++)
	{
		sprintf(file, "%s/%04d.PNG", directory, n);

		if (!FileExists(file))
			break;
	}

	if (n == 0)
	{
		ErrorExit("No png files or wrong naming or wrong directory path.\n");
	}

	printf("Writing header...\n");

	out = fopen(output, "wb");

	if (!out)
	{
		ErrorExit("Cannot open/create output file.\n");
	}

	memcpy(data1+0xC, code, 9);

	fwrite(data1, 1, sizeof(data1), out);
	fwrite(&n, 1, 4, out);

	entries = (DocumentEntry *)malloc(sizeof(DocumentEntry) * n);
	if (!entries)
	{
		fclose(out);
		ErrorExit("Cannot alloc memory for entries!\n");
	}

	memset(entries, 0, sizeof(DocumentEntry) * n);
	fwrite(entries, 1, sizeof(DocumentEntry) * n, out);

	for (i = 0; i < 8; i++)
		fputc(0, out);

	printf("Adding png's...\n");

	for (i = 0; i < n; i++)
	{
		sprintf(file, "%s/%04d.PNG", directory, i);

		in = fopen(file, "rb");
		if (!in)
		{
			fclose(out);
			ErrorExit("Cannot open file %s\n", file);
		}

		entries[i].offset_low = ftell(out);
		entries[i].size_low = getsize(in);
		
		if (entries[i].size_low > sizeof(buffer))
		{
			fclose(in);
			fclose(out);
			ErrorExit("File %s too big.\n", file);
		}

		fread(buffer, 1, entries[i].size_low, in);
		fwrite(buffer, 1, entries[i].size_low, out);

		fclose(in);
	}

	printf("Writing entries...\n");
	fseek(out, 0x88, SEEK_SET);
	fwrite(entries, 1, sizeof(DocumentEntry) * n, out);
	fclose(out);
}

#define N_GAME_CODES	12

char *gamecodes[N_GAME_CODES] =
{
	"SCUS",
	"SLUS",
	"SLES",
	"SCES",
	"SCED",
	"SLPS",
	"SLPM",
	"SCPS",
	"SLED",
	"SLPS",
	"SIPS",
	"ESPM"
};

int main(int argc, char *argv[])
{
	int i;

	if (argc != 3)
	{
		ErrorExit("Invalid number of arguments\n"
				  "Usage: %s gamecode directory\n", argv[0]);
	}

	if (strlen(argv[1]) != 9)
	{
		ErrorExit("Invalid game code.\n");
	}

	for (i = 0; i < N_GAME_CODES; i++)
	{
		if (strncmp(argv[1], gamecodes[i], 4) == 0)
			break;
	}

	if (i == N_GAME_CODES)
	{
		ErrorExit("Invalid game code.\n");
	}

	for (i = 4; i < 9; i++)
	{
		if (argv[1][i] < '0' || argv[1][i] > '9')
		{
			ErrorExit("Invalid game code.\n");
		}
	}

	generate_document(argv[2], "DOCUMENT.DAT", argv[1]);
	printf("Done.\n");
	
	return 0;
}

