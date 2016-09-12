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
	//If the first argument is null
 	if(argv[1] == NULL) {
 		return INVALID;
 	}

	//If the first argument is help then retun success
 	if(strncmp(argv[1],"-h",2) == 0) {
 		print_help_menu();
 		return HELP;
 	}

	//If there are more than four arguments which is impossible then retun failure
 	if(argc > 4) {
 		return INVALID;
 	}

	//If either of the optional flags are anywhere but the the first argument then return failure
 	for(int i = 2; i < argc; i++) {
 		if(strncmp(argv[i], "-h",2) == 0 || strncmp(argv[i], "-v", 2) == 0) {
 			return INVALID;
 		}
 	}

	//if there is no v then do this stuff
 	if(strncmp(argv[1],"-v",2) !=0) {
		if(strncmp(argv[1], "ana", 3) !=0 && strncmp(argv[1], "stats", 4) !=0) { //if the argument is not ana nor stats then fail
			return INVALID;
		}
		DIR* dir = opendir(argv[2]); //create a directory pointer to see if it exists
		if(dir) {
			closedir(dir); // if it does then close
			for(int i = 3; i < argc; i++) //check if there are anymore unecessary arguments after the directory, resulting in failure
				if(!argv[i]) {
					return INVALID;
				}
			if(strncmp(argv[1], "ana", 3) ==0) { //if we get here everything has passed and return 1 for ana
				return ANA;
			}
			if(strncmp(argv[1], "stats", 4) ==0) {
				return STATS;
			}
		}

		else if(dir == NULL) { //if the dir is not existent then fail
			return INVALID;
		}
	}

	//if there is a v then do this stuff
	if(strncmp(argv[1],"-v",2) ==0) {
		if(strncmp(argv[2], "ana", 3) !=0 && strncmp(argv[2], "stats", 4) !=0) { //if the argument is not ana nor stats then fail
			return INVALID;
		}
		DIR* dir = opendir(argv[3]); //create a directory pointer to see if it exists
		if(dir) {
			closedir(dir); // if it does then close
			for(int i = 4; i < argc; i++) { //check if there are anymore unecessary arguments after the directory, resulting in failure
				if(!argv[i])
					return INVALID;
			}
			if(strncmp(argv[2], "ana", 3) ==0) {  //if we get here everything has passed and return 1 for ana
				return ANA_V;
			}
			if(strncmp(argv[2], "stats", 4) ==0) {
				return STATS_V;
			}
		}

		else if(dir == NULL)  {
			return INVALID;
		}
	}
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
 	int sum = 0;
 	//int dir_size = strlen(dir);;
 	int i = 0;
 	for(i = 0; i < NFILES; i++) {
 		if((file = readdir(directory)) != NULL) {
 			if(file -> d_type == DT_REG) {
 				char buffer[strlen(dir) + strlen(file->d_name)+1];
 				memset(buffer, 0, strlen(buffer));
 				strcpy(buffer, dir);
 				if(strncmp(buffer + (strlen(buffer) -1), "/", 1) != 0)
 					strcat(buffer, "/");
 				strcat(buffer, file->d_name);
 				FILE* file_to_read = fopen(buffer, "r");
 				if(file_to_read != NULL)
 					sum += act(file_to_read, results, file->d_name);
 				else
 					return -1;
 				fclose(file_to_read);
 				results+= size;
 			}
 		}
 	}

 	closedir(directory);
 	return sum;
 }

 /**
 * This reduce function takes the results produced by map and cumulates all
 * the data to give one final Analysis struct. Final struct should contain 
 * filename of file which has longest line.
 *
 * @param  n       The number of files analyzed.
 * @param  results The results array that has been populated by map.
 * @return         The struct containing all the cumulated data.
 */
 struct Analysis analysis_reduce(int n, void* results) {
 	int i;
 	int longest_ana = 0;
 	struct Analysis* ana = (struct Analysis*) results; 
 	struct Analysis return_ana;
 	for(int k = 0; k < 128; k++)
 		(return_ana.ascii[k]) = 0;
 	for(i= 0; i < n; i++) {
 		for(int k = 0; k < 128; k++)
 			(return_ana.ascii[k]) += (ana[i].ascii[k]);
 		if(i != n-1) {
 			if(ana[i].lnlen < ana[i+1].lnlen) {
 				return_ana.lnlen = ana[i+1].lnlen;
 				longest_ana = i;
 			}
 			else {
 				return_ana.lnlen = ana[i].lnlen;
 				longest_ana = i;
 			}
 		}
 	}
 	return_ana.lnno = ana[longest_ana].lnno;
 	return_ana.filename = ana[longest_ana].filename;
 	return return_ana;
 }

