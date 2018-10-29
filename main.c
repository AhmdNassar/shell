#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void input_handle(char c [],char *pars[])
{
   int num_of_words=0 , lineflag;
   fgets(c,100,stdin); // read line
   lineflag = remove_endOfText(c); // remove \n from the end of input line
   if(lineflag)
        num_of_words = parsing(c,pars);
   printf("num of words is : %d\n",num_of_words);
   int i = 0;
   while(i<num_of_words)
   {
       printf("%s\n",pars[i]);
       i++;
   }
}

int  remove_endOfText(char line [])
{
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

    int i = 0 , j = 0 , k = 0;
    char ch,l[30],prev_ch='k',*word;

    while(1)
    {
        ch = line[i];
        if(ch ==' '&& prev_ch !=' ')
        {
            l[k]='\0';
            word = l;
            pars[j]=malloc(sizeof(word));
            strcpy(pars[j],word);
            k=0;
            j++;
        }
        else if (ch=='\0')
        {
            l[k]='\0';
            word = l;
            pars[j]=malloc(sizeof(word));
            strcpy(pars[j],word);
            j++;
            break;
        }
        else
        {
            if(ch!=' ')
                {
                    l[k] = ch;
                    k++;
                }
        }
        prev_ch=ch;
        i++;
    }
    return j;
}


void print_rooe()
{
    printf("@Nassar:>");
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
