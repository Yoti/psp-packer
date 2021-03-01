#include <pspsdk.h>
#include <psppopsman.h>

/*
 * Note: this module is based on RE of $ce module
 * "simple", which is the data.psp of psx games.
*/

PSP_MODULE_INFO("complex", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int main(int argc, char *argv[])
{
	// Start the emulator.
	if (scePopsManInitPops(argv[0], 1) < 0)
	{
		scePopsManExitVSHWithError(0x80000004);
	}

	return sceKernelExitDeleteThread(0);
}