/**
 * This reduce function takes the results produced by map and cumulates all
 * the data to give one final Stats struct. Filename field in the final struct 
 * should be set to NULL.
 *
 * @param  n       The number of files analyzed.
 * @param  results The results array that has been populated by map.
 * @return         The struct containing all the cumulated data.
 */
 Stats stats_reduce(int n, void* results) {
 	Stats* stat = (Stats*) results;
 	Stats return_stats;
 	return_stats.sum = 0;
 	return_stats.n = 0;
 	return_stats.filename = NULL;
 	for(int i = 0; i < NVAL; i++)
 		return_stats.histogram[i] = 0;
 	for(int i = 0; i < n; i++) {
 		for(int k = 0; k < NVAL; k++)
 			(return_stats.histogram[k]) += (stat[i].histogram[k]);
 		return_stats.sum += stat[i].sum;
 		return_stats.n += stat[i].n;
 	}
 	return return_stats;
 }

/**
 * Always prints the following:
 * - The name of the file (for the final result the file with the longest line)
 * - The longest line in the directory's length.
 * - The longest line in the directory's line number.
 *
 * Prints only for the final result:
 * - The total number of bytes in the directory.
 *
 * If the hist parameter is non-zero print the histogram of ASCII character
 * occurrences. When printing out details for each file (i.e the -v option was
 * selected) you MUST NOT print the histogram. However, it MUST be printed for
 * the final result.
 *
 * Look at sample output for examples of how this should be print. You have to
 * match the sample output for full credit.
 *
 * @param res    The final result returned by analysis_reduce
 * @param nbytes The number of bytes in the directory.
 * @param hist   If this is non-zero, prints additional information. (Only non-
 *               zero for printing the final result.)
 */
 void analysis_print(struct Analysis res, int nbytes, int hist) {
 	int n = 0;
 	for(int i = 0; i < 128; i++) {
 		n+=res.ascii[i];
 	}
 	if(n == nbytes) {
 		printf("File: %s\n", res.filename);
 		printf("Longest line length: %d\n", res.lnlen);
 		printf("Longest line number: %d\n", res.lnno);
 		printf("Total Bytes in directory: %d\n", n);
 	}
 	else {
 		printf("File: %s\n", res.filename);
 		printf("Longest line length: %d\n", res.lnlen);
 		printf("Longest line number: %d\n\n", res.lnno);
 	}
 	if(hist !=0) {
 		printf("Histogram:\n");
 		for(int i = 0; i < 128; i++) {
 			if(res.ascii[i] != 0) {
 				printf("%d:", i);
 				for(int k = 0; k < res.ascii[i]; k++) {
 					if(k == res.ascii[i] -1)
 						printf("-\n");
 					else
 						printf("-");
 				}
 			}
 		}
 	}
 }

 /**
 * Always prints the following:
 * Count (total number of numbers read), Mean, Mode, Median, Q1, Q3, Min, Max
 *
 * Prints only for each Map result:
 * The file name
 *
 * If the hist parameter is non-zero print the the histogram. When printing out
 * details for each file (i.e the -v option was selected) you MUST NOT print the
 * histogram. However, it MUST be printed for the final result.
 *
 * Look at sample output for examples of how this should be print. You have to
 * match the sample output for full credit.
 *
 * @param res  The final result returned by stats_reduce
 * @param hist If this is non-zero, prints additional information. (Only non-
 *             zero for printing the final result.)
 */
 void stats_print(Stats res, int hist) {
 	int mode;
 	double median=0;
 	double q1=0;
 	double q3=0;
 	int median_arr[res.n];
 	for(int i = 0; i < res.n; i++)
 		median_arr[i] = 0;
 	int median_size = (sizeof(median_arr)/sizeof(median_arr[0]));
 	int median_count = 0;
 	int min = 0;
 	int max = 0;
 	double mean = 0;
 	mean = (double)((res.sum)/(res.n));

 	for(int i = 0; i < NVAL; i++) {
 		if(res.histogram[i] > mode)
 			mode = i;
 	}
 	for(int i = 0; i < NVAL; i++) {
 		if(res.histogram[i] != 0) {
 			for(int k = 0; k < res.histogram[i]; k++) {
 				median_arr[median_count] = i;
 				median_count++;
 			}
 		}
 	}
 	if(median_size%2 == 0) {
 		median = (median_arr[median_size/2] + median_arr[median_size/2 +1])/2;
 		int q1_arr[median_size/2];
 		int q3_arr[median_size/2];
 		for(int i = 0; i < median_size/2 - 1; i++) {
 			q1_arr[i] = q3_arr[i] = 0;
 		}
 		for(int i = 0; i < median_size/2; i++) {
 			q1_arr[i] = median_arr[i];
 		}
 		for(int i = median_size/2; i < median_size - 1; i++) {
 			q3_arr[i] = median_arr[i];
 		}
 		q1 = (q1_arr[median_size/4] + q1_arr[(median_size/4) + 1])/2;
 		q3 = (q3_arr[median_size/4] + q3_arr[(median_size/4)+ 1])/2;
 	}

 	else {
 		median = median_arr[median_size/2];
 		int q1_arr[(median_size/2)-1];
 		int q3_arr[(median_size/2)-1];
 		for(int i = 0; i < median_size/2; i++) {
 			q1_arr[i] = q3_arr[i] = 0;
 		}
 		for(int i = 0; i < (median_size/2 - 1); i++)
 			q1_arr[i] = median_arr[i];
 		for(int i = (median_size/2)+1; i < median_size; i++)
 			q3_arr[i] = median_arr[i];
 		int q1_size = sizeof(q1_arr)/sizeof(q1_arr[0]);
 		int q3_size = sizeof(q3_arr)/sizeof(q3_arr[0]);
 		q1 = q1_arr[q1_size/2];
 		q3 = q3_arr[q3_size/2];
 	}
 	min = median_arr[0];
 	max = median_arr[median_size];

 	if(hist != 0) {
 		printf("Histogram:\n");
 		for(int i = 0; i < NVAL; i++) {
 			if(res.histogram[i] != 0) {
 				printf("%d:", i );
 				for(int k = 0; k < res.histogram[i]; k++) {
 					if(k == res.histogram[i] - 1)
 						printf("-\n\n");
 					else
 						printf("-");
 				}
 			}
 		}
 		//print other stats
 		printf("Count: %d\n", res.n);
 		printf("Mean: %f\n", mean );
 		printf("Mode: %d\n", mode);
 		printf("Median: %f\n", median);
 		printf("Q1: %f\n",q1 );
 		printf("Q3: %f\n", q3);
 		printf("Min: %d\n",  min);
 		printf("Max: %d\n", max );
 	}

 	else {
 		printf("File: %s\n", res.filename );
 		printf("Count: %d\n", res.n);
 		printf("Mean: %f\n", mean );
 		printf("Mode: %d\n", mode);
 		printf("Median: %f\n", median);
 		printf("Q1: %f\n",q1 );
 		printf("Q3: %f\n", q3);
 		printf("Min: %d\n",  min);
 		printf("Max: %d\n\n", max );
 	}
 }

