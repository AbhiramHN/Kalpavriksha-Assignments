#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

void printMatrix(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in printMatrix.\n");
        return;
    }
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            printf("%4d ", *(*(mat + row) + column));
        }
        printf("\n");
    }
}

void transposeMatrix(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in transposeMatrix.\n");
        return;
    }
    int temp;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            temp = *(*(mat + i) + j);
            *(*(mat + i) + j) = *(*(mat + j) + i);
            *(*(mat + j) + i) = temp;
        }
    }
}

void reverseRows(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in reverseRows.\n");
        return;
    }
    int temp;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size / 2; j++)
        {
            temp = *(*(mat + i) + j);
            *(*(mat + i) + j) = *(*(mat + i) + (size - 1 - j));
            *(*(mat + i) + (size - 1 - j)) = temp;
        }
    }
}

void rotateMatrix(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in rotateMatrix.\n");
        return;
    }
    transposeMatrix(mat, size);
    reverseRows(mat, size);
}

void smoothCell(int (*mat)[MAX_SIZE], int size, int row, int column)
{
    int sum = 0, count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int r = row + i;
            int c = column + j;
            if (r >= 0 && r < size && c >= 0 && c < size)
            {
                sum += *(*(mat + r) + c) & 0xFF;
                count++;
            }
        }
    }
    int newValue = sum / count;
    *(*(mat + row) + column) = *(*(mat + row) + column) | (newValue << 8);
}

void applySmoothing(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in applySmoothing.\n");
        return;
    }
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            smoothCell(mat, size, row, column);
        }
    }
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            *(*(mat + row) + column) = (*(*(mat + row) + column) >> 8) & 0xFF;
        }
    }
}

int getMatrixSize()
{
    int matrixSize;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &matrixSize);
    if (matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid size! Please enter a value between 2 and 10.\n");
        return -1; 
    }
    return matrixSize;
}

void fillRandomMatrix(int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in fillRandomMatrix.\n");
        return;
    }
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            *(*(mat + row) + column) = rand() % 256;
        }
    }
}

void displayMatrix(const char *title, int (*mat)[MAX_SIZE], int size)
{
    if (!mat)
    {
        printf("Error: Null matrix pointer in displayMatrix.\n");
        return;
    }
    printf("\n%s\n", title);
    printMatrix(mat, size);
}

int main()
{
    int matrixSize;
    int matrix[MAX_SIZE][MAX_SIZE];

    srand(time(NULL));

    matrixSize = getMatrixSize();
    if (matrixSize == -1)
        return 1; 

    fillRandomMatrix(matrix, matrixSize);

    displayMatrix("Original Randomly Generated Matrix:", matrix, matrixSize);

    rotateMatrix(matrix, matrixSize);

    displayMatrix("Matrix after 90° Clockwise Rotation:", matrix, matrixSize);

    applySmoothing(matrix, matrixSize);

    displayMatrix("Matrix after Applying 3x3 Smoothing Filter:", matrix, matrixSize);

    return 0;
}
