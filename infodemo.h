#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

void getAndPrintGroup(gid_t grpNum);
void getAndPrintUserName(uid_t uid);