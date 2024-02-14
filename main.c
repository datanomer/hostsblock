#include <stdio.h>
#include <stddef.h>
#include <regex.h>
#include <sys/stat.h>

#define HFILE "/etc/hosts"
//#define eFILE "./test" testing purposes
#define LOCALIP4 "127.0.0.1"
#define LOCALIP6 "::1"
FILE *filep;

// plan: ability to remove blocks from sites
// straight from command line execution(ex. user@machine:~$ hblock twitter.com)

int main(int argc, char** argv)
{

in: 
    printf("|--hblock--|\n"); 
    printf("Type the site url (ex. twitter.com): ");   

    mode_t mode = 0000;
    chmod(HFILE, mode);

    filep = fopen(HFILE, "a");

    char input[40]; 
    scanf("%s", input);

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
        //goto in;
    }
    else
    {
        printf("something happened\n");
        goto in;
    }
    
    if (filep == NULL) {
        printf("Cant find file. aborting...\n");
        return 1;
    }

    mode_t mode2 = 0644;
    chmod(HFILE, mode2);
    return 0;
    
}
