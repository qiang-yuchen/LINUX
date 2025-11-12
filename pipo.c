#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
        int fd[2];
        int res=pipe(fd);

        pid_t pid;
        pid=fork();
        char buf[1024];
        if(pid>0)
        {
                close(fd[1]);
                read(fd[0],buf,1024);
                printf("%s\n",buf);
                close(fd[0]);

                close(fd[0]);
                write(fd[1],"hello son",10);
                close(fd[1]);
        }
        else if(pid==0)
        {
                close(fd[0]);
                write(fd[1],"hello parent",13);
                close(fd[1]);

                close(fd[1]);
                read(fd[0],buf,1024);
                printf("%s\n",buf);
                close(fd[0]);
        }

        return 1;
}

