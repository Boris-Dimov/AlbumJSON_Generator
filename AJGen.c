#include <stdio.h>
#include <string.h>
#define MAX_STRING_LENGTH 255 //arbitrary, chosen because the maximum filename size in most OSes is 255

_Bool CheckForExistingFile(char* filename){ //A boolean function to make it a little easier to check if a file is present
    FILE* filePointer =  fopen(filename, "r");
    if (filePointer != NULL){
        return 1;
    }
    else{
        return 0;
    }
}

int main(void){
    char albumTitle[MAX_STRING_LENGTH],
         albumArtist[MAX_STRING_LENGTH],
         albumYear[5],
         albumDesc[MAX_STRING_LENGTH],
         albumCoverFilename[MAX_STRING_LENGTH]; //definitions for the album
    printf("Enter album title: ");
    scanf("%s", albumTitle);
    printf("Enter album artist: ");
    scanf("%s", albumArtist);
    printf("Enter album year: ");
    scanf("%s", albumYear);
    printf("Enter album description (this will show in-game and should be in human-readable format): ");
    scanf("%s", albumDesc);
    printf("Enter the filename of the file you wish to use as an album cover (must be a .png, don't omit the extension here): "); //prompting the user to enter the corresponding values
    while(1){   //loop for getting a valid filename
    scanf("%s", albumCoverFilename);    //the user enters the filename
    if (strstr(albumCoverFilename, ".png") == NULL){    //we check if it contains .png
        printf("Invalid filename! Not a .png file! Try again: ");
    }
    else{   //if it does
        if(!CheckForExistingFile(albumCoverFilename)){  //we check if the filename is actually valid and points to a file
            printf("Could not find the specified file. Aborting\n");
            return 0;   //if the filename is invalid, we quit
        }
        break;
    }
    }
    if(!CheckForExistingFile("album.json")){    //if an album.json is successfully created
        FILE* albumJSON = fopen("album.json", "w");
        if(albumJSON != NULL){  //if the file pointer actually points to a file
            fputs("{\n", albumJSON);
            fprintf(albumJSON, "\"title\": \"%s\",\n", albumTitle);
            fprintf(albumJSON, "\"author\": \"%s\",\n", albumArtist);
            fprintf(albumJSON, "\"date\": \"%s\",\n", albumYear);
            fprintf(albumJSON, "\"description\": \"%s\",\n", albumDesc);
            fprintf(albumJSON, "\"album_cover_filename\": \"%s\",\n", albumCoverFilename);
            fprintf(albumJSON, "\"tracks\": [\n");  //all of these are the "header", where the album info resides
            char trackFilename[MAX_STRING_LENGTH],
                 trackTitle[MAX_STRING_LENGTH],
                 trackArtist[MAX_STRING_LENGTH];
            int trackVolume = 100, trackBPM;    //definitions for the tracks
            while(1){   //loop which writes info about each individual track
                printf("Enter track filename (must be an .ogg): ");
                while(1){
                    scanf("%s", trackFilename);
                    if (strstr(trackFilename, ".ogg") == NULL){ //we check if the input filename contains .ogg
                        printf("Invalid filename! Not an .ogg file! Try again: ");
                    }
                    else break;
                }
                if(!CheckForExistingFile(trackFilename)){   //again, checking if an actual file with that name exists
                    printf("Could not find the specified file. Try again.\n");
                    continue;   //if not, we stop the loop here and continue to the next iteration
                }
                printf("Enter track title: ");
                scanf("%s", trackTitle);
                printf("Enter track artist (enter . to use album artist): ");
                scanf("%s", trackArtist);
                if (trackArtist[0] == '.') 	{   //if the user enters '.', we copy the artist name from the album to the track using memcpy()
                    memcpy(trackArtist, albumArtist, MAX_STRING_LENGTH);
                }
                char trackModes[100] = "["; //this will be the text after "default_music_modes". This definition needs to be here because it's the only one that doesn't get cleared automatically after each iteration and so needs to be created blank every time
                char YorN[2]; //should be a single char, but on Linux at least the scanf/getchar gets problematic because of the \n character from stdin
                int stringIsFull = 0; //to determine whether or not there's another value in the square bracket string trackModes
                printf("Should the track be played in campaign? [y/n] ");
                scanf("%s", YorN);
                if (YorN[0] == 'y'){                                        //
                    printf("Including track in campaign.\n");               //
                    strcat(trackModes, "\"campaign\"");                     //
                    stringIsFull = 1;                                       //we prompt the user if he wants to add the track in campaign, and if yes, we add the text "campaign" to trackModes and indicate that there's already something there by setting stringIsFull to 1.
                }
                else{
                    printf("Excluding track from campaign.\n");
                }
                printf("Should the track be played in challenges? [y/n] ");
                scanf("%s", YorN);
                if (YorN[0] == 'y'){
                    printf("Including track in challenges.\n");
                    if (stringIsFull == 1) strcat(trackModes,  ", ");   //we check if there's an element in trackModes, and if yes, we add a ", " to the array
                    strcat(trackModes,  "\"challenge\"");   //add the "challenge" indicator
                    stringIsFull = 1;   //and set stringIsFull to 1, if this happens to be the first element.
                }
                else{
                    printf("Excluding track from challenges.\n");
                }
                printf("Should the track be played in skirmish? [y/n] ");
                scanf("%s", YorN);
                if (YorN[0] == 'y'){
                    printf("Including track in skirmish.\n");
                    if (stringIsFull == 1) strcat(trackModes,  ", ");   //same as previous if-else
                    strcat(trackModes,  "\"skirmish\"");
                    stringIsFull = 1;
                }
                else{
                    printf("Excluding track from skirmish.\n");
                }
                printf("Should the track be played in multiplayer? [y/n] ");
                scanf("%s", YorN);
                if (YorN[0] == 'y'){
                    printf("Including track in multiplayer.\n");
                    if (stringIsFull == 1) strcat(trackModes,  ", ");   //same as previous if-else
                    strcat(trackModes,  "\"multiplayer\"");
                }
                else{
                    printf("Excluding track from multiplayer.\n");
                }
                strcat(trackModes,  "]");   //the closing square bracket on the line

                printf("Enter base track volume (0-100): ");
                scanf("%i", &trackVolume);
                if (trackVolume > 100 || trackVolume < 0) trackVolume = 100;
                printf("Enter track BPM (if unknown, write 0): ");
                scanf("%i", &trackBPM);     //prompting for volume and BPM

                fputs("{\n", albumJSON);    //start writing the track segment
                fprintf(albumJSON, "\"filename\": \"%s\",\n", trackFilename);
                fprintf(albumJSON, "\"title\": \"%s\",\n", trackTitle);
                fprintf(albumJSON, "\"author\": \"%s\",\n", trackArtist);
                fprintf(albumJSON, "\"default_music_modes\": %s,\n", trackModes);
                fprintf(albumJSON, "\"base_volume\": %i,\n", trackVolume);
                fprintf(albumJSON, "\"bpm\": %i\n", trackBPM);  //fprintf all properties to the file
               
                printf("Added 1 track. Continue? [y/n] ");
                scanf("%s", YorN);  //prompt the user if they want to add another file
                if (YorN[0] == 'y'){
                    fputs("},\n", albumJSON);   //if yes, we add a closing bracket and a comma
                }
                else{
                    fputs("}\n", albumJSON);    //if no, we just add a closing bracket and break the loop
                    break;
                }
            }
            printf("Quitting\n");
            fputs("]\n", albumJSON);    //these 2 lines add the necessary closing brackets to the end of the file
            fputs("}", albumJSON);      //
            fclose(albumJSON);  //flush the filestream and close the file
        }
        else{
        printf("Couldn't create 'album.json' file. Check if you have writing permissions for the directory.\n");    //in case there was a problem with creating an album.json file, we quit
        }
    }
    else{
        printf("An 'album.json' file already exists in this directory. Delete it or change the current working directory.\n");  //if a file album.json already exists, we quit, because we don't want to overwrite anything
    }

    return 0;
}
