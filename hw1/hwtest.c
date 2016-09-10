#include "map_reduce.h"
#include <dirent.h>
#include <sys/types.h>

#define INVALID -1
#define HELP 0
#define ANA 1
#define STATS 2
#define ANA_V 3
#define STATS_V 4

int main(int argc, char** argv) {
	int n = validateargs(argc, argv);
	if(n == INVALID)
		print_help_menu();
	if(n == HELP)
		print_help_menu();
	if(n == ANA)
		printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
	if(n == STATS)
		printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
	if(n == ANA_V)
		printf("Number of files in %s: %d\n",argv[3], nfiles(argv[3]));
	if(n == STATS_V)
		printf("Number of files in %s: %d\n",argv[3], nfiles(argv[3]));
	return 0;
}

int validateargs(int argc, char** argv) {
	printf("Number of arguments:%d\n", argc);

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

void print_help_menu() {
	printf("Usage:\t./mapreduce [h|v] FUNC DIR\n");
	printf("\t\tFUNC\tWhich operation you would like to run on the data:\n\t\t\tana - Analysis of various text files in a directory.\n\t\t\tstats - Calculates stats on files which contain only\n\t\t\tnumbers.\n\t\tDIR\tThe directory in which the files are located.\n\n");
	printf("\t\tOptions:\n\t\t-h\tPrints this help menu.\n\t\t-v\tPrints the map function's results, stating the file it's from.\n");
}