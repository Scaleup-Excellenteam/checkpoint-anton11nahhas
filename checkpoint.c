#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COURSES 20
#define NAMELEN 15
#define LEVELS 7
#define CLASSES 20
#define CELLLEN 13

struct student {
    char first_name[NAMELEN];
    char last_name[NAMELEN];
    char cellphone[CELLLEN];
    int level;
    int class;
    float grades[COURSES];
    struct student* next;
};

struct school {
    struct student* DB[LEVELS][CLASSES];
};

static struct school S;


int main(){
    printf("hello world\n");

    return 0;
}