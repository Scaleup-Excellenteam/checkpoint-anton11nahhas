#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COURSES 10
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

void init_db();
void save_to_binary(const char* filename);
void display_first_10_students();


void init_db() {
    FILE* file = fopen("students_with_class.txt", "r");
        if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        struct student* newStudent = (struct student*)malloc(sizeof(struct student));
        int result = sscanf(buffer, "%s %s %s %d %d %f %f %f %f %f %f %f %f %f %f",
            newStudent->first_name, newStudent->last_name, newStudent->cellphone,
            &newStudent->level, &newStudent->class,
            &newStudent->grades[0], &newStudent->grades[1], &newStudent->grades[2],
            &newStudent->grades[3], &newStudent->grades[4], &newStudent->grades[5],
            &newStudent->grades[6], &newStudent->grades[7], &newStudent->grades[8],
            &newStudent->grades[9]);

        if (result != 15) {
            printf("Error parsing line: %s\n", buffer);
            free(newStudent);
            continue;
        }

        newStudent->next = NULL;

        if (S.DB[newStudent->level][newStudent->class] == NULL) {
            S.DB[newStudent->level][newStudent->class] = newStudent;
        } else {
            struct student* currentStudent = S.DB[newStudent->level][newStudent->class];
            while (currentStudent->next != NULL) {
                currentStudent = currentStudent->next;
            }
            currentStudent->next = newStudent;
        }
    }

    save_to_binary("school_data.bin");

    fclose(file);
}

void display_first_10_students() {
    int count = 0;

    for (int level = 0; level < LEVELS; level++) {
        for (int class = 0; class < CLASSES; class++) {
            struct student* currentStudent = S.DB[level][class];

            while (currentStudent != NULL && count < 10) {
                printf("First Name: %s\n", currentStudent->first_name);
                printf("Last Name: %s\n", currentStudent->last_name);
                printf("Cellphone: %s\n", currentStudent->cellphone);
                printf("Level: %d\n", currentStudent->level);
                printf("Class: %d\n", currentStudent->class);

                printf("Grades: ");
                for (int i = 0; i < COURSES; i++) {
                    printf("%.2f ", currentStudent->grades[i]);
                }
                printf("\n\n");

                currentStudent = currentStudent->next;
                count++;

                if (count >= 10) {
                    break; // Displayed the first 10 students, exit the loop
                }
            }

            if (count >= 10) {
                break; // Displayed the first 10 students, exit the loop
            }
        }
    }
}

void save_to_binary(const char* filename) {
    FILE* file = fopen(filename, "wb");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(&S, sizeof(struct school), 1, file);

    fclose(file);
}

int main(){
    init_db();

    display_first_10_students();

    return 0;
}



