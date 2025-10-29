#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

void printMatrix(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in printMatrix.\n");
    }
    else
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                printf("%4d ", *(matrix + row * size + column));
            }
            printf("\n");
        }
    }
}

void transposeMatrix(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in transposeMatrix.\n");
    }
    else
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = row + 1; column < size; column++)
            {
                int temp = *(matrix + row * size + column);
                *(matrix + row * size + column) = *(matrix + column * size + row);
                *(matrix + column * size + row) = temp;
            }
        }
    }
}

void reverseRows(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in reverseRows.\n");
    }
    else
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size / 2; column++)
            {
                int temp = *(matrix + row * size + column);
                *(matrix + row * size + column) = *(matrix + row * size + (size - 1 - column));
                *(matrix + row * size + (size - 1 - column)) = temp;
            }
        }
    }
}

void rotateMatrix(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in rotateMatrix.\n");
    }
    else
    {
        transposeMatrix(matrix, size);
        reverseRows(matrix, size);
    }
}

void smoothCell(int *matrix, const int size, int row, int column)
{
    int sum = 0;
    int count = 0;

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            int r = row + dr;
            int c = column + dc;

            if (r >= 0 && r < size && c >= 0 && c < size)
            {
                sum += *(matrix + r * size + c) & 0xFF;
                count++;
            }
        }
    }

    int newValue = sum / count;
    *(matrix + row * size + column) = *(matrix + row * size + column) | (newValue << 8);
}

void applySmoothing(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in applySmoothing.\n");
    }
    else
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                smoothCell(matrix, size, row, column);
            }
        }

        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                *(matrix + row * size + column) = (*(matrix + row * size + column) >> 8) & 0xFF;
            }
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
        matrixSize = -1;
    }

    return matrixSize;
}


void fillRandomMatrix(int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in fillRandomMatrix.\n");
    }
    else
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                *(matrix + row * size + column) = rand() % 256;
            }
        }
    }
}

void displayMatrix(const char *title, int *matrix, const int size)
{
    if (!matrix)
    {
        printf("Error: Null matrix pointer in displayMatrix.\n");
    }
    else
    {
        printf("\n%s\n", title);
        printMatrix(matrix, size);
    }
}

int main()
{
    int matrixSize;

    srand(time(NULL));
    matrixSize = getMatrixSize();
    if (matrixSize == -1)
    {
        return 1;
    }

    int *matrix = malloc(matrixSize * matrixSize * sizeof(int));

    if (!matrix)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    fillRandomMatrix(matrix, matrixSize);
    displayMatrix("Original Randomly Generated Matrix:", matrix, matrixSize);

    rotateMatrix(matrix, matrixSize);
    displayMatrix("Matrix after 90° Clockwise Rotation:", matrix, matrixSize);

    applySmoothing(matrix, matrixSize);
    displayMatrix("Matrix after Applying 3x3 Smoothing Filter:", matrix, matrixSize);

    free(matrix);
    return 0;
}
