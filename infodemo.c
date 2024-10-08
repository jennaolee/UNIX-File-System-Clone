#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include "infodemo.h"

void getAndPrintGroup(gid_t grpNum) {
  struct group *grp;

  grp = getgrgid(grpNum); 
  
  if (grp) {
    printf("%s", grp->gr_name);
  } else {
    printf("No group name for %u found\n", grpNum);
  }
}



void getAndPrintUserName(uid_t uid) {

  struct passwd *pw = NULL;
  pw = getpwuid(uid);

  if (pw) {
    printf("%s", pw->pw_name);
  } else {
    perror("Hmm not found???");
    printf("No name found for %u\n", uid);
  }
}


// int main() {

//   getAndPrintGroup(1001);
//   getAndPrintGroup(514378);
//   getAndPrintGroup(103);
//   getAndPrintGroup(1000);
  
//   getAndPrintUserName(59894);
//   getAndPrintUserName(23524);
//   getAndPrintUserName(20746);
//   getAndPrintUserName(5970);
//   getAndPrintUserName(10485);

// 	return 0;

// }
