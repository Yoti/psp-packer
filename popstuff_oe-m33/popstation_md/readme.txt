This popstation is for create multidisc games only. 
To create single disc games, continue using old popstation.

popstation_md works pretty much the same as normal popstation. If you want to include to the game
some icon, image or pmf icon, put them in the same directory as popstation_md with the usual names: ICON0.PNG, PIC1.PNG, etc

popstation_md also requires BASE.PBP, which should be the game Hot Shots bought in the psn store.

Limits of postation_md:

- Minimum number of disc is 2. Maximum is 5. These restrictions are imposed due to the format itself 
and due to pops.prx code, they are not imposed by me.
- There is no support for no compression (e.g. compression level 0). This is due to technical 
issues, all games will go compressed.

Usage:

popstation_md main_title <titles> main_gamecode <gamecodes> <compressionlevels> <files>

- main_title: The title of the game.

- <titles>: the titles for each disc, usually you will specify all of them to be the same as main_title.

- main_gamecode: The gamecode of the game (should be the one of first disc).

- <gamecodes>: The gamecodes of each disc. The first disc usually will be the same as main_gamecode.
  The codes of the rest of disc are get by adding 1 to the code of first disc.

- <compressionlevels>: the compression levels for each disc, in the range of 1-9. 1=faster compression, 
  9=better compression, 6=default in zlib.

- <files> The paths to the isos.

Examples of command lines:

popstation_md "Metal Gear" "Metal Gear" "Metal Gear" SLPM86111 SLPM86111 SLPM86112 6 8 metal1.iso metal2.iso

- It Would create a multidisc metal gear using compression level 6 for first disc, and compression level 8 for second disc.

popstation_md "ATHENA: Awakening" "ATHENA: Awakening" "ATHENA: Awakening" "ATHENA: Awakening" SLPM86185 SLPM86185 SLPM86186 SLPM86187 1 4 9 athena1.iso athena2.iso athena3.iso

- It would create a multidisc athena using compression levels of 1, 4, 9 for disc 1, 2 and 3.

--------

FAQ about multidisc games.

- How can I change the disc?

The disc change option is usually in grey until the game asks you to change it.
Sometimes the option may be in white even if the game doesn't ask a disc change. 
Do not change disc if game doesn't ask for it!

If you want to force a change of disc, use the "Restart game" option. It will ask you with which disc
to reboot. Pops will remember last disc in which you booted.

- Can no multidisc games be put in a package with this?
Yes, they can. You would change of game by using the restart game option, and selecting the disc.

- Which firmware require multidisc games?
3.71 M33-4. Alternatively, you can use the new popsloader too to make it work in 3.71 M33-3, selecting
3.72 pops.

- Which OS is popstation_md for? 
The binary is for windows/cygwin (you will need cygwin dll's, they are easy to find if you don't have them yet).
But source is included, which should compile and work in most LE systems.


