#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "infodemo.h"
#include "UnixLs.h"

int main(int argc, char* argv[]) {	
    bool isLongList = false; 
    bool isInode = false; 
    const char* directory = NULL;

    if (argc > 1) {

        for (int i = 1; i < argc; i++) {

            if (*(argv[i]) == '-') {

                if (directory != NULL) {
                    printf("Error: Options must be specified before directory or file\n");
                    exit(1);
                }
            
                for (int j = 1; j < strlen(argv[i]); j++) {
                    if (*(argv[i] + j) == 'l') {
                        isLongList = true;

                    } else if (*(argv[i] + j) == 'i') {
                        isInode = true;

                    } else {
                        printf("Error: Invalid command -- '%c'\n", *(argv[i] + j));
                        printf("Valid option: -i, -l, -il, -li\n");
                        exit(1);
                    }
                }
            } else {
                directory = argv[i];

                if ((i + 1) < argc && *(argv[i + 1]) != '-') {
                    processMultipleDirectories(isLongList, isInode, argv + i, argc - i);
                    return 0;

                } else {
                    processSingleDirectory(isLongList, isInode, directory);
                }
            }
        }
    }

    if (directory == NULL) {
        processSingleDirectory(isLongList, isInode, directory);
    }

	return 0;
}

