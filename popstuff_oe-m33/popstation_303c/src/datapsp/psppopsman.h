#ifndef __PSPPOPSMAN_H__
#define __PSPPOPSMAN_H__

/**
 * Inits the emulator.
 *
 * @param file - The pbp file with the psx iso image.
 * @param loadpops - Set to 1 to load pops prx.
 *
 * @return < 0 on error.
 *
*/
int scePopsMan_29B3FB24(const char *file, int loadpops);

#define scePopsManInitPops	scePopsMan_29B3FB24

/**
 * Exits to the vsh with the specified error code.
 *
 * @param code - The error code
 *
 * @returns < 0 on error
*/
int scePopsMan_0090B2C8(int code);

#define scePopsManExitVSHWithError scePopsMan_0090B2C8

/**
 * Does some checksum check?
 *
 * @returns < 0 on error
*/
int scePopsMan_2E18E4E9(void *a0, void *a1, void *a2);

/**
 * Reads the iso header.
 *
 * @param buf - The buffer where the header is read. (Note: it is read at buf+0x400)
 *
 * @return < 0 on error
 * 
*/
int scePopsMan_6768B22F(void *buf);

#define scePopsManReadIsoHeader scePopsMan_6768B22F

/**
 * Opens the document.dat (which is in the same directory as the file
 * passed to scePopsMan_29B3FB24)
 *
 * @returns < 0 on error
*/
int scePopsMan_53F9ABD2(void);

#define scePopsManOpenDocument scePopsMan_53F9ABD2

/**
 * Reads the document.dat
 *
 * @param buf - the buffer to store the data read
 * @param len - The number of bytes to read
 *
 * @returns - The number of bytes read or < 0 on error
*/
int scePopsMan_FC56480E(void *buf, int len);

#define scePopsManReadDocument scePopsMan_FC56480E

/**
 * Sets the file pointer of the document.dat.
 *
 * @param offset - The offset
 * @param whence - PSP_SEEK_SET, PSP_SEEK_CUR or PSP_SEEK_END
 *
 * @returns - The file pointer after the seek.
*/
SceOff scePopsMan_875F4C05(SceOff offset, int whence);

#define scePopsManLseekDocument	scePopsMan_875F4C05

/**
 * Closes document.dat
 *
 * @returns < 0 on error
*/
int scePopsMan_03971322(void);

#define scePopsManCloseDocument scePopsMan_03971322

/**
 * Gets the ps code from idstorage, which contains information such
 * as the region.
 *
 * @param buf - A buffer with at least 8 bytes to store the pscode
 *
 * @returns < 0 on error.
*/
int scePopsManChkregGetPsCode(u8 *buf);


#endif

