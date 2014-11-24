#include "types.h"
#include "stat.h"
#include "user.h"
#define MAXLEN 20
//open("archivo", O_CREATE);

int checkpasswd(int fd, char *user, char *passwd){
  int i, n, c,l;
  char ipasswd[MAXLEN];
  char iuser[MAXLEN];
  char buf[1024];

  l = c = 0;
 
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n;) {

      if(l == 0){	
      	while(i < n && buf[i] != ':' )iuser[c++] = buf[i++];
      	if(i == n) break; 
      	iuser[c] = '\0';
      	i++;
      }
      while(i <n && buf[i] != ':')ipasswd[l++] = buf[i++];
      if(i == n) break;
      ipasswd[l] = '\0';
      c = 0;
      l = 0;
     
      if(!strcmp(user,iuser) && !strcmp(passwd,ipasswd))
      	return 1;
      while(i <n && buf[i++] != '\n');

    }
  }

  return 0;
  
}


int main(int argc, char *argv[]){
  int fd;

  if(argc <= 2){
    printf(1,"Usage: testuser file use passwd");
    exit();
  }

  if((fd = open(argv[1], 0)) < 0){
    printf(1, "testuser: cannot open %s\n", argv[1]);
    exit();
  }
  if( checkpasswd(fd,argv[2],argv[3])){
  	printf(1,"password correct\n");
  }else{
  	printf(1,"wrong user or password\n");
  }
    close(fd);
  
  exit();
}

