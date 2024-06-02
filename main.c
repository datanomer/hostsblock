#include <stdio.h>
#include <stddef.h>
#include <regex.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
//#include <malloc.h>
#include <assert.h>
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
    time_t timestamp;
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input_buf, 0, NULL, 0);

    if (regval == 0)
    {
        time(&timestamp);
        fprintf(filep,"\n" LOCALH "       %s #Timestamp: %s ", input_buf, ctime(&timestamp));
        fprintf(filep,""LOCALIP6 "             %s ", input_buf);
        printf("Blocking %s ...\n", input_buf);
        
        fclose(filep);
        system("cat /etc/hosts");
    }
    else if (regval == REG_NOMATCH)
    {
        printf("Not valid, try again: ");
        fclose(filep);
    }
    else
    {
        printf("something happened\n");
        if (filep == NULL) {
            printf("Aborting...\n");
        };
    }
}
void block_del(char *input_buf)
{
    regex_t rx;

    int regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    int result = regexec(&rx, input_buf, 0, NULL, 0);
     
    char *line = NULL;
    size_t line_len = 0;

    if (result == 0) {
    
        printf("Unblocking %s ... \n ", input_buf);
    
        while(getline(&line, &line_len, filep) != -1)
        {
            if (result == 0) {
                fprintf(filep,"\n#" LOCALH "       %s \n", input_buf);
                fprintf(filep,"#"LOCALIP6 "             %s ", input_buf);
            }
    //delete blocking lines from hfile
            //fprintf(filep, "%s \n", input_buf);           
        }
        printf("%s Unblocked.\n", input_buf);
        assert(feof(filep));
        regfree(&rx);
        system("cat /etc/hosts");
        //system("cat /etc/hosts");
    }
    else if (result == REG_NOMATCH) {
        printf("Not valid URL");
        regfree(&rx);
    }
    else {
        printf("Something went shit.");
        if (filep == NULL) {
            printf("Aborting...\n");
        };
    }

    // does not comment out wanted lines in the file    

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
        fclose(filep);
    }
    else if (in == 2) {
        printf("Type the site url to be unblocked(ex. reddit.com): ");
        scanf("%s", input_buf);
        block_del(input_buf);
        fclose(filep);
    }
    else
    {
        printf("Not valid number, try again\n");
        fclose(filep);
    }
    return 0;
    
}
