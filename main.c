#include <stdio.h>
#include <stddef.h>
#include <regex.h>
#include <sys/stat.h>
#include <stdlib.h>

#define HFILE "/etc/hosts"
//#define eFILE "./test" testing purposes
#define LOCALIP4 "127.0.0.x"
#define LOCALIP6 "::1"
FILE *filep;
FILE *srcfile;

// plan: ability to remove blocks from sites
// straight from command line execution(ex. user@machine:~$ hblock twitter.com)

void block_add(char *input)
{
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input, 0, NULL, 0);

    if (regval == 0)
    {
        fprintf(filep,"\n" LOCALIP4 "       %s", input);
        fprintf(filep,"\n" LOCALIP6 "             %s", input);
        printf("Blocking %s ...", input);
        fclose(filep);
    }
    else if (regval == REG_NOMATCH)
    {
        printf("not good, try again: ");
    }
    else
    {
        printf("something happened\n");
        
        if (filep == NULL) {
            printf("Aborting...\n");
        };
    }
}

void block_del(char *input)
{
    regex_t rx;
    int regval_input;
    int regval_ip4;
    int regval_ip6;
    regval_input = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval_input = regexec(&rx, input, 0, NULL, 0);
    //regval_ip4 = regcomp(&rx, "", REG_EXTENDED); 
    //regval_ip4 = regexec(&rx, input, 0, NULL, 0 );
    //regval_ip6 = regcomp(&rx, "", REG_EXTENDED); 
    //regval_ip6 = regexec(&rx, input, 0, NULL, 0 );
    //could be put in a struct

   if (regval_input == 0) {
        printf("Searching...\n");
                     
   } 
   else if (regval_input == REG_NOMATCH) {
       printf("Cant find the blocked site in file or the input is invalid. Aborting... ");
       exit(EXIT_SUCCESS);
   }
   else {
       printf("Unexpected error.\n");   
        
        if (filep == NULL) {
            printf("Aborting...\n");
        }
       exit(EXIT_FAILURE);
   }
}

int main(int argc, char** argv)
{
 
    mode_t mode = 0000;
    chmod(HFILE, mode);
    filep = fopen(HFILE, "a");

    char input[40]; 
        
    printf("|--hblock--|\n"); 
    printf("Choose option: \n1. Block site\n2. Remove block from site \n");
    printf("Choice: ");
    int in;
    scanf("%d", &in);

    if (in == 1) {
        printf("Type the site url (ex. twitter.com): ");  
        scanf("%s", input);
        block_add(input);
    }
    else if (in == 2) {
        printf("Choose the site to be unblocked(ex. reddit.com): ");
        scanf("%s", input);
        block_del(input);
    }
    else
    {
        printf("Not valid number, try again\n");
    }
    // not sure if necessary and also may be a vuln :D
    mode_t mode2 = 0644;
    chmod(HFILE, mode2);
    return 0;
    
}
