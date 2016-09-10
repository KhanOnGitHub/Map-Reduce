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
    printf("Welcome to CSE 320!\n");
    return EXIT_SUCCESS;
}

/**
* Validates the command line arguments passed in by the user.
* @param argc The number of arguments.
* @param argv The arguments.
* @return Returns -1 if arguments are invalid (refer to hw document).
* Returns 0 if -h optional flag is selected. Returns 1 if analysis
* is chosen. Returns 2 if stats is chosen. If the -v optional flag
* has been selected, validateargs returns 3 if analysis
* is chosen and 4 if stats is chosen.
*/
int validateargs(int argc, char** argv) {
    if(argv[1] == "-h")
}
