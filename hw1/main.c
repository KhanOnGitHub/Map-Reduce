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
    if(n == INVALID)
        print_help_menu();
    if(n == HELP)
        print_help_menu();
    if(n == ANA) {
        if(nfiles(argv[2]) != 0) {
            printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
            printf("%s\n", argv[2]);
            map(argv[2], analysis_space, 1, &cat);
        }
        else{
            printf("No files present in the directory.");
            return EXIT_SUCCESS;
        }
    }
    if(n == STATS) {
        if(nfiles(argv[2]) != 0)
            printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
        else{
            printf("No files present in the directory.");
            return EXIT_SUCCESS;
        }
    }
    if(n == ANA_V){
        if(nfiles(argv[2]) != 0)
            printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
        else{
            printf("No files present in the directory.");
            return EXIT_SUCCESS;
        }
    }
    if(n == STATS_V) {
        if(nfiles(argv[2]) != 0)
            printf("Number of files in %s: %d\n",argv[2], nfiles(argv[2]));
        else{
            printf("No files present in the directory.");
            return EXIT_SUCCESS;
        }
    }
    return 0;
}


