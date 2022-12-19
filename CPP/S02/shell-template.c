#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "readcmd.h"


void trace_cmd(struct cmdline *l) {

		int i, j;

		/* Display each command of the pipe */
		printf("trace_cmd\n");
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j=0; cmd[j]!=0; j++) {
				printf("(%s) ", cmd[j]);
			}
			printf("\n");
		}
}

void change_dir(char *param) {
	char *curr_dir;
	char buf[1024];
	char *path;
	if ((curr_dir=getcwd(NULL, 0)) == NULL) {
		perror("cd failed (getcwd)");
		return;
	}
	if (!param) path = getenv("HOME");
	else 
		if (strncmp(param,"/",1)) {
			if ((strlen(curr_dir)+strlen(param)+2)>1024) {
				perror("cd failed (path too large)");
				free(curr_dir);
				return;
			}
			path = strcpy(buf,curr_dir);
			path = strcat(path, "/");
			path = strcat(path, param);
		} else path = param;
		//printf("%s\n",path);
		if (chdir(path)) {
			perror ("cd failed (chdir)");
			chdir(curr_dir);
		}
		free(curr_dir);
}



int main()
{
	int spid, status;
	struct cmdline *l;
	char ***seq;
	
	while(1) {
		printf("shell> ");

		l = readcmd();
		trace_cmd(l);
		seq = l->seq;

		if (! *seq) continue;

		if(!strcasecmp(**seq, "exit")) {
			return 0;
		}

		if(!strcasecmp(**seq, "cd")) {
			change_dir((*seq)[1]);
			continue;
		}

			
		
	}
}
