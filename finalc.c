#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

/*************** global variables *******************/
int flag_in = 0 , flag_out = 0 , flag_pipe = 0 ,flag_bg = 1,p[2] ,num_of_cmds=0,num_of_words[5]={0,0,0,0,0},in_file,out_file;
char *cmds[5][100] , *file_in , *file_out , read_line[100] ,*pipe_lines[10],*mainInput;
pid_t child;
/***************************************************/


/**************functions declarations ***********/

void handle_special();
void parsing();
void exe();
void exe_child();
void pipeing();
void isPipe();
void pipe_redirect();


int main()
{
    char *ret;
    
    while(1)
    {
        
        printf("mr.robot~>");
        ret = fgets(read_line,200,stdin);
        if(ret==NULL ||strcmp(read_line,"exit\n")==0)
            {printf("exit..\n");break;}
        isPipe();        
        if(flag_pipe)
        {
            pipeing();
        }
        else
        {
            handle_special();
            parsing();
            exe();
        }
        memset(cmds,0,sizeof(cmds));
        memset(read_line,0,sizeof(read_line));
        memset(pipe_lines,0,sizeof(pipe_lines));
        memset(p,0,sizeof(p));
        memset(num_of_words,0,sizeof(num_of_words));
        flag_in = 0 ; flag_bg = 0 ; flag_out = 0 ; flag_pipe = 0 ; 
    }
 
   
}

void isPipe()
{
    int i = 0 ;
    while(1)
    {
        if(read_line[i]=='|')
            flag_pipe++;
        else if (read_line[i]=='\n')
        {
            read_line[i]='\0';
            break;
        }
        i++;

    }
}
void handle_special()
{
    int i = 0 ;
    while(1)
    {
        if(read_line[i]=='>')
            {flag_out++; read_line[i]=' ';i++;}
        else if (read_line[i]=='<')
            {flag_in++; read_line[i]=' ';i++;}

        else if (read_line[i]=='&')
            flag_bg++;
        else if (read_line[i]=='\0')
        {
            break;
        }
        i++;      
    }
    if(flag_in>1 || flag_out>1)
        printf("Error in i/o redirect, too many redirect!\n");

}

void parsing()
{
    num_of_words[flag_pipe]=0;
    char *s;
    s = strtok (read_line," \0");
    while(s!=NULL)
    {
        cmds[flag_pipe][num_of_words[flag_pipe]] = s;
        num_of_words[flag_pipe]++;
        s = strtok (NULL," ");
    }
   cmds[flag_pipe][num_of_words[flag_pipe]]='\0';
}

void exe()
{
    
    child = fork();
    if(child==0)
        exe_child();
    
    waitpid(child,NULL,0);
    return;
}

void exe_child()
{
    int status;
    if(flag_in)
    {
        file_in = cmds[flag_pipe][num_of_words[flag_pipe]-1];
        cmds[flag_pipe][num_of_words[flag_pipe]-1] = '\0';
        in_file = open(file_in,O_RDONLY);
        int ret = dup2(in_file,STDIN_FILENO);
        if(ret<0)
            perror("i/p redirect error: \n");
        close(in_file);
        
    }
    
    if(flag_out)
    {
        file_out = cmds[flag_pipe][num_of_words[flag_pipe]-1];
        cmds[flag_pipe][num_of_words[flag_pipe]-1] = '\0';
        out_file = open(file_out,O_WRONLY | O_CREAT, 0644);
        int ret = dup2(out_file,1);
        if(ret<0)
            perror("o/p redirect error: \n");
        close(out_file);
    }
    status =  execvp(cmds[flag_pipe][0],cmds[flag_pipe]);
    if(status==-1)
        perror("cmd error:");

}

void pipeing()
{
    num_of_cmds = 0 ;
    char *s;
    s = strtok(read_line,"|");
    while(s!=NULL)
    {
        pipe_lines[num_of_cmds] = s;
        num_of_cmds++;
        s = strtok(NULL,"|");
    }
    flag_pipe = 0 ; 
    while(flag_pipe<num_of_cmds)
    {
        flag_in = 0 ; flag_out = 0 ;
        strcpy(read_line,pipe_lines[flag_pipe]);
        handle_special();
        if(flag_pipe!=0 && flag_in)
        {
            printf("only first cmd in pipe can have i/p redirect..\n");
            return;
        }
        else if (flag_pipe!=num_of_cmds-1 && flag_out)
        {
            printf("only last cmd in pipe can have o/p redirect..\n");
            return;
        }
       // printf("current line is :%s55\n",read_line);
        parsing();
        //printf("current cmd is :%s55\n",cmds[flag_pipe][1]);
        pipe_redirect();
        flag_pipe++;
    }
}

void pipe_redirect()
{
    //pipe(p);
    int file = 0 ;
    child = fork();
    if(child==0)
    {
        if(flag_pipe==0)
        {
           /* printf("first cmd is :%s\n",cmds[flag_pipe][0]);
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);*/
            file = open("pipe.txt",O_WRONLY | O_CREAT, 0644);
            int ret = dup2(file,1);
            close(file);
            exe_child();// cal execute func
        }
        else if (flag_pipe==num_of_cmds-1)
        {
            /*printf("sec cmd is :%s\n",cmds[flag_pipe][0]);
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);*/
            file = open("pipe.txt",O_RDONLY);
            int ret = dup2(file,STDIN_FILENO);
            close(file);
            exe_child();// cal execute func

        }
     
    }
    
    waitpid(child,NULL,0);
}

