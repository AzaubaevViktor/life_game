#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core.h"

#define DEBUG_MEMORY_
#define DEBUG_NEIGHBORS_
#define DEBUG_NEIGHBORS_STEP_

long int mod(const long int a, const long int b) {
    register long int x = a % b;
    if (x < 0) return b + x;
    return x;
}

char **get_field(const long int X, const long int Y, ERROR *error) {
    long int x=0;
    char **result = 0;

    result = (char **) malloc(sizeof(char *) * X);
#ifdef DEBUG_MEMORY
    printf("MEM | Memory allocated for *X, %li elements\n", X);
#endif

    if (0 == result) {
        error->error = MEMORY_ALLOCATE_ERROR;
        strcpy(error->value,"\0");
        return 0; }

    for (x=0;x<X;x++) {
        *(result + x) = (char *) malloc(sizeof(char *) * Y);
#ifdef DEBUG_MEMORY
        printf("MEM | Memory allocated for x=%li, %li elements\n",x,Y);
#endif
        /* Если память не выделилась -- валимся с ошибкой */
        if (0 == *(result + x)) {
            error->error = MEMORY_ALLOCATE_ERROR;
            strcpy(error->value,"\0");
            return 0; }
        ; }


    return result; }

void null_field(const char **field, const long int X, const long int Y) {
    long int x = 0, y = 0;
    char **res_field = (char **) field;
    char *res = 0;
    for (x=0;x<X;x++) {
        for (y=0;y<Y;y++) {
            res = (res_field[x] + y);
            if ((*res)) {
                *res = 0; }
        }
    }
}

char ****get_neighbors(const char **field, const long int X, const long int Y, ERROR *error) {
    /* individual array for count of neighbors for efficiently, but uses are more memory */
    long int x = 0, y = 0, xi = 0, yi = 0, offset = 0;
    char ****neighbors = 0;
    char *res = 0;

    neighbors = (char ****) calloc(X,sizeof(char ****));

#ifdef DEBUG_MEMORY
    printf("MEM | Memory allocated for neighbors ****, %li elements\n",X);
#endif

    if (0 == neighbors) {
        error->error = MEMORY_ALLOCATE_ERROR;
        strcpy(error->value,"\0");
        return 0; }

    for (x=0;x<X;x++) {
        neighbors[x] = (char ***) calloc(Y, sizeof(char ***));

        if (0 == neighbors[x]) {
            error->error = MEMORY_ALLOCATE_ERROR;
            strcpy(error->value,"\0");
            return 0; }

#ifdef DEBUG_MEMORY
        printf("MEM | -> Memory allocated for neighbors ***, x=%li, %li elements\n",x,Y);
#endif

        for (y=0;y<Y;y++) {
            neighbors[x][y] = (char **) calloc(8,sizeof(char **));

            if (0 == neighbors[x][y]) {
                error->error = MEMORY_ALLOCATE_ERROR;
                strcpy(error->value,"\0");
                return 0; }

#ifdef DEBUG_MEMORY
            printf("MEM | -> -> Memory allocated for neighbors **,[x,y]=[%li;%li], 8 elements\n",x,y);
#endif

            offset = 0;

#ifdef DEBUG_NEIGHBORS
            printf("[%li,%li]:\n",x,y);
#endif

            for (xi=-1;1>=xi;xi++) {
                for (yi=-1;1>=yi;yi++) {
                    if (yi || xi) {
                        /* pointer to field[x+xi][y+yi] */
                        res = &((char **)field)[mod(x+xi,X)][mod(y+yi,Y)];
                        neighbors[x][y][offset] = res;

#ifdef DEBUG_NEIGHBORS
                        printf("%p\n",neighbors[x][y][offset]);
#endif
                        offset++;
                    }
                }

#ifdef DEBUG_NEIGHBORS
                printf("\n");
#endif

            }
        }
    }

    return neighbors;
}

char set_cell(const char **field, const long int x, const long int y, char value) {
    *(*( ((char **) field) + x) + y) = value;
    return value; }

char get_cell(const char **field, const long int x, const long int y) {
    return *(*( ((char **) field) + x) + y); }

void print_field(const char **field, const long int X, const long int Y) {
    long int x = 0, y = 0;
    for (y=0;y<Y;y++) {
        for (x=0;x<X;x++) {
            printf("%c",(*(*(field+x)+y) * ('o'-' ') +' '));
        }
        printf("\n");
    }
}

void print_field_num(const char **field, const long int X, const long int Y) {
    long int x = 0, y = 0;
    for (y=0;y<Y;y++) {
        for (x=0;x<X;x++) {
            printf("%d",(*(*(field+x)+y)));
        }
        printf("\n");
    }
}

int num_in_rule(const int N, const int *rule) {
    int *_rule = (int *) rule;
    while ((*_rule != -1) && (N != *_rule)) {
        _rule++; }

    return (N == *_rule); }

int step(const char **field,const long int X, const long int Y, int *rule_born, int *rule_alive, ERROR *error) {
    static char ****neighbors = 0;
    static char **neighbors_num_field = 0;
    char *_cell = 0;
    long int x=0, y=0, offset = 0, _x=0, _y=0;

    if (0 == neighbors_num_field) {
        neighbors_num_field = get_field(X,Y,error);
        if (0 == neighbors_num_field) {
            error->error = MEMORY_ALLOCATE_ERROR;
            strcpy(error->value,"\0");
            return -1; } }

    if (0 == neighbors) {
        neighbors = get_neighbors((const char **) neighbors_num_field, X, Y, error);
        if (0 == neighbors) {
            error->error = MEMORY_ALLOCATE_ERROR;
            strcpy(error->value,"\0");
            return -1; } }

    null_field((const char **)neighbors_num_field, X, Y);

    for (x=0;x<X;x++) {
        for (y=0;y<Y;y++) {
            for (offset=0;offset<8;offset++) {
                /* считаем кол-во соседей; результат оказывается в neigbor_field */
                if (field[x][y]) {
                    (*neighbors[x][y][offset])++; }

#ifdef DEBUG_NEIGHBORS_STEP
                printf("[%li,%li]+%li\n",x,y,offset);

                for (_y=0;_y<Y;_y++) {
                    for (_x=0;_x<X;_x++) {
                        printf("%2d,",*(*(neighbors_field+_x)+_y)); }
                    printf("\n"); }
                printf("\n");
#endif
            } } }


    /* Game Step */
    for (x=0;x<X;x++) {
        for (y=0;y<Y;y++) {

            _cell = (char *)(field[x] + y);

            switch (*_cell) {
            case 0:
                if (num_in_rule(neighbors_num_field[x][y],rule_born)) {
                    *_cell = 1; }
                break;
            case 1:
                if (!num_in_rule(neighbors_num_field[x][y],rule_alive)) {
                    *_cell = 0; }
                break;
            default:
                break; } } }

//    print_field_num((const char **) neighbors_field,X,Y);
    return 0;
}


int read_info_from_file(FILE *input,int **rule_alive,int **rule_born,int ***field){
    ;
}

int print_error_string(ERROR error) {
    ;
}
