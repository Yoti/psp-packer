#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <psprtc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <kubridge.h>

PSP_MODULE_INFO("DveSample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf pspDebugScreenPrintf

void ErrorExit(int milisecs, char *fmt, ...)
{
	va_list list;
	char msg[256];	

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	printf(msg);
	
	sceKernelDelayThread(milisecs*1000);
	sceKernelExitGame();
}

int pspDveMgrCheckVideoOut();
int pspDveMgrSetVideoOut(int, int, int, int, int, int, int);

int main(void)
{
	SceCtrlData pad;
	int cable, i, u, width, height, displaymode;
	u32 *vram; 
	u64 tick;
	SceKernelUtilsMt19937Context ctx;

	pspDebugScreenInit();

	if (sceKernelDevkitVersion() < 0x03070110)
	{
		ErrorExit(5000, "This program requires 3.71+.\n");
	}

	if (kuKernelGetModel() != PSP_MODEL_SLIM_AND_LITE)
	{
		ErrorExit(5000, "This program is for the psp slim only.\n");
	}

	if (pspSdkLoadStartModule("dvemgr.prx", PSP_MEMORY_PARTITION_KERNEL) < 0)
	{
		ErrorExit(5000, "Error in load/start module.\n");
	}

	printf("This sample will just show random colors in the screen in the full resolution.\n\n");
	printf("Press X to begin\n");

	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS)
		{
			break;
		}

		sceKernelDelayThread(10000);
	}

	cable = pspDveMgrCheckVideoOut();

	if (cable == 0)
	{
		printf("Insert a videoout cable.\n");

		while (cable == 0)
		{
			sceKernelDelayThread(10000);
			cable = pspDveMgrCheckVideoOut();			
		}
	}

	if (cable != 1 && cable != 2)
	{
		ErrorExit(5000, "Unknown cable %d.\n", cable);
	}

	printf("Cable %d detected.\n", cable);
	printf("Going into videoout mode in 5 seconds.\nKeep pressed X when you want to exit");

	sceKernelDelayThread(5*1000*1000);

	if (cable == 1) // composite, sdtv
	{
		u = 2;
		width = 720;
		height = 503;
		displaymode = 0x1D1;
	}
	else // 2, component, (and probably d-terminal too)
	{
		u = 0;
		width = 720;
		height = 480;
		displaymode = 0x1D2;
	}

	sceRtcGetCurrentTick(&tick);
	sceKernelUtilsMt19937Init(&ctx, (u32)tick);

	vram = (u32 *)(0x40000000 | (u32)sceGeEdramGetAddr());
	
	pspDveMgrSetVideoOut(u, displaymode, width, height, 1, 15, 0);
	sceDisplaySetFrameBuf(vram, 1024, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);

	while (1)
	{	
		u32 color = sceKernelUtilsMt19937UInt(&ctx) & 0x00FFFFFF;
		
		for (i = 0; i < (1024 * height); i++)
		{
			vram[i] = color;
		}
		
		sceDisplayWaitVblankStart();
		sceKernelDelayThread(500000);

		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS)
			break;	
	}

	sceKernelExitGame();

    return 0;
}

