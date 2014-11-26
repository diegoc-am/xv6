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
  fd = open("shadow", O_RDWR);
  int exists = 0;
  int nUsers = 0;
  char * newUser = argv[1];
  char * tempPass = argv[2];
  char * newDir = "/home/";
  if(argc <= 2){
    printf(1,"Usage: Specify user and password, Ex.: useradd user password");
    exit();
  }
  printf(1,"ARGS: %d", argc);
  printf(1,"ARGS 1: %s", argv[1]);
  printf(1,"ARGS 2: %s", newDir);
  while((n = read(fd, buf, sizeof(buf))) > 0){
    printf(1,"iter");
    for(i=0; i<n;) {
      printf(1,"itersub: %d", i);
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
    nUsers++;
  }
  printf(1,"exists: %d\n", nUsers);
  if(exists){
    printf(1,"User already exists");
    exit();
  }
  read(fd, buf, sizeof(buf));
  printf(1,"fd: %d y n: %d\n", fd,  strlen(buf));
  write(fd, newUser,strlen(newUser));
  write(fd, ":",1);
  write(fd, tempPass, strlen(tempPass));
  write(fd, ":",1);
  
  int numUsrFile = open("nusers", O_RDWR);
  read(numUsrFile, bufUsers, sizeof(bufUsers));
  write(fd, bufUsers, sizeof(bufUsers)-1);
  close(numUsrFile);
  
  write(fd, ":",1);  
  write(fd, newDir, strlen(newDir));
  write(fd, newUser,strlen(newUser));
  write(fd, "\n", 1);
  bufUsers[0]= bufUsers[0]+1;
  numUsrFile = open("nusers", O_RDWR);
  write(numUsrFile, bufUsers, sizeof(bufUsers));

  close(fd);
  
  exit();
}