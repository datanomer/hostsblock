#include <stdio.h>
#include <stddef.h>
#include <regex.h>
#include <sys/stat.h>
#include <stdlib.h>

#define HFILE "/etc/hosts"
//#define LOCALIP4 "127.0.0.1"
#define LOCALH "localhost"
#define LOCALIP6 "::1"
FILE *filep;

// plan: ability to remove blocks from sites
// straight from command line execution(ex. user@machine:~$ hblock twitter.com)


void block_add(char *input_buf)
{
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input_buf, 0, NULL, 0);

    if (regval == 0)
    {
        fprintf(filep,"\n" LOCALH "       %s", input_buf);
        fprintf(filep,"\n" LOCALIP6 "             %s", input_buf);
        printf("Blocking %s ...", input_buf);
        fclose(filep);
    }
    else if (regval == REG_NOMATCH)
    {
        printf("Not valid, try again: ");
    }
    else
    {
        printf("something happened\n");
        
        if (filep == NULL) {
            printf("Aborting...\n");
        };
    }
}
void block_srch(char *input_buf)
{
    regex_t rx;
    int regval_input_buf;
    int regval_ip4;
    int regval_ip6;
    regval_input_buf = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval_input_buf = regexec(&rx, input_buf, 0, NULL, 0);
    
    int line_pos = ftell(filep);
    char thing =fscanf(filep, "%s", input_buf);
    fseek(filep, line_pos, SEEK_SET); // fseek(stream to modify, offset, origin)
    fwrite("#", 1, 1,filep); // finishing this tomorrow
    // does not comment out wanted lines in the file     
}


void block_del(char *input_buf)
{
    regex_t rx;
    int regval_input_buf;
    int regval_ip4;
    int regval_ip6;
    regval_input_buf = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval_input_buf = regexec(&rx, input_buf, 0, NULL, 0); 

    //below maybe needed maybe not
    //regval_ip4 = regcomp(&rx, "", REG_EXTENDED); 
    //regval_ip4 = regexec(&rx, input_buf, 0, NULL, 0 );
    //regval_ip6 = regcomp(&rx, "", REG_EXTENDED); 
    //regval_ip6 = regexec(&rx, input_buf, 0, NULL, 0 );
    //could be put in a struct

   if (regval_input_buf == 0) {
       printf("\rFound site, removing block on %s...\n", input_buf);
       block_srch(input_buf);

   } 
   else if (regval_input_buf == REG_NOMATCH) {
       printf("Cant find the blocked site in file or the input_buf is invalid. Aborting... ");
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

    filep = fopen(HFILE, "a+");

    char input_buf[40]; 
    
    printf("|--hblock--|\n"); 
    printf("Choose option: \n1. Block site\n2. Remove block from site \n");
    printf("Choice: ");
    int in;
    scanf("%d", &in);

    if (in == 1) {
        printf("Type the site url (ex. twitter.com): ");  
        scanf("%s", input_buf);
        block_add(input_buf);
    }
    else if (in == 2) {
        printf("Type the site url to be unblocked(ex. reddit.com): ");
        scanf("%s", input_buf);
        block_del(input_buf);
    }
    else
    {
        printf("Not valid number, try again\n");
    }
    return 0;
    
}
