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
#include <limits.h>
#include <libgen.h>
#include "infodemo.h"

void printFileType(struct stat file_info);
void printPermission(char *permission);
void printPermissions(struct stat file_info);
void printDateAndTime(struct stat file_info);
void printLongListData(struct stat file_info);
void printLink(struct stat file_info, const char* directory);
void processSingleDirectory(bool isLongList, bool isInode, const char* directory);
void processMultipleDirectories(bool isLongList, bool isInode, char** directory_names, int num_arguments);