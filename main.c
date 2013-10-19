#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core.h"

#define STANDART (0)
#define QUIET (1)
#define HELP (2)

void print_help();

int main(int argc, char *argv[])
{
    char **field = 0;
    char ****neighbors = 0;
    long int X = 132, Y = 43, N = 0, i = 0;
    int *rule_born = 0, *rule_alive = 0;
    ERROR error;
    FILE *input = NULL, *output = NULL;
    char filename_input[100] = {'\0'}, filename_output[100] = {'\0'};
    int parse_result = 0;
    int mode = STANDART;
    int x=0, y=0;

    error.error = 0;
    strcpy(error.value,"\0");

    /* parse command line arguments */
    while ( 1 != (parse_result = getopt(argc,argv,"i:o:qh")) ) {
        switch (parse_result) {
        case 'i': strcpy(filename_input,optarg); break;
        case 'o': strcpy(filename_output,optarg); break;
        case 'q': mode = mode ? mode : QUIET ; break;
        case 'h': mode = HELP; break;
        }
    }

    if (HELP == mode) {
        print_help();
        return 0; }

    /* Open input file */
    if ('\0' != filename_input) {
        input = fopen(filename_input,"rt");

        if (NULL == input) {
            printf("Error opening the file \"%s\", programm terminated",filename_input);
            return -1;
        } else {
            read_info_from_file(input,rule_alive,rule_born,field);
        }
    } else {
        /* random filling */
        rule_born = malloc(sizeof(int)*2);
        rule_alive = malloc(sizeof(int)*3);

        *rule_born = 3;
        *(rule_born+1) = -1;
        *rule_alive = 2;
        *(rule_alive + 1) = 3;
        *(rule_alive + 2) = -1;

        field = get_field(X,Y,&error);

        if (0 != error.error) {

        }

    }


//    /* инициилизируем правила */
//    *rule_revive = 4;
//    *(rule_revive+1) = 5;
//    *(rule_revive + 2) = 6;
//    *(rule_revive + 3) = 7;
//    *(rule_revive + 4) = 8;
//    *(rule_revive + 5) = -1;

//    rule_revive = malloc(sizeof(int)*8);
//    rule_alive = malloc(sizeof(int)*8);
//    *rule_alive = 2;
//    *(rule_alive + 1) = 3;
//    *(rule_alive + 2) = 4;
//    *(rule_alive + 3) = 5;
//    *(rule_alive + 4) = -1;
//    *(rule_alive + 5) = -1;

//    field = get_field(X,Y,&error);

//    /* инициализация поля */
//    srand(time(NULL));

//    for (x=0;x<X;x++) {
//        for (y=0;y<Y;y++) {
//            set_cell((const char **) field,x,y,(random() > RAND_MAX / 10));
//        }
//    }

//    N = 0;

//    while(100000 > N)
//    {
//        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//        print_field((const char **)field,X,Y);
//        printf("\n");
//        step((const char **) field,X,Y,rule_revive,rule_alive,&error);
//        for(i=0;i<50000000;i++);
//        N++;
//        if (N % 1000 == 0) printf("%li\n",N);
//    }

//    free(field);
//    free(neighbors);
//    free(rule_revive);
//    free(rule_alive);

    return 0;
}

void print_help() {
    ;
}
