#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define MAXLEN 30

int main(int argc, char *argv[]){
  int i, n, c,l;
  char iuser[MAXLEN];
  char buf[1024];
  char userFileBufer[2];

  l = c = 0;
  int fd;
  fd = open("/shadow", O_RDWR);
  int usersCount = 0;
  char * newUsername = argv[1];
  char * tempPassword = argv[2];
  char * newHomeDirectory = "/home/";
  if(argc <= 2){
    printf(1,"Usage: Specify user and password, Ex.: useradd user password\n");
    exit();
  }
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++) {
      if(l == 0){ 
        if(i != 0){
          i--;  
        }
        while(i < n && buf[i] != ':' ){
          iuser[c++] = buf[i++];
        }
        if(i == n){
          break; 
        } 
        iuser[c] = '\0';
      }
      if(!strcmp(argv[1],iuser)){
        printf(1, "Username already exists\n");
        exit();
      }
      c = 0;
      while(i < n && buf[i++] != '\n');
    }
    
    usersCount++;
  }

  strcpy(newHomeDirectory + strlen(newHomeDirectory), newUsername);
  mkdir(newHomeDirectory);
  read(fd, buf, sizeof(buf));
  write(fd, newUsername,strlen(newUsername));
  write(fd, ":",1);
  write(fd, tempPassword, strlen(tempPassword));
  write(fd, ":",1);
  
  int userNumber = open("/nusers", O_RDWR);
  read(userNumber, userFileBufer, sizeof(userFileBufer));
  write(fd, userFileBufer, sizeof(userFileBufer)-1);
  close(userNumber);
  
  write(fd, ":",1);  
  write(fd, newHomeDirectory, strlen(newHomeDirectory));  
  write(fd, "\n", 1);
  userFileBufer[0]= userFileBufer[0]+1;
  userNumber = open("/nusers", O_RDWR);
  write(userNumber, userFileBufer, sizeof(userFileBufer));

  close(fd);
  
  exit();
}