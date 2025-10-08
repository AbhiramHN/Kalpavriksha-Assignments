#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
}User;

enum CrudOperation {
    ADD_USER = 1,
    DISPLAY_USERS = 2,
    UPDATE_USER = 3,
    DELETE_USER = 4,
    EXIT_PROGRAM = 5
};



FILE* openFile(const char *mode) {
    FILE *filePointer = fopen(FILENAME, mode);
    if (filePointer == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }
    return filePointer;
}

void closeFile(FILE *filePointer) {
    if (filePointer != NULL)
        fclose(filePointer);
}

void ensureFileExists() {
    FILE *filePointer = fopen(FILENAME, "a+");
    if (filePointer == NULL) {
        printf("File error\n");
        exit(1);
    }
    fclose(filePointer);
}

void createUser() {
    FILE *filePointer = openFile("a");
    if (filePointer == NULL) return;

    User user;
    printf("Enter User ID : ");
    scanf("%d", &user.id);
    printf("Enter Name : ");
    scanf("%s", user.name);
    printf("Enter Age : ");
    scanf("%d", &user.age);

    fprintf(filePointer, "%d,%s,%d\n", user.id, user.name, user.age);

    closeFile(filePointer);
    printf("User added!\n");
}

void readUsers() {
    FILE *filePointer = openFile("r");
    if (filePointer == NULL) return;

    User user;
    printf("\n--- User Records ---\n");
    while (fscanf(filePointer, "%d,%49[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        printf("ID:%d  Name:%s  Age:%d\n", user.id, user.name, user.age);
    }

    closeFile(filePointer);
}

void updateUser() {
    FILE *filePointer = openFile("r");
    FILE *tempFilePointer = fopen("temp.txt", "w");
    if (filePointer == NULL || tempFilePointer == NULL) {
        printf("Error opening file\n");
        return;
    }

    int userId, isFound = 0;
    User user;
    printf("Enter User ID to update: ");
    scanf("%d", &userId);

    while (fscanf(filePointer, "%d,%49[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == userId) {
            isFound = 1;
            printf("Enter new name : ");
            scanf("%s", user.name);
            printf("Enter new age : ");
            scanf("%d", &user.age);
        }
        fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    closeFile(filePointer);
    closeFile(tempFilePointer);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (isFound)
        printf("Updated!\n");
    else
        printf("Not found!\n");
}

void deleteUser() {
    FILE *filePointer = openFile("r");
    FILE *tempFilePointer = fopen("temp.txt", "w");
    if (filePointer == NULL || tempFilePointer == NULL) {
        printf("Error opening file\n");
        return;
    }

    int userId, isFound = 0;
    User user;
    printf("Enter User ID to Delete: ");
    scanf("%d", &userId);

    while (fscanf(filePointer, "%d,%49[^,],%d\n", &user.id, user.name, &user.age) == 3) {
        if (user.id == userId) {
            isFound = 1;
            continue;
        }
        fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    closeFile(filePointer);
    closeFile(tempFilePointer);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (isFound)
        printf("Deleted!\n");
    else
        printf("Not found!\n");
}


int main() {
    int userChoice;
    enum CrudOperation selectedOperation;
    int isRunning = 1;

    ensureFileExists();

    while (isRunning) {
        printf("\n==== User Management System ====\n");
        printf("1. Add User (Create)\n");
        printf("2. Display Users (Read)\n");
        printf("3. Update User (Update)\n");
        printf("4. Delete User (Delete)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        selectedOperation = (enum CrudOperation)userChoice;

        switch (selectedOperation) {
            case ADD_USER:
                        createUser();
                        break;

            case DISPLAY_USERS:
                        readUsers();
                        break;

            case UPDATE_USER:
                        updateUser();
                        break;

            case DELETE_USER:
                        deleteUser();
                        break;

            case EXIT_PROGRAM:
                        printf("Exiting program...\n");
                        isRunning = 0;
                        break;

            default:
                        printf("Invalid choice\n");
        }
    }

    return 0;
}
