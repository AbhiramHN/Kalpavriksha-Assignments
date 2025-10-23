#include <stdio.h>

typedef struct
{
    int rollNumber;
    char name[20];
    int mark1, mark2, mark3;
} Student;

char gradeCalculator(const float averageMarks)
{
    char grade;
    if (averageMarks >= 85)
        grade = 'A';
    else if (averageMarks >= 70)
        grade = 'B';
    else if (averageMarks >= 50)
        grade = 'C';
    else if (averageMarks >= 35)
        grade = 'D';
    else
        grade = 'F';
    return grade;
}

void printStars(const char grade)
{
    int stars;
    switch (grade)
    {
    case 'A': stars = 5; break;
    case 'B': stars = 4; break;
    case 'C': stars = 3; break;
    case 'D': stars = 2; break;
    default: stars = 0; break;
    }

    printf("Performance: ");
    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
}

void readStudents(Student students[], int *numberOfStudents)
{
    printf("Enter number of students: ");
    scanf("%d", numberOfStudents);

    printf("Enter details (RollNumber Name Mark1 Mark2 Mark3):\n");
    for (int i = 0; i < *numberOfStudents; i++)
    {
        scanf("%d %s %d %d %d",
              &students[i].rollNumber,
              students[i].name,
              &students[i].mark1,
              &students[i].mark2,
              &students[i].mark3);
    }
}

void sortStudents(Student students[], const int numberOfStudents)
{
    for (int i = 0; i < numberOfStudents - 1; i++)
    {
        for (int j = i + 1; j < numberOfStudents; j++)
        {
            if (students[i].rollNumber > students[j].rollNumber)
            {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void calculateResults(const Student students[], const int numberOfStudents)
{
    for (int i = 0; i < numberOfStudents; i++)
    {
        int totalMarks = students[i].mark1 + students[i].mark2 + students[i].mark3;
        float averageMarks = totalMarks / 3.0f;
        char grade = gradeCalculator(averageMarks);

        printf("\nRoll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", totalMarks);
        printf("Average: %.2f\n", averageMarks);
        printf("Grade: %c \n", grade);
        if (averageMarks < 35)
        {
            continue;
        }
        printStars(grade);
        printf("\n");
    }
}

void printRollNumbers(const Student students[], const int numberOfStudents, int index)
{
    if (index == numberOfStudents)
    {        
        return;
    }
    printf("%d ", students[index].rollNumber);
    printRollNumbers(students, numberOfStudents, index + 1);
}

int main()
{
    Student students[100];
    int numberOfStudents;

    readStudents(students, &numberOfStudents);
    sortStudents(students, numberOfStudents);
    calculateResults(students, numberOfStudents);

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbers(students, numberOfStudents, 0);
    printf("\n");

    return 0;
}