/**
 * This function performs various different analyses on a file. It
 * calculates the total number of bytes in the file, stores the longest line
 * length and the line number, and frequencies of ASCII characters in the file.
 *
 * @param  f        The filestream on which the action will be performed. You
 *                  you can assume the filestream passed by map will be valid.
 * @param  res      The slot in the results array in which the data will be
 *                  stored.
 * @param  filename The filename of the file currently being processed.
 * @return          Return the number of bytes read.
 */
 int analysis(FILE* f, void* res, char* filename) {
 	int n = 0;
 	int longlen=0;
 	int longno=0;
 	int currlen = 0;
 	int currno = 1;
 	char c;
 	struct Analysis* ana = (struct Analysis*) res;
 	ana->filename = filename;
 	while((c = fgetc(f))!= EOF) {
 		if((int) c == 10) {
 			if(currlen > longlen) {
 				longlen = currlen;
 				longno = currno;
 			}
 			currlen = 0;
 			currno++;
 			ana->ascii[(int)c]++;
 		}

 		else {
 			ana->ascii[(int)c]++;
 			currlen++;
 		}
 		n++;
 	}

 	ana->lnlen = longlen;
 	ana->lnno = longno;
 	return n;
 }

 /**
 * This function counts the number of occurrences of each number in a file. It
 * also calculates the sum total of all numbers in the file and how many numbers
 * are in the file. If the file has an invalid entry return -1.
 *
 * @param  f        The filestream on which the action will be performed. You
 *                  you can assume the filestream passed by map will be valid.
 * @param  res      The slot in the results array in which the data will be
 *                  stored.
 * @param  filename The filename of the file currently being processed.
 * @return          Return 0 on success and -1 on failure.
 */
 int stats(FILE* f, void* res, char* filename) {

 	Stats* stat = (Stats*) res;
 	int n = 0;
 	stat->filename = filename;
 	while(fscanf(f, "%d ",&n)  > 0) {
 		stat->histogram[n] ++;
 		stat->sum+=n;
 		stat->n++;		
 	}
 	return 0;
 }

 void print_help_menu() {
 	printf("Usage:\t./mapreduce [h|v] FUNC DIR\n");
 	printf("\t\tFUNC\tWhich operation you would like to run on the data:\n\t\t\tana - Analysis of various text files in a directory.\n\t\t\tstats - Calculates stats on files which contain only\n\t\t\tnumbers.\n\t\tDIR\tThe directory in which the files are located.\n\n");
 	printf("\t\tOptions:\n\t\t-h\tPrints this help menu.\n\t\t-v\tPrints the map function's results, stating the file it's from.\n");
 }