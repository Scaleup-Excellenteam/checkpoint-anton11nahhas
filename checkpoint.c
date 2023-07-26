#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

//-------------CONSTS----------------------------
#define COURSESLEN 10
#define NAMELEN 15
#define LEVELS 7
#define CLASSES 20
#define CELLLEN 13
#define LINESIZE 256
#define READMODE "r"
#define WRITEBINARYMODE "wb"
#define FILENAME "students_with_class.txt"
#define OPENFILEERR "Error openning file..\n"
#define WRITETOFILEERR "Error opening file for writing.\n"
#define BINARYFILE "school_data.bin"
#define MY_CONST_ARRAY_SIZE 10
#define MY_CONST_ARRAY {"Algebra", "Physics", "Biology", "Chemistry", "Physical Educations", "Human Biology", "Core Maths", "Statistis", "French", "English"}


//------------STRUCT SECTION-----------------------

//Student struct that stores all data of the user, first name, last name,
//cellphone number, level, class, and grades. In addtition theres a pointer
//of type student that points to the next student in the linked list.
struct student {
    char first_name[NAMELEN];
    char last_name[NAMELEN];
    char cellphone[CELLLEN];
    int level;
    int class;
    float grades[COURSESLEN];
    struct student* next;
};

//School struct represents a dynamic linked list of students, to identify
//a student use the level he is in and the class he attends.
struct school {
    struct student* DB[LEVELS][CLASSES];
};

static struct school S;


//---------------FUNCTION DECLARATIONS-----------------
void init_db();
void save_to_binary(const char* filename);
void display_students();
void display_menu();
void show_tasks_list();
void task_handler(int choice);


//---------------FUNCTION SECTION----------------------
/*
    This function opens the file of data in read mode, checks if it was succesffully 
    opened if not displays an error message. 
    Then the function reads each line from the file, extracts each input according to
    its type, and saved it in our data structure under the name of newStudent, 
    (which was allocated dynamically). Then accordingly save the student at the
    end of the linked list. Finally the save the file in binary, and close it. 
*/
void init_db() {
    FILE* file = fopen(FILENAME, READMODE);
        if (file == NULL) {
        printf(OPENFILEERR);
        return;
    }

    char buffer[LINESIZE];

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

    save_to_binary(BINARYFILE);

    fclose(file);
}


/*This function displays all students from our struct, which is the data retrieved from
    the attached file, this function is used to ensure that we parsed and populated the 
    data properly and correctly.
*/
void display_students() {

    for (int level = 0; level < LEVELS; level++) {
        const char* myConstArrayData[MY_CONST_ARRAY_SIZE] = MY_CONST_ARRAY;
        for (int class = 0; class < CLASSES; class++) { 
            struct student* currentStudent = S.DB[level][class];

            while (currentStudent != NULL) {
                printf("First Name: %s\n", currentStudent->first_name);
                printf("Last Name: %s\n", currentStudent->last_name);
                printf("Cellphone: %s\n", currentStudent->cellphone);
                printf("Level: %d\n", currentStudent->level);
                printf("Class: %d\n", currentStudent->class);

                printf("Grades: ");
                for (int i = 0; i < COURSESLEN; i++) {
                    printf("%s: ", myConstArrayData[i]);
                    printf("%.2f ", currentStudent->grades[i]);
                }
                printf("\n\n");

                currentStudent = currentStudent->next;
            }
        }
    }
}

void display_menu() {
    printf("========== Menu ==========\n");
    printf("1. Display all students\n");
    printf("2. Add a new student (Not implemented yet)\n");
    printf("3. Update student data (Not implemented yet)\n");
    printf("4. Delete a student (Not implemented yet)\n");
    printf("5. Exit\n");
    printf("==========================\n");
}

/**
 This function receives a file name, opens it in write binary mode, and writes to
 the data from our data structure.
*/
void save_to_binary(const char* filename) {
    FILE* file = fopen(filename, WRITEBINARYMODE);

    if (file == NULL) {
        printf(WRITETOFILEERR);
        return;
    }

    fwrite(&S, sizeof(struct school), 1, file);

    fclose(file);
}

void show_tasks_list(){
    while (true) {
        display_menu();

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        task_handler(choice);
    }    
}

void task_handler(int choice) {
    switch (choice) {
        case 1:
            display_students();
            break;
        case 2:
            printf("Adding a new student (Not implemented yet)\n");
            break;
        case 3:
            printf("Updating student data (Not implemented yet)\n");
            break;
        case 4:
            printf("Deleting a student (Not implemented yet)\n");
            break;
        case 5:
            printf("Exiting the program\n");
            exit(0);
        default:
            printf("Invalid choice. Please select a valid option.\n");
    }
}

/*Main function*/
int main(){
    init_db();
    show_tasks_list();

    return 0;
}



