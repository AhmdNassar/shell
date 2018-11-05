#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>



int flag_in = 0 , flag_out = 0 ,flag_pipe = 0, num_of_words = 0 ,in_file , out_file , firsLine = 0 ,pd[2];
char *command[100] ,*in_path , *out_path,* temp[50][50],*command2[100];
char  line[100], newLine1[50],newLine2[50];
pid_t child1,child2;

void wc_command();
void execute();
void  pwd ( );
void cd();
void ls();
void input_handle();
int  remove_endOfText();
void parsing(char lineToPars[],char *com[]);
void piping();
void pipePars(int index);



int main()
{
    int l = 1 ;
    while(1)
    {
        printf("@mr.robot:>");
        input_handle(); // handle user input (take it and remove \n from the end of it)
        memset(line,0,sizeof(line)); // make line  zeros
        memset(newLine1,0,sizeof(newLine1));
        memset(newLine2,0,sizeof(newLine2));
        num_of_words = 0;
        flag_in = 0 ; flag_out = 0 ;flag_pipe = 0; num_of_words = 0 ;in_file=0 ;out_file=0;
       // usleep(1000000);

    }
    return 0;
}




void input_handle()
{
    int  lineflag;
    fgets(line,100,stdin); // read line
    lineflag = remove_endOfText(); // remove \n from the end of input line and will be 1 if not empty command
    if(lineflag)
        parsing(line,command);
    if(!num_of_words&&lineflag) // if command ware just spaces we print no command enteredd
        printf("no command entered!\n");
    if(strcmp("exit",line) == 0) // check if user  input exit to close terminal
    {
        printf("exit..\n");
        exit(0);
    } 
    if(flag_in>1)
        {printf("too many '<'!\n");return;}
    if(flag_out>1)
        {printf("too many '>'!\n");return;}
    if(!flag_pipe && lineflag)
    {
    if(strcmp(command[0],"cd")==0)
        cd();
    /*else if(strcmp(command[0],"ls")==0)
        ls();*/
    else if (strcmp(command[0],"wc")==0)
      wc_command();
   /* else if (strcmp(command[0],"pwd")==0)
        pwd();*/
    else if (strcmp(command[0],"cd")==0)
        cd();
    else 
        execute();
    }
    


}

int  remove_endOfText()
{

    // this fun remove /n from the end of text and if we entered empty line will print that we don't enter any command
    // return:
    //        0 if empty command
    //        1 if not empty command
    int i = 0 ;
    if(line[0]=='\n') // check if user entered empty line
        {printf("no command entered!\n"); return 0;}
    else
    {
      while(1)
      {
        if(line[i]=='<')
            {flag_in++; line[i]=' '; i++;}
        else if (line[i]=='>')
            {flag_out++; line[i]=' ';i++;}
        else if (line[i]=='|')
            {flag_pipe++; line[i]='|';pipePars(i);return 0;}
        else if (line[i]=='\n')
            break;
        else
            i++;
      }
      line[i]='\0';
    }
    return 1;
}

void pipePars(int  index)
{
    int i = 0 ; 

    while(i<index)
    {
        newLine1[i]=line[i];
        i++;
    }
    newLine1[i]='\0';
    parsing(newLine1,command);    
    
    i = 0 ;
    index++;
    while(line[index]==' ')
    {
        index++;
    }
    while(1)
    {
        newLine2[i] = line[index];
        index++;
        i++;
        if(line[index]=='\n')
            {
                newLine2[i] ='\0';
                break;
            }
    }
    i = 0;
    parsing(newLine2,command2);


    pipe(pd);
    child1 = fork();
    if(child1==0)
    {
        close(1);
        dup(pd[1]);
        close(pd[0]);
        close(pd[1]);
        execvp(command[0],command);
        perror("First execvp() failed");

        return;
    }
    child2 = fork();
    if(child2==0)
    {
        close(0);
		dup(pd[0]);
        //dup2(pd[1],0);
		close(pd[0]);
		close(pd[1]);
        execvp(command2[0],command2);
        perror("Second execvp() failed");
        return;
    }
    close(pd[0]);
	close(pd[1]);
	/* Wait for the children to finish, then exit. */
	waitpid(child1,NULL,0);
	waitpid(child2,NULL,0);
}
void parsing(char lineToPars[],char *com[])
{
    num_of_words = 0 ;
    char *p;
    p = strtok (lineToPars," ");
    while(p!=NULL)
    {
        com[num_of_words] = p;
        num_of_words++;
        p = strtok (NULL," ");
    }

    com[num_of_words]='\0';
}




