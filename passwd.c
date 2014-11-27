#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define MAXLEN 30

int makeTempFile(char * username, char * npass){
  char buf[1024], buftemp [1024], newuser[1024];
  int i;
  int fd = open("/shadow", O_RDONLY);
  //printf(1,"%s", username);
  int tempFile = open("/shadow_temp", O_CREATE | O_RDWR);
  read(fd, buf, sizeof(buf));
  //printf(1,"%s\n", buf);
  int pos = 0;
  int postemp = 0;
  int found = 0;
  char uname[MAXLEN];
  while(buf[pos]!=0 && !found){
    uname[postemp] = buf[pos];
    buftemp[postemp] = buf[pos];
    if(buf[pos] == ':'){
      uname[postemp] = 0;
      //printf(1,"Comparando %s con %s\n", username, uname);
      if(!strcmp(username, uname)){
        printf(1,"Changing %s's password\n", username);
        found=1;
      }
      if(found){
        pos++;
        strcpy(newuser,buftemp);
        strcpy(newuser+strlen(newuser),npass);
        postemp=strlen(newuser);
        while(buf[pos]!=':'){
          pos++;
        }
        buftemp[0]=0;
        postemp=0;
        while(buf[pos]!='\n'){
          buftemp[postemp++] = buf[pos++];
        }
        buftemp[postemp] = '\n';
        strcpy(newuser+strlen(newuser),buftemp);
        //printf(1,"Usuario con nueva pass: %s\n", newuser);
        write(tempFile, newuser, strlen(newuser));
        buftemp[0]=0;
        postemp=0;
      }
      else{
        while(buf[pos]!='\n'){
          buftemp[postemp++] = buf[pos++];
        }
        buftemp[postemp] = '\n';
        write(tempFile, buftemp,strlen(buftemp));
      }
      postemp = -1;
      //printf(1,"%s", buftemp);
      for (i = 0; i < sizeof(buftemp); i++){
        buftemp[i] = 0;
      }
      for (i = 0; i < sizeof(uname); i++){
        uname[i] = 0;
      }
    }
    postemp++;
    pos++;
  }
  if(!found){
    printf(1,"User %s not found\n", username);
    exit();
  }
  postemp=0;
  while(buf[pos]!=0){
    while(buf[pos]!='\n'){
      buftemp[postemp++] = buf[pos++];
    }
    buftemp[postemp] = '\n';
    write(tempFile, buftemp,strlen(buftemp));
    postemp = 0;
    
    for (i = 0; i < sizeof(buftemp); i++){
        buftemp[i] = 0;
    }
    pos++;
  }
  //printf(1," USUARIO: %s", newuser);
  if(found){    
    close(fd);
    close(tempFile);
    unlink("/shadow");
    fd = open("/shadow", O_CREATE | O_RDWR);
    tempFile = open("/shadow_temp", O_CREATE | O_RDWR);
    read(tempFile, buftemp, sizeof(buftemp));    
    write(fd, buftemp, strlen(buftemp));
  }
  close(tempFile);
  unlink("/shadow_temp");
  exit();
  return 0;
}

int main(int argc, char *argv[]){
  if (argc <= 2){
    printf(1,"Usage: passwd username new_password\n");
    exit();
  }
  else{
    makeTempFile(argv[1], argv[2]);
    exit();
  }
  return 0;
}