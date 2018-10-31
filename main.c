#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include <sys/wait.h>

//check the name of fun> echo

void  echo ( char *line[])
 {
    if (line[0]=="echo")
 {
    printf("execut");
      }
    else
  {
    printf("%s","error:\n echo has options [-e,-n,-E,--help,--version]");
        }
  }
//num of string

void  pwd ( char *line[],int num )
{
   if (num==1) //if user enter pwd only
    {
   printf("execut");
    }
   else if (num >1) //check if user write option or not
    {
      if (line[1]=="-help"||line[1]=="-L"||line[1]=="-P"||line[1]=="-version")
    {
      printf("execut");
        }
     else
    {
      printf("%s","error:\n pwd has option [-L,-P,-help,-version]");
     }
           }
  }






void input_handle(char c [],char *pars[])
{
   int num_of_words=0 , lineflag;
   fgets(c,100,stdin); // read line
   lineflag = remove_endOfText(c); // remove \n from the end of input line and will be 1 if not empty command
   if(lineflag)
        num_of_words = parsing(c,pars);
        if(!num_of_words&&lineflag) // if command ware just spaces we print no command entered
            printf("no command entered!\n");
        /*else
        {
            int i = 0 ;
            while(i<num_of_words)
            {
                printf("%s\n",)
            }
        }*/
    if(strcmp(pars[0],"ls")==0)
        ls(pars,num_of_words);
    else
      wc_command(pars,num_of_words);

    // here we will check what is the command and then we call fun which response for handle this fun
}

int  remove_endOfText(char line [])
{

    // this fun remove /n from the end of text and if we entered empty line will print that we don't enter any command
    // return:
    //        0 if empty command
    //        1 if not empty command
    int i = 0 ;
    if(line[0]=='\n') // check if user entered empty line
        {printf("no command entered!\n");
        return 0;
        }

    else
    {
      while(1)
      {
          if (line[i]!='\n')
            i++;
          else
            break;
      }
      line[i]='\0';
    }
    return 1;
}

void ls(char *ls[], int c)
{
    // 29 options

    // list of available options for ls command
    char *options[] = { "..","../..","~","-l","-L","-a","-A","-al","-AL","-ls","--all","--almost-all","--author","-b","--escape","--ignore-backups","-B","-b","-c","-C","-color","--directory","--dired","-d","-D","-f","-F","--classify","--file-type"};
    int flag=0; // flag = 1 if accepted options and 0 if not
    if(c>1) // if we have options with ls
    {
        int i=1,j=0;
        char*current ;
        while (i < c) // iterate on all options to check it's accepted
        {
            current = ls[i];
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
                    printf("%s is not option for ls, accepted options is :\n",ls[i]);
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
    if(c==1 || flag==1)
    {
        // call execute fun
        //printf("correct command we call execute \n"); // for test
        execute(ls);
    }
}

int parsing(char line[],char *pars[])
{

     int i = 0 , j = 0 ;
    char *p;
    p = strtok (line," ");
    while(p!=NULL)
    {
        pars[j] = p;
        j++;
        p = strtok (NULL," ");
    }
    pars[j]='\0';
    return j;
}


void print_rooe()
{
    printf("@mr.robot:>");
}



void wc_command(char *command[] , int num_of_words)
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
                return 0;
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
                       return 0;
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
                return 0;
            }
            start_index++;
        }
       execute(command);

}

void execute(char *command[])
{
   // printf("command is : %s\n",command[0]);
    //(execvp(command[0],command));    /*
   int status;
   pid_t child = fork();
    if(child==0)
        execvp(command[0],command);
    else
        waitpid(child,NULL,0);


}

int main()
{
    char *pars[100];
    int l = 1 ;
    while(l!=0)
    {
        print_rooe();
        char  c[100]; // list of char where we store user input == command line
        input_handle(c,pars); // handle user input (take it and remove \n from the end of it)
        if(strcmp("exit",c) == 0) // check if user  input exit to close terminal
            l=0;
        else
        {
            memset(c,0,sizeof(c)); // make c  zeros
            //printf(execl("/bin/pwd","pwd",NULL));

        }
       /* else
        {
            printf(execl('/bin/pwd','pwd',NULL));
        }*/
    }
    exit(1);

    return 0;
}
