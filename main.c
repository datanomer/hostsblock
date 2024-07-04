#include <stdio.h>
#include <stddef.h>
#include <regex.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
//#include <malloc.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define HFILE "test"
//#define LOCALIP4 "127.0.0.1"
#define LOCALH "localhost"
#define LOCALIP6 "::1"
FILE * filep;
// plan: ability to remove blocks from sites
// straight from command line execution(ex. user@machine:~$ hblock twitter.com)
bool block_exists(char *input_buf)
{
    int line_num = 1; 
    int block_found = 0;
    char temp[256];

    if ((filep = fopen(HFILE, "r")) == NULL) {
        return -1;
    }

    while ((fgets(temp, 256, filep)) != NULL) {
        if ((strstr(temp, input_buf)) != NULL) {
            printf("Found match on line: %d\n", line_num); // does not show correct linenum
            printf("Found in file: %s\n", temp);
            block_found++;
        }
    line_num++;
    }

    if (block_found == 0) {
        printf("Didnt find the blocked site.\n");
        return false;
    }
    return true;
}

void block_add(char *input_buf)
{

    filep = fopen(HFILE, "a+");
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
        regfree(&rx);
        system("cat /etc/hosts");
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
            fclose(filep);
        }
    }
}

void block_del(char *input_buf)
{
    filep = fopen(HFILE, "+a");
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input_buf, 0, NULL, 0);

    if (regval == 0) {
        regfree(&rx);
        if (block_exists(input_buf) == true) {
            fopen(HFILE, "a+");
            fprintf(filep,"\n" LOCALH "       #%s \n", input_buf);
            fprintf(filep,""LOCALIP6 "             #%s", input_buf);
            printf("%s unblocked ... \n ", input_buf);
            //system("cat /etc/hosts");
        }
        
        fclose(filep);         
    }
    else if (regval == REG_NOMATCH) 
    {
        printf("Not valid, try again: ");
    }
    else {
        printf("Something went shit.");
        if (filep == NULL) {
            printf("Aborting...\n");
            fclose(filep);
        }
    }
}

int main(int argc, char** argv)
{

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
        fclose(filep);
    }
    return 0;
    
}
