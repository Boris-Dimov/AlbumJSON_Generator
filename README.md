# AlbumJSON Generator
A simple program that generates album.json files for Warzone 2100's Music manager

# Instructions
The whole program is contained within the AJGen.c file. It uses only the standard C libraries and should be cross-platform compatible.
A compiled Windows version is available in [Releases.](https://github.com/Boris-Dimov/AlbumJSON_Generator/releases)

The executable file must be located in the same folder as the album cover PNG file and the OGG/Vorbis music files. 
For more information on where to put the finished folder, see [the Warzone 2100 GitHub page.](https://github.com/Warzone2100/warzone2100)

The program operates entirely in CLI mode.

If you encounter any issues, report them at the [repository.](https://github.com/Boris-Dimov/AlbumJSON_Generator)

# Detailed steps of operation
Preparation: Put all your .ogg files and a .png file you wish to use as an album cover in a single folder.

1. When firing up the program, the user has to enter the corresponding info about the album (incl. a valid filename for the cover).
2. Then, the user is prompted if they'd like to set numerous options to their default values: track volume, BPM, etc.
3. If an album.json file is detected in the folder, the user is asked whether or not it should be overwritten. If not, the program quits.
4. The program enters a loop, where it prompts for track filename and title, and, depending on chosen options, volume, BPM, and playback modes.
5. After a track is added, the user is prompted if they want to continue. If yes, the cycle is repeated. If not, the program finalizes the file and quits.

# Known issues
Track filenames of the kind "track#.ogg" (where # is a number) are known to cause conflicts with existing files in the game and should be avoided.
