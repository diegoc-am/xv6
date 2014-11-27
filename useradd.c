#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define MAXLEN 30

int main(int argc, char *argv[]){
  int i, n, c,l;
  char iuser[MAXLEN];
  char buf[1024];
  char bufUsers[2];

  l = c = 0;
  int fd;
  fd = open("/shadow", O_RDWR);
  int exists = 0;
  int usersCount = 0;
  char * newUsername = argv[1];
  char * tempPassword = argv[2];
  char * newHomeDirectory = "/home/";
  if(argc <= 2){
    printf(1,"Usage: Specify user and password, Ex.: useradd user password");
    exit();
  }
  while((n = read(fd, buf, sizeof(buf))) > 0){
    //printf(1,"iter");
    for(i=0; i<n;) {
      //printf(1,"itersub: %d", i);
      if(l == 0){ 
        while(i < n && buf[i] != ':' )iuser[c++] = buf[i++];
        if(i == n) break; 
        iuser[c] = '\0';
        i++;
      }
      if(!strcmp(argv[1],iuser)){
        exists = 1;
      }else{
        break;
      }
    }
    usersCount++;
  }
  //printf(1,"exists: %d\n", usersCount);
  if(exists){
    printf(1,"User already exists");
    exit();
  }

  strcpy(newHomeDirectory + strlen(newHomeDirectory), newUsername);
  //printf(1,"%s\n", newHomeDirectory);
  mkdir(newHomeDirectory);
  read(fd, buf, sizeof(buf));
  //printf(1,"fd: %d y n: %d\n", fd,  strlen(buf));
  write(fd, newUsername,strlen(newUsername));
  write(fd, ":",1);
  write(fd, tempPassword, strlen(tempPassword));
  write(fd, ":",1);
  
  int userNumber = open("/usersCount", O_RDWR);
  read(userNumber, bufUsers, sizeof(bufUsers));
  write(fd, bufUsers, sizeof(bufUsers)-1);
  close(userNumber);
  
  write(fd, ":",1);  
  write(fd, newHomeDirectory, strlen(newHomeDirectory));  
  write(fd, "\n", 1);
  bufUsers[0]= bufUsers[0]+1;
  userNumber = open("/usersCount", O_RDWR);
  write(userNumber, bufUsers, sizeof(bufUsers));

  close(fd);
  
  exit();
}