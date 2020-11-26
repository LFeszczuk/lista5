#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define BUF_SIZE 256

int main() {

    pid_t pid;
    int fd[2], in_fd, n;
    char buf[BUF_SIZE];
    char fname[32];
    
    if(pipe(fd) < 0) {

        fprintf(stderr, "Failed creating pipe\n");
        return 1;
    }

    pid = fork();

    if(pid > 0) {
        //rodzic
        close(fd[0]);
        sprintf(fname,"text.txt");
        if((in_fd = open(fname, O_RDONLY)) < 0) {
            
            fprintf(stderr, "Failed opening file\n");
            return 2;
        }
        
        while((n = read(in_fd, &buf, BUF_SIZE)) > 0) {
    
            if(write(fd[1], &buf, n) < 0) {
                fprintf(stderr, "Failed writing to pipe\n");
                return 3;
            }  
        }
        close(in_fd);
    
    }
    else if(pid == 0) {
       //potomek
        close(fd[1]);
        
        while((read(fd[0], &buf, 1)) > 0)
        {
            write(STDOUT_FILENO,"#",1);
            write(STDOUT_FILENO,&buf,1);
            write(STDOUT_FILENO,"#",1);
            write(STDOUT_FILENO,"\n",1);
        }
        close(fd[0]);
    } 
}