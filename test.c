#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void handle_input(char c[],char *pars[])
{
    int num_of_words;
    fgets(c,100,stdin);
    remove(c);
    num_of_words = parsing(c, pars);
    printf("num of  words is :%d\n",num_of_words);

}

int parsing(char line[],char *pars[])
{
    int i = 0 , j = 0 ;
    char *p;
    p = strtok (line," /");
    while(p!=NULL)
    {
        pars[j] = p;
        printf("%s",pars[j]);
        j++;
        p = strtok (NULL," /");
    }
    return j;

}
void remove_end_ofLine(char c[])
{
    int i = 0 ;
    char ch ;
    while(1)
    {
        ch = c[i];
        if(ch!='\n')
            i++;
        else
        {
            c[i]='\0';
            break;
        }
    }
}
void print()
{
    printf("test>");
}

int main()
{
    char c [100];
    char *pars[100];
    while(1)
    {
        print();
        handle_input(c,pars);


    }
    return 0;
}
