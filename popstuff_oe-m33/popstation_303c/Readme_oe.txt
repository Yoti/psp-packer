Changes in this version of popstation:

- The writing is now a bit different. This avoid "Memory stick blinking too much" bug when using
  the plugin to load the 3.02 pops.

- popstation now lets the use of non-encrypted DATA.PSP. If data.psp is in the same directory as the app,
  popstation will use that as the DATA.PSP of the eboot.pbp. Otherwise, it will use the one from BASE.PBP.
  A DATA.PSP that imitates the original sony one is included.

  EBOOT.PBP generated with a non encrypted data.psp won't need the file KEYS.BIN at all.

Note: these EBOOT.PBP cannot work in versions prior to 3.03 OE-C due to a bug that those versions had.