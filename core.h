#ifndef CORE_H
#define CORE_H

#define MEMORY_ALLOCATE_ERROR -1;

typedef struct {
    int error;
    char value[100];
} ERROR;

char **get_field(const long int X, const long int Y, ERROR *error);
char set_cell(const char **field, const long x, const long y, char value) ;
char get_cell(const char **field, const long int x, const long int y);
void print_field(const char **field, const long int X, const long int Y);
int step(const char **field,const long int X, const long int Y, int *rule_revive, int *rule_alive, ERROR *error);
int read_info_from_file(FILE *input,int **rule_alive,int **rule_born,int ***field);

#endif // CORE_H
