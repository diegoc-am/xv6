#include "types.h"
#include "user.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
	int pid, wpid;
	printf(1,"Username: ");
	char * username = gets("username", 15);
	printf(1,"Password: ");
	char * password = gets("password", 15);

	if(username[0] != '\n' && password[0] != '\n')
		printf(1,"hello\n");
	dup(0);  // stdout
	dup(0);  // stderr

	//printf(1, "init: starting sh\n");
	pid = fork();
	if(pid < 0){
	  printf(1, "login: fork failed\n");
	  exit();
	}
	if(pid == 0){
	  exec("sh", argv);
	  printf(1, "login: exec sh failed\n");
	  exit();
	}
	while((wpid=wait()) >= 0 && wpid != pid)
	  printf(1, "zombie!\n"); 
	return 0;
}
