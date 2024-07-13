// hostsblock made by datanomer

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
#include <unistd.h>
#define HFILE "/etc/hosts"
#define LOCALH "localhost"
#define LOCALIP6 "::1"
FILE * filep;

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
            printf("Found on line %d, %s", line_num, temp); 
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
    time_t timestamp;
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input_buf, 0, NULL, 0);

    if (regval == REG_NOMATCH)
    {
        printf("Not valid, try again.");
    } 
    else if (regval == 0)
    {
        if (block_exists(input_buf) == true) {
            fclose(filep); 
            exit(0);
        }

        filep = fopen(HFILE, "a+");
        time(&timestamp);
        fprintf(filep,"\n" LOCALH "       %s #Timestamp: %s ", input_buf, ctime(&timestamp));
        fprintf(filep,""LOCALIP6 "             %s ", input_buf);
        printf("Blocking %s ...\n", input_buf);
        
        fclose(filep);
        regfree(&rx);
        system("cat "HFILE"");
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
    regex_t rx;
    int regval;
    regval = regcomp(&rx, "[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,3}(/[^[:space:]]*)?$" , REG_EXTENDED); 
    regval = regexec(&rx, input_buf, 0, NULL, 0);
    FILE * tempfp;
    int counter = 0;
    char blankline = '\0';
    char buffer[256];
    int line_num = 1;

    if (regval == REG_NOMATCH) 
    {
        printf("Not valid, try again.");
    }
    else if (regval == 0) {
       
        if (block_exists(input_buf) == false) {
            fclose(filep); 
            exit(0);
        }
        filep = fopen(HFILE, "r");
        tempfp = fopen(".hbtemp.txt", "w");    
       
        while ((fgets(buffer, 256, filep)) != NULL) {
            if ((strstr(buffer, input_buf)) == NULL) {
                fflush(tempfp);
                if (counter == line_num) {
                    fputs(&blankline, tempfp);
                }
                else {
                    fputs(buffer, tempfp);
                }        
                fflush(tempfp);
                counter++;
            }
        line_num++;
        }
                
        printf("\n%s unblocked ... \n ", input_buf);
    
        rename(".hbtemp.txt", "hosts"); 
        system("mv hosts "HFILE"");
        //system("cat testetc/hosts");
        remove(".hbtemp.txt");
        remove("hosts");
        system("rm -rf hosts");
        fclose(filep);
        fclose(tempfp);
        system("cat "HFILE"");
    }

    else {
        printf("Something went shit.");
        if (filep == NULL) {
            printf("Aborting...\n");
            fclose(filep);
            fclose(tempfp);
        }
    }
}

int main(int argc, char** argv)
{

    char input_buf[128];

    printf("|--hostsblock--|\n");
    printf("Choose option: \n1. Block site\n2. Remove block from site \n");
    printf("---------|/etc/hosts|----------\n");
    system("cat "HFILE"");
    printf("\n--------------EOF--------------\n");
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
        return 1;
    }

    return 0;
    
}
