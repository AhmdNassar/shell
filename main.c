#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void input_handle(char c [],char *pars[])
{
   int num_of_words=0 , lineflag;
   fgets(c,100,stdin); // read line
   lineflag = remove_endOfText(c); // remove \n from the end of input line and will be 1 if not empty command
   if(lineflag)
        num_of_words = parsing(c,pars);
        if(!num_of_words&&lineflag) // if command ware just spaces we print no command entered
            printf("no command entered!\n");

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

int parsing(char line[],char *pars[])
{

     int i = 0 , j = 0 ;
    char *p;
    p = strtok (line," /");
    while(p!=NULL)
    {
        pars[j] = p;
        j++;
        p = strtok (NULL," /");
    }
    return j;
}


void print_rooe()
{
    printf("@mr.robot:>");
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
       /* else
        {
            printf(execl('/bin/pwd','pwd',NULL));
        }*/
    }
    exit(1);

    return 0;
}
