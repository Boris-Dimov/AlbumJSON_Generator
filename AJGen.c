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

_Bool YesFromKeyboard(void){
	char YorN[2]; //to hold the keyboard input, should be a single char, but the scanf/getchar gets problematic because of the \n character from stdin if only a single char is held
	scanf("%s", YorN); //grabbing input
	if (YorN[0] == 'y' || YorN[0] == 'Y') return 1;	//return true if the input character is 'y' or 'Y'
	return 0;	//otherwise return false
}

int main(void){
	char albumTitle[MAX_STRING_LENGTH],
		 albumArtist[MAX_STRING_LENGTH],
		 albumYear[5],
		 albumDesc[MAX_STRING_LENGTH],
		 albumCoverFilename[MAX_STRING_LENGTH]; //definitions for the album
	
	printf("Album.json Generator v0.2\n\nPlace the executable file in the directory where the audio files are located.\n");
	printf("If you encounter any issues, feel free to report them at the repository: https://github.com/Boris-Dimov/AlbumJSON_Generator\n\n");
	//Just some basic info about the program.
		 
	printf("Enter album title: ");
	scanf("%s", albumTitle);
	printf("Enter album artist: ");
	scanf("%s", albumArtist);
	printf("Enter album year: ");
	scanf("%s", albumYear);
	printf("Enter album description (this will show in-game and should be in human-readable format): "); 
	scanf("%s", albumDesc);		//prompting the user to enter the corresponding values
	
	printf("Enter the filename of the file you wish to use as an album cover (omit .png extension): ");
	while(1){   //loop for getting a valid filename
		scanf("%s", albumCoverFilename);    					//the user enters the filename
		strcat(albumCoverFilename, ".png");						//we add the .png extension
		if(!CheckForExistingFile(albumCoverFilename)){  	//we check if the filename is actually valid and points to a file
			printf("Could not find file: '%s'. Try again: ", albumCoverFilename);
			continue;   									//if it doesn't, we prompt the user to enter a new filename and continue the loop.
		}
		break;
	}
	
	_Bool 	useDefaultArtist,
			setAllVolTo100,
			setAllBPMTo0,
			allInCampaign,
			allInChallenges,
			allInSkirmish,
			allInMultiplayer;	//see comment below
	
	//Set up booleans which indicate whether the default values should be used:
	printf("Use the album artist in all songs? [y/n] ");
	useDefaultArtist = YesFromKeyboard();
	printf("Set the volume of all songs to 100? [y/n] ");
	setAllVolTo100 = YesFromKeyboard();
	printf("Set the BPM of all songs to 0? [y/n] ");
	setAllBPMTo0 = YesFromKeyboard();
	printf("Use all songs in campaign? [y/n] ");
	allInCampaign = YesFromKeyboard();
	printf("Use all songs in challenges? [y/n] ");
	allInChallenges = YesFromKeyboard();
	printf("Use all songs in skirmish? [y/n] ");
	allInSkirmish = YesFromKeyboard();
	printf("Use all songs in multiplayer? [y/n] ");
	allInMultiplayer = YesFromKeyboard();
	
	if(CheckForExistingFile("album.json")){
		printf("An 'album.json' file already exists in this directory. Overwrite? [y/n] ");
		if(!YesFromKeyboard()){
			printf("Operation halted by user.\nQuitting\n");
			return 0;
		}
	}
	
	FILE* albumJSON = fopen("album.json", "w");
	if(albumJSON != NULL){  //if the file pointer actually points to a file
		
		//writing out album data:
		fputs("{\n", albumJSON);
		fprintf(albumJSON, "\"title\": \"%s\",\n", albumTitle);
		fprintf(albumJSON, "\"author\": \"%s\",\n", albumArtist);
		fprintf(albumJSON, "\"date\": \"%s\",\n", albumYear);
		fprintf(albumJSON, "\"description\": \"%s\",\n", albumDesc);
		fprintf(albumJSON, "\"album_cover_filename\": \"%s\",\n", albumCoverFilename);
		fprintf(albumJSON, "\"tracks\": [\n");  //all of these are the "header", where the album info resides
		
		char trackFilename[MAX_STRING_LENGTH],
			 trackTitle[MAX_STRING_LENGTH],
			 trackArtist[MAX_STRING_LENGTH];	//string definitions for the tracks
		
		int trackVolume = 100, trackBPM;    	//int definitions for the tracks
		
		while(1){   //loop which writes info about each individual track
			
			printf("Enter track filename (omit .ogg extension): ");
			scanf("%s", trackFilename);					//prompting for track filename
			strcat(trackFilename, ".ogg");				//adding .ogg extension
			if(!CheckForExistingFile(trackFilename)){   //again, checking if an actual file with that name exists
				printf("Could not find file: '%s'. Try again.\n", trackFilename);
				continue;   //if not, we stop the loop here and continue to the next iteration
			}
			
			printf("Enter track title: ");
			scanf("%s", trackTitle);
			
			if(useDefaultArtist){
				memcpy(trackArtist, albumArtist, MAX_STRING_LENGTH);
			}
			else{
				printf("Enter track artist: ");
				scanf("%s", trackArtist);
			}

			char trackModes[100] = "["; 	//this will be the text after "default_music_modes". This definition needs to be here because it's the only one that doesn't get cleared automatically after each iteration and so needs to be created blank every time
			int stringIsFull = 0; 			//to determine whether or not there's another value in the square bracket string trackModes
			
			if(allInCampaign){
				printf("Including track in campaign.\n");   //
				strcat(trackModes, "\"campaign\"");         //
				stringIsFull = 1; 							//if the default bool is set to true, we automatically include the track in campaign.
			}
			else{
				printf("Should the track be played in campaign? [y/n] ");
				if (YesFromKeyboard()){                                     //
					printf("Including track in campaign.\n");               //
					strcat(trackModes, "\"campaign\"");                     //
					stringIsFull = 1;                                       //we prompt the user if he wants to add the track in campaign, and if yes, we add the text "campaign" to trackModes and indicate that there's already something there by setting stringIsFull to 1.
				}
				else{
					printf("Excluding track from campaign.\n");
				}
			}
			
			if(allInChallenges){	//automatically include track in challenges
				printf("Including track in challenges.\n");
				if (stringIsFull == 1) strcat(trackModes,  ", ");
				strcat(trackModes,  "\"challenge\"");  
				stringIsFull = 1; 
			}
			else{
				printf("Should the track be played in challenges? [y/n] ");
				if (YesFromKeyboard()){
					printf("Including track in challenges.\n");
					if (stringIsFull == 1) strcat(trackModes,  ", ");   //we check if there's an element in trackModes, and if yes, we add a ", " to the array
					strcat(trackModes,  "\"challenge\"");   			//add the "challenge" indicator
					stringIsFull = 1;   								//and set stringIsFull to 1, if this happens to be the first element.
				}
				else{
					printf("Excluding track from challenges.\n");
				}
			}
			
			if(allInSkirmish){		//automatically include track in skirmish
				printf("Including track in skirmish.\n");
				if (stringIsFull == 1) strcat(trackModes,  ", ");
				strcat(trackModes,  "\"skirmish\"");
				stringIsFull = 1;
			}
			else{
				printf("Should the track be played in skirmish? [y/n] ");
				if (YesFromKeyboard()){
					printf("Including track in skirmish.\n");
					if (stringIsFull == 1) strcat(trackModes,  ", ");   //same as previous inner if-else
					strcat(trackModes,  "\"skirmish\"");
					stringIsFull = 1;
				}
				else{
					printf("Excluding track from skirmish.\n");
				}
			}
			
			if(allInMultiplayer){	//automatically include track in multiplayer
				printf("Including track in multiplayer.\n");
				if (stringIsFull == 1) strcat(trackModes,  ", ");
				strcat(trackModes,  "\"multiplayer\"");
			}
			else{
				printf("Should the track be played in multiplayer? [y/n] ");
				if (YesFromKeyboard()){
					printf("Including track in multiplayer.\n");
					if (stringIsFull == 1) strcat(trackModes,  ", ");   //same as previous inner if-else
					strcat(trackModes,  "\"multiplayer\"");
				}
				else{
					printf("Excluding track from multiplayer.\n");
				}
			}
			
			strcat(trackModes,  "]");   //the closing square bracket on the default_music_modes line

			if(setAllVolTo100){		//if the bool is true, we automatically set the track volume to 100
				printf("Set track volume to 100.\n");
				trackVolume = 100;
			}
			else{					//otherwise we prompt the user to enter the desired volume
				printf("Enter base track volume (0-100): ");
				scanf("%i", &trackVolume);
				if (trackVolume > 100 || trackVolume < 0) trackVolume = 100;
			}
			
			if(setAllBPMTo0){		//since the BPM value is not used at the moment, we can spare the user some time and automatically set it to 0.
				printf("Set track BPM to 0.\n");
				trackBPM = 100;
			}
			else{
				printf("Enter track BPM (if unknown, type 0): ");
				scanf("%i", &trackBPM);     //prompting for volume and BPM
			}

			fputs("{\n", albumJSON);    //start writing the track segment
			fprintf(albumJSON, "\"filename\": \"%s\",\n", trackFilename);
			fprintf(albumJSON, "\"title\": \"%s\",\n", trackTitle);
			fprintf(albumJSON, "\"author\": \"%s\",\n", trackArtist);
			fprintf(albumJSON, "\"default_music_modes\": %s,\n", trackModes);
			fprintf(albumJSON, "\"base_volume\": %i,\n", trackVolume);
			fprintf(albumJSON, "\"bpm\": %i\n", trackBPM);  //fprintf all properties to the file
		   
			printf("Added 1 track. Continue? [y/n] ");
			if (YesFromKeyboard()){			//prompt the user if they want to add another file
				printf("\n");
				fputs("},\n", albumJSON);   //if yes, we add a closing bracket and a comma
			}
			else{
				fputs("}\n", albumJSON);    //if no, we just add a closing bracket, send out a little message and break the loop
				printf("\nSequence interrupted by user.\nFinalizing file.\n");
				break;
			}
		}
		printf("Quitting\n");
		fputs("]\n", albumJSON);    //these 2 lines add the necessary closing brackets to the end of the file
		fputs("}", albumJSON);      //
		fclose(albumJSON);  		//flush the filestream and close the file
	}
	else{
		printf("Couldn't create 'album.json' file. Check if you have writing permissions for the directory.\n");    //in case there was a problem with creating an album.json file, we quit
	}
	
	return 0;
}
