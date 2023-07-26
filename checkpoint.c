#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-------------CONSTS----------------------------
#define COURSES 10
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
    float grades[COURSES];
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
void display_first_10_students();


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


/*This function displays the first 10 students from the struct, this is done
after parsing the data from the file and populating it onto the data structure.
the function does so by looping through the levels and classes in the linked list.
This is done until the counter reaches 10, becuase we are only interested in the 
first ten users, basically this function is used for debug purposes*/
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

/*Main function*/
int main(){
    init_db();

    display_first_10_students();

    return 0;
}



