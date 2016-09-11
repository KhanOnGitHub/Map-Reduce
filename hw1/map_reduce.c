#include "map_reduce.h"
#include <dirent.h>
#include <sys/types.h>


/**
 * Validates the command line arguments passed in by the user.
 * @param  argc The number of arguments.
 * @param  argv The arguments.
 * @return      Returns -1 if arguments are invalid (refer to hw document).
 *              Returns 0 if -h optional flag is selected. Returns 1 if analysis
 *              is chosen. Returns 2 if stats is chosen. If the -v optional flag
 *              has been selected, validateargs returns 3 if analysis
 *              is chosen and 4 if stats is chosen.
 */
int validateargs(int argc, char** argv) {
	printf("Number of arguments:%d\n", argc);

	//If the first argument is null
	if(argv[1] == NULL) {
		return HELP;
	}

	//If the first argument is help then retun success
	if(strncmp(argv[1],"-h",2) == 0) {
		print_help_menu();
		printf("%d\n",HELP );
		return HELP;
	}

	//If there are more than four arguments which is impossible then retun failure
	if(argc > 4) {
		printf("%d\n", INVALID);
		return INVALID;
	}

	//If either of the optional flags are anywhere but the the first argument then return failure
	for(int i = 2; i < argc; i++) {
		if(strncmp(argv[i], "-h",2) == 0 || strncmp(argv[i], "-v", 2) == 0) {
			printf("%d\n",INVALID);
			return INVALID;
		}
	}

	//if there is no v then do this stuff
	if(strncmp(argv[1],"-v",2) !=0) {
		if(strncmp(argv[1], "ana", 3) !=0 && strncmp(argv[1], "stats", 4) !=0) { //if the argument is not ana nor stats then fail
			printf("%d\n", INVALID);
			return INVALID;
		}
		DIR* dir = opendir(argv[2]); //create a directory pointer to see if it exists
		if(dir) {
			closedir(dir); // if it does then close
			for(int i = 3; i < argc; i++) //check if there are anymore unecessary arguments after the directory, resulting in failure
				if(!argv[i]) {
					printf("%d\n", INVALID);
					return INVALID;
				}
			if(strncmp(argv[1], "ana", 3) ==0) { //if we get here everything has passed and return 1 for ana
				printf("%d\n",ANA);
				return ANA;
			}
			if(strncmp(argv[1], "stats", 4) ==0) {
				printf("%d\n", STATS);
				return STATS;
			}
		}

		else if(dir == NULL) { //if the dir is not existent then fail
			printf("%d\n", INVALID);
			return INVALID;
		}
	}

	//if there is a v then do this stuff
	if(strncmp(argv[1],"-v",2) ==0) {
		if(strncmp(argv[2], "ana", 3) !=0 && strncmp(argv[2], "stats", 4) !=0) { //if the argument is not ana nor stats then fail
			printf("%d\n", INVALID);
			return INVALID;
		}
		DIR* dir = opendir(argv[3]); //create a directory pointer to see if it exists
		if(dir) {
			closedir(dir); // if it does then close
			for(int i = 4; i < argc; i++) { //check if there are anymore unecessary arguments after the directory, resulting in failure
				if(!argv[i])
					printf("%d\n",INVALID);
				return INVALID;
			}
			if(strncmp(argv[2], "ana", 3) ==0) {  //if we get here everything has passed and return 1 for ana
				printf("%d\n", ANA_V);
				return ANA_V;
			}
			if(strncmp(argv[2], "stats", 4) ==0) {
				printf("%d\n", STATS_V);
				return STATS_V;
			}
		}

		else if(dir == NULL)  {
			printf("%d\n",INVALID);
			return INVALID;
		}
	}
	printf("%d\n", INVALID);
	return INVALID;
}

/**
 * Counts the number of files in a directory EXCLUDING . and ..
 * @param  dir The directory for which number of files is desired.
 * @return     The number of files in the directory EXCLUDING . and ..
 *             If nfiles returns 0, then print "No files present in the
 *             directory." and the program should return EXIT_SUCCESS.
 *             Returns -1 if any sort of failure or error occurs.
 */
int nfiles(char* dir) {
	int n = 0;
	struct dirent *file;
	DIR* directory = opendir(dir);

	while((file = readdir(directory)) !=NULL) {
		if(file->d_type == DT_REG)
			n++;
	}
	closedir(directory);
	return n;
}

/**
 * The map function goes through each file in a directory, performs some action on
 * the file and then stores the result.
 *
 * @param  dir     The directory that was specified by the user.
 * @param  results The space where map can store the result for each file.
 * @param  size    The size of struct containing result data for each file.
 * @param  act     The action (function map will call) that map will perform on
 *                 each file. Its argument f is the file stream for the specific
 *                 file. act assumes the filestream is valid, hence, map should
 *                 make sure of it. Its argument res is the space for it to store
 *                 the result for that particular file. Its argument fn is a
 *                 string describing the filename. On failure returns -1, on
 *                 sucess returns value specified in description for the act
 *                 function.
 *
 * @return        The map function returns -1 on failure, sum of act results on
 *                success.
 */
 int map(char* dir, void* results, size_t size, int (*act)(FILE* f, void* res, char* fn)) {
 	DIR* directory = opendir(dir);
 	struct dirent *file;
 	//int dir_size = strlen(dir);;
 	int i = 0;
 	int numfiles = nfiles(dir);
 	
 	while(i < numfiles && (file = readdir(directory)) != NULL) {
 		if(file -> d_type == DT_REG) {
 			char buffer[strlen(dir) + strlen(file->d_name)];
 			strcpy(buffer, dir);
 			if(strncmp((char*) buffer + (strlen(buffer) * sizeof(buffer[0])), "/", 1) != 0)
 				strcat(buffer, "/");
 			strcat(buffer, file->d_name);
 			FILE* file_to_read = fopen(buffer, "r");
 			if(file_to_read != NULL)
 				act(file_to_read, results, file->d_name);
 			else
 				return 0;
 			fclose(file_to_read);
 			results+= size;
 		}
 		
 		i++;
 	}

 	closedir(directory);
 	return 0;
 }

 void print_help_menu() {
 	printf("Usage:\t./mapreduce [h|v] FUNC DIR\n");
 	printf("\t\tFUNC\tWhich operation you would like to run on the data:\n\t\t\tana - Analysis of various text files in a directory.\n\t\t\tstats - Calculates stats on files which contain only\n\t\t\tnumbers.\n\t\tDIR\tThe directory in which the files are located.\n\n");
 	printf("\t\tOptions:\n\t\t-h\tPrints this help menu.\n\t\t-v\tPrints the map function's results, stating the file it's from.\n");
 }