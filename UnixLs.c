#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>

#include "UnixLs.h"
#include "infodemo.h"

void printFileType(struct stat file_info) {
	// determine the file type - directory, link, other
    if (S_ISDIR(file_info.st_mode)) {
        printf("d");
    } else if ((file_info.st_mode & S_IFMT) == S_IFLNK) {
        printf("l");
    } else {
        printf("-");
    }
}

void printPermission(char *permission) {
    printf("%s", permission);
}

void printPermissions(struct stat file_info) {
    // User permissions - read, write, execute
    printPermission(file_info.st_mode & S_IRUSR ? "r" : "-");
    printPermission(file_info.st_mode & S_IWUSR ? "w" : "-");
    printPermission(file_info.st_mode & S_IXUSR ? "x" : "-");

    // Group permissions - read, write, execute
    printPermission(file_info.st_mode & S_IRGRP ? "r" : "-");
    printPermission(file_info.st_mode & S_IWGRP ? "w" : "-");
    printPermission(file_info.st_mode & S_IXGRP ? "x" : "-");

    // Other permissions - read, write, execute
    printPermission(file_info.st_mode & S_IROTH ? "r" : "-");
    printPermission(file_info.st_mode & S_IWOTH ? "w" : "-");
    printPermission(file_info.st_mode & S_IXOTH ? "x" : "-");
}

void printDateAndTime(struct stat file_info) {
	// Array to store the months =
	const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	// Date Information
    struct tm date = *(localtime(&file_info.st_mtime));

	// Print the date in following format: mmm dd yy hh:mm
	printf("%s ", months[date.tm_mon]); // month
	printf("%2d ", date.tm_mday); // day of the month
	printf("%d ", date.tm_year + 1900); // year
	printf("%02d:", date.tm_hour); // hour
	printf("%02d ", date.tm_min); // minut
}

void printLongListData(struct stat file_info) {
	// Print the data for the -l command
	printFileType(file_info);
    printPermissions(file_info);

   	printf(" %2hu ", file_info.st_nlink);

	getAndPrintUserName(file_info.st_uid);
	printf(" ");

	getAndPrintGroup(file_info.st_gid);
	printf(" %5lld ", file_info.st_size);
    printDateAndTime(file_info);
	return;
}

void printLink(struct stat file_info, const char* directory) {
	// Handle link information and printing
	memset(&file_info, 0, sizeof(struct stat));
	stat(directory, &file_info);
	char link[4096];
	memset(&link, 0, 4096);
	readlink(directory, link, 4096);
	printf(" -> %s", link);
}

// Handle execution with one input directory given
void processSingleDirectory(bool isLongList, bool isInode, const char* directory) {
	struct dirent* dir;
	struct stat file_info;

	// if no directory is given, use the current directory
	if (directory == NULL) {
		directory = "."; 
	}

	DIR* directory_stream = opendir(directory);

	if (directory_stream == NULL) {
		// Error handling: file/directory not found
		if (lstat(directory, &file_info) != 0) {
			printf("UnixLs: %s: No such file or directory\n", directory);
			return;
		} else {
			// Error handling: no permissions
			if (S_ISDIR(file_info.st_mode) ) {
				printf("UnixLs: %s: Permission denied\n", directory);
				return;
			}

			// -i command: print the inode number
			if (isInode) {
				printf("%ju ", (uintmax_t)file_info.st_ino);
			}

			// -l command: print the data
			if (isLongList) {
				printLongListData(file_info);
			}

			printf("%s", directory);

			// link handling
			if (isLongList && (file_info.st_mode & S_IFMT) == S_IFLNK) {
				printLink(file_info, directory);
			}
			printf("\n");
			return;
		}
	}

	while ((dir = readdir(directory_stream)) != NULL) {
		if (dir->d_name[0] != '.') {

			// store the full directory name
			char full_directory[4096];
			memset(&full_directory, 0, 4096);
			strcpy(full_directory, directory);

			// append a '/' to separate directory hierarchies
			if (strcmp(directory, "/") != 0) {
				strcat(full_directory, "/");
			}

			// append the directory name
			strcat(full_directory, (char* restrict)dir->d_name);
			
			if (lstat(full_directory, &file_info) == 0) {

				// -i command: print the inode number
				if (isInode) {
					printf("%ju ", (uintmax_t)file_info.st_ino);
				}

				// -l command: print the data
				if (isLongList) {
					printLongListData(file_info);
				}
				
				// print the directory name
				printf("%s", dir->d_name);

				// link handling and printing
				if (isLongList && (file_info.st_mode & S_IFMT) == S_IFLNK) {
					printLink(file_info, full_directory);
				}

				printf("\n");
			}
		}
	}

	// close the directory stream opened by opendir()
	closedir(directory_stream);
	return;
}

// Handling execution when given multiple directory inputs
void processMultipleDirectories(bool isLongList, bool isInode, char** directory_names, int num_arguments) {
	struct stat file_info;
	char** current_directory = directory_names;
	
	// loop for each directory given
	for (int i = 0; i < num_arguments; i++) {

		// Error handling: flag is given
		if (*(*current_directory) == '-') {
			printf("UnixLs: %s: No such file or directory\n", *current_directory);

		} else {
			// Error handling: no file/directory found
			if (lstat(*current_directory, &file_info) != 0) {
				printf("UnixLs: %s: No such file or directory\n", *current_directory);

			} else {
				// if directory, then print the directory name before printing its contents
                if (!S_ISREG(file_info.st_mode)) {
                   printf("%s:\n", *current_directory); 
                }
                
				// process the directory and print the corresponding data
				processSingleDirectory(isLongList, isInode, *current_directory);

				// print newline if more directories
				if (i != num_arguments - 1) {
					printf("\n");
				}
			}
		}

		// move on to next directory
		current_directory++;
	}
}