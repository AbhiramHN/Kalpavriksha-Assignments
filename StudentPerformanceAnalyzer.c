#include <stdio.h>

struct student
{
    int rollNumber;
    char name[20];
    int mark1, mark2, mark3;
};

char GradeCalculator(float averageMarks)
{
    if (averageMarks >= 85)
    {
        return 'A';
    }
    else if (averageMarks >= 70)
    {
        return 'B';
    }
    else if (averageMarks >= 50)
    {
        return 'C';
    }
    else if (averageMarks >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

const char *getStar(char grade)
{
    if (grade == 'A')
    {
        return "*****";
    }
    else if (grade == 'B')
    {
        return "****";
    }
    else if (grade == 'C')
    {
        return "***";
    }
    else if (grade == 'D')
    {
        return "**";
    }
    else
    {
        return NULL;
    }
}

void printRollNumbers(struct student s[], int n, int index)
{
    if (index == n)
    {
        return;
    }
    printf("%d ", s[index].rollNumber);
    printRollNumbers(s, n, index + 1);
}

int main()
{
    struct student Student[100];
    int numberOfStudents;

    scanf("%d", &numberOfStudents);

    for (int index = 0; index < numberOfStudents; index++)
    {
        scanf("%d %s %d %d %d", &Student[index].rollNumber, Student[index].name, &Student[index].mark1, &Student[index].mark2, &Student[index].mark3);
    }

    
    for (int i = 0; i < numberOfStudents - 1; i++)
    {
        for (int j = i + 1; j < numberOfStudents; j++)
        {
            if (Student[i].rollNumber > Student[j].rollNumber)
            {
                struct student temp = Student[i];
                Student[i] = Student[j];
                Student[j] = temp;
            }
        }
    }

    for (int index = 0; index < numberOfStudents; index++)
    {
        int totalMarks = Student[index].mark1 + Student[index].mark2 + Student[index].mark3;
        float averageMarks = totalMarks / 3.0;
        char grade = GradeCalculator(averageMarks);

        printf("\nRoll: %d\n", Student[index].rollNumber);
        printf("Name: %s\n", Student[index].name);
        printf("Total: %d\n", totalMarks);
        printf("Average: %.2f\n", averageMarks);
        printf("Grade: %c\n", grade);

        const char *stars = getStar(grade);
        if (stars != NULL)
        {
            printf("Performance: %s\n", stars);
        }
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbers(Student, numberOfStudents, 0);
    printf("\n");

    return 0;
}