void execute()
{
    int status , i = 0 , j = 0  ; 
   /* if(flag_pipe)
    {
        while(i<num_of_words)
        {
            printf("outer loop j = %d\n",j);
            while(strcmp(command[i],"|")!=0 )
            {
                printf("inner loop j = %d\n",i);
                temp[j][i] = command[i];
                i++;
            }
            temp[j][i]='\0';
            i++;
            j++;
            
        }
        printf("finish piping pars\n");
        piping(temp);
        
    }*/
    if(flag_out)
    {
        out_path = command[num_of_words-1];
        command[num_of_words-1]='\0';
    }
    if(flag_in)
    {
        in_path = command[1];
        command[1]='\0';
    }

    
   child1 = fork();
   if(child1==0)
        {
             if(flag_in)
            {
                in_file = open(in_path,O_RDONLY); 
                int ret = dup2(in_file,STDIN_FILENO);
                if(ret<0)
                    perror("dub2 at in\n");
                close(in_file);
             }
             if(flag_out)
            {
                out_file = open(out_path,O_WRONLY | O_CREAT, 0644);
                int ret = dup2(out_file,1);
                if(ret<0)
                    perror("dub2 at out\n");
                close(out_file);
            }
            status =  execvp(command[0],command);
            if(status==-1)
                printf("ERROR\n");
        }
        
    else
        {
            waitpid(child1,NULL,0);
        }

}

void piping()
{
    int cmds = flag_pipe + 1 , p1[2],p2[2],i=0,fd_in=0;
    pipe(p1);
    pipe(p2);

    while(i<cmds)
    {         
        child1 = fork();        
        if(child1==0)
        {
            if(i==0) // first command
            {
                dup2(p1[1],1);
             }
            else if(i==flag_pipe)
            {
                dup2(p1[1],0);
            }
           /*else
            {
                close(1);
                close(0);
                dup2(p2[1],0);
                dup2(p1[1],1);
            }*/
        close(p1[0]);
        close(p1[1]);

           execvp(temp[i][0],temp[i]);
        }
        else
        {
               waitpid(child1,NULL,0);
               i++;
        } 
        
    }

}

void  pwd ( )
{
    if (num_of_words==1) //if user enter pwd only
    {
        execute(command);
    }
    else if (num_of_words >1) //check if user write option or not
    {
        if (command[1]=="-help"||command[1]=="-L"||command[1]=="-P"||command[1]=="-version")
        {
           execute(command);
        }
        else
        {
            printf("ERROR: %s not available option .. pwd has option [-L,-P,-help,-version]",command[1]);
        }
    }

}




void cd()
{
    DIR *dir ;
    if(num_of_words==1)
    {
        execute(command);

    }
    if(num_of_words>1)
    {
        dir = opendir(command[1]);
        if(!dir)
        {
            printf("%s not exist\n",command[1]);
        }
        else
            {

                execute(command);
            }
    }
}
void ls()
{
    // 29 options

    // list of available options for ls command
    char *options[] = { "..","../..","~","-l","-L","-a","-A","-al","-AL","-ls","--all","--almost-all","--author","-b","--escape","--ignore-backups","-B","-b","-c","-C","-color","--directory","--dired","-d","-D","-f","-F","--classify","--file-type"};
    int flag=0; // flag = 1 if accepted options and 0 if not
    if(num_of_words>1) // if we have options with ls
    {
        int i=1,j=0;
        char*current ;
        while (i < num_of_words) // iterate on all options to check it's accepted
        {
            current = command[i];
            flag = 0 ;
            while(j<29)
                {
                  if (strcmp(current,options[j])==0)
                    {
                       i++;
                       j=0;
                       flag=1;
                       break;
                    }
                  j++;
                }
            if(!flag) // if options not accepted
                {
                    printf("%s is not option for ls, accepted options is :\n",command[i]);
                    int k = 0 ;
                    while(k<29)
                    {
                        printf("%s,",options[k]);
                        k++;
                    }
                    printf("\n");
                    break;
                }
        }
    }
    if(num_of_words==1 || flag==1)
    {
        execute(command);
    }
}




void wc_command()
{
// handle command wc and call execute fun if command is correct
    char *accepted_par[]={"-c","--bytes","-m","--chars","-l","--lines","--files0-from=-","--files0-from=","-L","--max-line-length","-w","--words","--help","--version","-"};
    int start_index = 1;
    if(command[1][0]=='-') // check if user input option for wc
        {
            int i =0,flag=0;
            while(i<15) // check if option is valid
            {
                if(strcmp(command[1],accepted_par[i])==0)
                {
                    flag = 1;
                    break;
                }
                i++;
            }
            if(!flag) // option not valid
            {
                printf("%s not option for wc, options is:\n",command[1]);
                i = 0;
                while(i<15)
                {
                    printf("%s, ",accepted_par[i]);
                    i++;
                }
                printf("\n");
                return;
            }
            else // option is valid
            {
                if(strcmp(command[1],"-")==0||strcmp(command[1],"--files0-from=-")==0) // check if we will get input from stream
                {
                    // TODO : we should call execute and read input from stdin

                    // to test it's work
                    printf("we should take input from stdin \n");
                }
                else if(num_of_words==2) // check if user pass files to read from it
                   {
                       printf("missing argument!, you should enter one file name at least \n");
                       return ;
                   }
                else
                    start_index = 2; // to iterate on files name [command , option , first file has index = 2 ]

            }
        }
        // check that input file is exist
        while(start_index<num_of_words)
        {
            FILE *f;
            f =fopen(command[start_index],"r");
            if(f==NULL)
            {
                printf("file %s not exist!\n",command[start_index]);
                return ;
            }
            start_index++;
        }
       execute(command);

}