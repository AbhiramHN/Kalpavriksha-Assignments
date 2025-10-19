#include <stdio.h>

struct Student
{
    int rollNumber;
    char name[20];
    int mark1, mark2, mark3;
};

char GradeCalculator(const float averageMarks)
{
    char grade;
    if (averageMarks >= 85)
    {
        grade = 'A';
    }
    else if (averageMarks >= 70)
    {
        grade = 'B';
    }
    else if (averageMarks >= 50)
    {
        grade = 'C';
    }
    else if (averageMarks >= 35)
    {
        grade = 'D';
    }
    else
    {
        grade = 'F';
    }
    return grade;
}

const char *GetStar(const char grade)
{
    const char *stars;
    switch (grade)
    {
    case 'A':
        stars = "*****";
        break;
    case 'B':
        stars = "****";
        break;
    case 'C':
        stars = "***";
        break;
    case 'D':
        stars = "**";
        break;
    default:
        stars = NULL;
        break;
    }
    return stars;
}

void PrintRollNumbers(const struct Student students[], const int numberOfStudents, int index)
{
    if (index == numberOfStudents)
    {
        return;
    }
    printf("%d ", students[index].rollNumber);
    PrintRollNumbers(students, numberOfStudents, index + 1);
}

int main()
{
    struct Student students[100];
    int numberOfStudents;

    scanf("%d", &numberOfStudents);

    for (int index = 0; index < numberOfStudents; index++)
    {
        scanf("%d %s %d %d %d", &students[index].rollNumber, students[index].name,
              &students[index].mark1, &students[index].mark2, &students[index].mark3);
    }

    for (int i = 0; i < numberOfStudents - 1; i++)
    {
        for (int j = i + 1; j < numberOfStudents; j++)
        {
            if (students[i].rollNumber > students[j].rollNumber)
            {
                struct Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    for (int index = 0; index < numberOfStudents; index++)
    {
        int totalMarks = students[index].mark1 + students[index].mark2 + students[index].mark3;
        const float averageMarks = totalMarks / 3.0f;
        const char grade = GradeCalculator(averageMarks);
        const char *stars = GetStar(grade);

        printf("\nRoll: %d\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);
        printf("Total: %d\n", totalMarks);
        printf("Average: %.2f\n", averageMarks);
        printf("Grade: %c\n", grade);

        if (stars != NULL)
        {
            printf("Performance: %s\n", stars);
        }
    }

    printf("\nList of Roll Numbers (via recursion): ");
    PrintRollNumbers(students, numberOfStudents, 0);
    printf("\n");

    return 0;
}
