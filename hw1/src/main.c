#include "map_reduce.h"

//Space to store the results for analysis map
struct Analysis analysis_space[NFILES];
//Space to store the results for stats map
Stats stats_space[NFILES];

//Sample Map function action: Print file contents to stdout and returns the number bytes in the file.
int cat(FILE* f, void* res, char* filename) {
	char c;
	int n = 0;
	printf("%s\n", filename);
	while((c = fgetc(f)) != EOF) {
		printf("%c", c);
		n++;
	}
	printf("\n");
	return n;
}

int main(int argc, char** argv) {
	int n = validateargs(argc, argv);
	int sum = 0;
	if(n == INVALID) {
		print_help_menu();
		return EXIT_FAILURE;
	}
	if(n == HELP)
		print_help_menu();
	if(n == ANA) {
		if(nfiles(argv[2]) != 0) {
			sum = map(argv[2], analysis_space, sizeof(analysis_space[0]), &analysis);
			if(sum != -1)
				analysis_print(analysis_reduce(nfiles(argv[2]), analysis_space), sum, 1);
		}
		else{
			printf("No files present in the directory.");
			return EXIT_SUCCESS;
		}
	}
	if(n == STATS) {
		if(nfiles(argv[2]) != 0) {
			map(argv[2], stats_space, sizeof(stats_space[0]), &stats);
			stats_print(stats_reduce(nfiles(argv[2]), stats_space), 1);
		}
		else{
			printf("No files present in the directory.");
			return EXIT_SUCCESS;
		}
	}
	if(n == ANA_V){
		if(nfiles(argv[3]) != 0) {
			sum = map(argv[3], analysis_space, sizeof(analysis_space[0]), &analysis);
			if(sum != -1) {
				for(int i = 0; i < nfiles(argv[3]); i ++) {
					if(i != nfiles(argv[3]) -1)
						analysis_print(analysis_space[i], sum, 0);
					else {
						analysis_print(analysis_space[i], sum, 0);
						analysis_print(analysis_reduce(nfiles(argv[3]), analysis_space), sum, 1);
					}

				}
			}
		}
		else{
			printf("No files present in the directory.");
			return EXIT_SUCCESS;
		}
	}
	if(n == STATS_V) {
			if(nfiles(argv[3]) != 0) {
				sum = map(argv[3], analysis_space, sizeof(analysis_space[0]), &analysis);
				if(sum != -1) {
					for(int i = 0; i < nfiles(argv[3]); i ++) {
						if(i != nfiles(argv[3]) -1)
							stats_print(stats_space[i], 0);
						else {
							stats_print(stats_reduce(nfiles(argv[3]), stats_space), 1);
						}
					}
				}
			}
			else {
				printf("No files present in the directory.");
				return EXIT_SUCCESS;
			}
		}
		return EXIT_SUCCESS;
	}


