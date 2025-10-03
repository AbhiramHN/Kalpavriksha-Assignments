#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

typedef struct {
    int id ;
    char name[50];
    int age ;
} User ;

void createUser();
 void readUsers();
void updateUser();
void deleteUser();
void ensureFileExists();

int main() {
    int choice ;

    ensureFileExists();

    while(1){
    printf("\n==== User Management System ====\n");
    printf("1. Add User (Create)\n");
    printf("2. Display Users (Read)\n");
    printf("3. Update User (Update)\n");
    printf("4. Delete User (Delete)\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice){
        case 1: createUser(); 
                break;

        case 2: readUsers(); 
                break;

        case 3: updateUser(); 
                break;
                
        case 4: deleteUser(); 
                break;

        case 5: exit(0);
        default: printf("Invalid choice\n");
    }
    }
    return 0;
}

void ensureFileExists(){
    FILE *fp = fopen(FILENAME,"a+");
    if(fp==NULL){
        printf("File error\n");
        exit(1);
    }
    fclose(fp);
}

void createUser(){
    FILE *fp = fopen(FILENAME,"a");
    if(fp==NULL){ printf("Error\n"); return; }


    User u ;

    printf("Enter User ID : ");
    scanf("%d",&u.id);
    printf("Enter Name : ");
    scanf("%s",u.name);    
    printf("Enter Age : ");
    scanf("%d",&u.age);

    fprintf(fp,"%d,%s,%d\n",u.id,u.name,u.age);
    fclose(fp);

    printf("User added!\n");
}

void readUsers(){
    FILE *fp = fopen(FILENAME,"r");
    if(fp==NULL){ 
        printf("Error\n"); 
        return; 
    }

    User u ;
    printf("\n--- User Records ---\n");
    while(fscanf(fp,"%d,%49[^,],%d\n",&u.id,u.name,&u.age)==3)
    {
        printf("ID:%d  Name:%s  Age:%d\n",u.id,u.name,u.age);
    }
    fclose(fp);
}

void updateUser(){
    FILE *fp = fopen(FILENAME,"r");
    FILE *temp = fopen("temp.txt","w");
    if(fp==NULL||temp==NULL){ printf("Error\n"); return; }

    int id ,found=0;
    User u ;
    printf("Enter User ID to update: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d,%49[^,],%d\n",&u.id,u.name,&u.age)==3)
    {
        if(u.id==id){
            found=1;
            printf("Enter new name : ");
            scanf("%s",u.name);
            printf("Enter new age : ");
            scanf("%d",&u.age);
        }
        fprintf(temp,"%d,%s,%d\n",u.id,u.name,u.age);
    }
    fclose(fp); 
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt",FILENAME);

    if(found) printf("Updated!\n");
    else printf("Not found!\n");
}

void deleteUser(){
    FILE *fp = fopen(FILENAME,"r");
    FILE *temp = fopen("temp.txt","w");
    if(fp==NULL||temp==NULL){ 
        printf("Error\n"); 
        return; 
    }

    int id ,found=0;
    User u ;
    printf("Enter User ID to delete: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d,%49[^,],%d\n",&u.id,u.name,&u.age)==3)
    {
        if(u.id==id)
        { 
            found=1; 
            continue; 
        }
        fprintf(temp,"%d,%s,%d\n",u.id,u.name,u.age);
    }
    fclose(fp); fclose(temp);
    remove(FILENAME);
    rename("temp.txt",FILENAME);

    if(found) printf("Deleted!\n");
    else printf("Not found!\n");
}
