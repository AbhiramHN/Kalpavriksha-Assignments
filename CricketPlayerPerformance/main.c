#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

typedef enum
{
    AFGHANISTAN = 1,
    AUSTRALIA,
    BANGLADESH,
    ENGLAND,
    INDIA,
    NEW_ZEALAND,
    PAKISTAN,
    SOUTH_AFRICA,
    SRI_LANKA,
    WEST_INDIES
} TeamID;

void removeNewline(char *text)
{
    size_t length = strlen(text);
    if (length > 0 && text[length - 1] == '\n')
    {
        text[length - 1] = '\0';
    }
}

TeamID getTeamId(const char* teamName)
{
    if (strcmp(teamName, "Afghanistan") == 0)
    {
        return AFGHANISTAN;
    }
    if (strcmp(teamName, "Australia") == 0)
    {
        return AUSTRALIA;
    }
    if (strcmp(teamName, "Bangladesh") == 0)
    {
        return BANGLADESH;
    }
    if (strcmp(teamName, "England") == 0)
    {
        return ENGLAND;
    }
    if (strcmp(teamName, "India") == 0)
    {
        return INDIA;
    }
    if (strcmp(teamName, "New Zealand") == 0)
    {
        return NEW_ZEALAND;
    }
    if (strcmp(teamName, "Pakistan") == 0)
    {
        return PAKISTAN;
    }
    if (strcmp(teamName, "South Africa") == 0)
    {
        return SOUTH_AFRICA;
    }
    if (strcmp(teamName, "Sri Lanka") == 0)
    {
        return SRI_LANKA;
    }
    if (strcmp(teamName, "West Indies") == 0)
    {
        return WEST_INDIES;
    }
    return -1;
}

typedef struct PlayerNode
{
    int playerId;
    char name[50];
    char teamName[50];
    char role[30];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    TeamID teamId;
    float performanceIndex;
    struct PlayerNode* next;
    struct PlayerNode* prev;
} PlayerNode;

float calculatePerformanceIndex(PlayerNode *p)
{
    if (strcmp(p->role, "Batsman") == 0)
    {
        return (p->battingAverage * p->strikeRate) / 100.0f;
    }
    if (strcmp(p->role, "Bowler") == 0)
    {
        return (p->wickets * 2) + (100.0f - p->economyRate);
    }
    if (strcmp(p->role, "All-rounder") == 0)
    {
        return ((p->battingAverage * p->strikeRate) / 100.0f) + (p->wickets * 2);
    }
    return 0.0f;
}

PlayerNode* convertToNode(const Player data)
{
    PlayerNode* newNode = (PlayerNode*)malloc(sizeof(PlayerNode));

    newNode->playerId = data.id;
    strcpy(newNode->name, data.name);
    strcpy(newNode->teamName, data.team);
    strcpy(newNode->role, data.role);
    newNode->totalRuns = data.totalRuns;
    newNode->battingAverage = data.battingAverage;
    newNode->strikeRate = data.strikeRate;
    newNode->wickets = data.wickets;
    newNode->economyRate = data.economyRate;
    newNode->teamId = getTeamId(data.team);
    newNode->performanceIndex = calculatePerformanceIndex(newNode);
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void insertPlayer(PlayerNode** head, const Player data)
{
    PlayerNode* newNode = convertToNode(data);

    if (*head == NULL)
    {
        *head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
        return;
    }

    PlayerNode* lastNode = (*head)->prev;

    newNode->next = *head;
    newNode->prev = lastNode;
    lastNode->next = newNode;
    (*head)->prev = newNode;
}

void safeReadString(char* buffer, int size)
{
    fgets(buffer, size, stdin);
    removeNewline(buffer);
}

PlayerNode* createNewPlayerNode()
{
    PlayerNode *newNode = (PlayerNode*)malloc(sizeof(PlayerNode));

    printf("\nEnter Player ID: ");
    scanf("%d", &newNode->playerId);
    getchar();

    printf("Enter Name: ");
    safeReadString(newNode->name, sizeof(newNode->name));

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int roleChoice;
    scanf("%d", &roleChoice);
    getchar();

    if (roleChoice == 1)
    {
        strcpy(newNode->role, "Batsman");
    }
    else if (roleChoice == 2)
    {
        strcpy(newNode->role, "Bowler");
    }
    else
    {
        strcpy(newNode->role, "All-rounder");
    }

    printf("Enter Total Runs: ");
    scanf("%d", &newNode->totalRuns);

    printf("Enter Batting Average: ");
    scanf("%f", &newNode->battingAverage);

    printf("Enter Strike Rate: ");
    scanf("%f", &newNode->strikeRate);

    printf("Enter Wickets: ");
    scanf("%d", &newNode->wickets);

    printf("Enter Economy Rate: ");
    scanf("%f", &newNode->economyRate);

    printf("Enter Team ID (1-10): ");
    int teamId;
    scanf("%d", &teamId);

    newNode->teamId = teamId;
    strcpy(newNode->teamName, teams[teamId - 1]);

    newNode->performanceIndex = calculatePerformanceIndex(newNode);
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void addNewPlayer(PlayerNode **head)
{
    PlayerNode *newNode = createNewPlayerNode();

    if (*head == NULL)
    {
        *head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
        return;
    }

    PlayerNode *lastNode = (*head)->prev;

    newNode->next = *head;
    newNode->prev = lastNode;
    lastNode->next = newNode;
    (*head)->prev = newNode;
}

void displayTeamPlayers(PlayerNode *head)
{
    if (head == NULL)
    {
        return;
    }

    int selectedTeamId;
    printf("Enter Team ID: ");
    scanf("%d", &selectedTeamId);

    printf("\nPlayers of Team %s:\n", teams[selectedTeamId - 1]);
    printf("============================================================\n");
    printf("ID  Name\tRole\tRuns Avg SR  Wkts ER  Perf.Index\n");
    printf("============================================================\n");

    PlayerNode *currentNode = head;
    int totalPlayersCount = 0;
    float strikeRateSum = 0.0;
    int battersAndARCount = 0;

    do
    {
        if (currentNode->teamId == selectedTeamId)
        {
            printf("%d  %s\t%s\t%d %.2f %.2f %d %.2f %.2f\n",
                currentNode->playerId,
                currentNode->name,
                currentNode->role,
                currentNode->totalRuns,
                currentNode->battingAverage,
                currentNode->strikeRate,
                currentNode->wickets,
                currentNode->economyRate,
                currentNode->performanceIndex
            );

            totalPlayersCount++;

            if (strcmp(currentNode->role, "Batsman") == 0 || strcmp(currentNode->role, "All-rounder") == 0)
            {
                strikeRateSum += currentNode->strikeRate;
                battersAndARCount++;
            }
        }

        currentNode = currentNode->next;

    } while (currentNode != head);

    printf("============================================================\n");

    if (battersAndARCount > 0)
    {
        printf("Average Batting Strike Rate: %.2f\n", strikeRateSum / battersAndARCount);
    }
    else
    {
        printf("Average Batting Strike Rate: 0.00\n");
    }
}

void sortTeamsByAverageSR(float averageSR[], int order[])
{
    for (int teamIndex = 0; teamIndex < 10; teamIndex++)
    {
        for (int compareIndex = teamIndex + 1; compareIndex < 10; compareIndex++)
        {
            if (averageSR[order[compareIndex]] > averageSR[order[teamIndex]])
            {
                int temp = order[teamIndex];
                order[teamIndex] = order[compareIndex];
                order[compareIndex] = temp;
            }
        }
    }
}

void displayTeamsByStrikeRate(PlayerNode* head)
{
    float strikeRateTotals[10] = {0};
    int batterCounts[10] = {0};
    int playerCounts[10] = {0};
    float averageStrikeRates[10] = {0};

    if (head == NULL)
    {
        return;
    }

    PlayerNode* currentNode = head;

    do
    {
        int index = currentNode->teamId - 1;

        if (strcmp(currentNode->role, "All-rounder") == 0 || strcmp(currentNode->role, "Batsman") == 0)
        {
            strikeRateTotals[index] += currentNode->strikeRate;
            batterCounts[index] += 1;
        }

        playerCounts[index] += 1;

        currentNode = currentNode->next;

    } while (currentNode != head);

    for (int teamIndex = 0; teamIndex < 10; teamIndex++)
    {
        if (batterCounts[teamIndex] > 0)
        {
            averageStrikeRates[teamIndex] = strikeRateTotals[teamIndex] / batterCounts[teamIndex];
        }
        else
        {
            averageStrikeRates[teamIndex] = 0;
        }
    }

    int order[10];
    for (int teamIndex = 0; teamIndex < 10; teamIndex++)
    {
        order[teamIndex] = teamIndex;
    }

    sortTeamsByAverageSR(averageStrikeRates, order);

    printf("\nID\tTeam Name\tAvg Bat SR\tTotal Players\n");
    printf("============================================================\n");

    for (int position = 0; position < 10; position++)
    {
        int sortedIndex = order[position];

        printf("%d\t%s\t %.1f\t%d\n",
            sortedIndex + 1,
            teams[sortedIndex],
            averageStrikeRates[sortedIndex],
            playerCounts[sortedIndex]);
    }
}

void displayTopKPlayers(PlayerNode* head)
{
    if (head == NULL)
    {
        return;
    }

    int teamId;
    int K;
    char selectedRole[30];

    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    getchar();

    printf("Enter Role (Batsman / Bowler / All-rounder): ");
    safeReadString(selectedRole, sizeof(selectedRole));

    printf("Enter K value: ");
    scanf("%d", &K);

    PlayerNode* roleMatchedPlayers[200];
    int matchedCount = 0;

    PlayerNode* currentNode = head;

    do
    {
        if (currentNode->teamId == teamId && strcmp(currentNode->role, selectedRole) == 0)
        {
            roleMatchedPlayers[matchedCount] = currentNode;
            matchedCount++;
        }

        currentNode = currentNode->next;

    } while (currentNode != head);

    for (int primaryIndex = 0; primaryIndex < matchedCount; primaryIndex++)
    {
        for (int secondaryIndex = primaryIndex + 1; secondaryIndex < matchedCount; secondaryIndex++)
        {
            if (roleMatchedPlayers[secondaryIndex]->performanceIndex >
                roleMatchedPlayers[primaryIndex]->performanceIndex)
            {
                PlayerNode* temp = roleMatchedPlayers[primaryIndex];
                roleMatchedPlayers[primaryIndex] = roleMatchedPlayers[secondaryIndex];
                roleMatchedPlayers[secondaryIndex] = temp;
            }
        }
    }

    if (K > matchedCount)
    {
        K = matchedCount;
    }

    printf("\nTop %d %s of Team %s:\n", K, selectedRole, teams[teamId - 1]);
    printf("============================================================\n");
    printf("ID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n");
    printf("============================================================\n");

    for (int index = 0; index < K; index++)
    {
        PlayerNode* p = roleMatchedPlayers[index];

        printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
            p->playerId,
            p->name,
            p->role,
            p->totalRuns,
            p->battingAverage,
            p->strikeRate,
            p->wickets,
            p->economyRate,
            p->performanceIndex
        );
    }
}

void displayRoleAcrossTeams(PlayerNode* head)
{
    if (head == NULL)
    {
        return;
    }

    char roleInput[30];
    printf("Enter Role (Batsman / Bowler / All-rounder): ");
    getchar();
    safeReadString(roleInput, sizeof(roleInput));

    PlayerNode* roleMatches[300];
    int roleMatchesCount = 0;

    PlayerNode* currentNode = head;

    do
    {
        if (strcmp(currentNode->role, roleInput) == 0)
        {
            roleMatches[roleMatchesCount] = currentNode;
            roleMatchesCount++;
        }

        currentNode = currentNode->next;

    } while (currentNode != head);

    for (int mainIndex = 0; mainIndex < roleMatchesCount; mainIndex++)
    {
        for (int subIndex = mainIndex + 1; subIndex < roleMatchesCount; subIndex++)
        {
            if (roleMatches[subIndex]->performanceIndex >
                roleMatches[mainIndex]->performanceIndex)
            {
                PlayerNode* temp = roleMatches[mainIndex];
                roleMatches[mainIndex] = roleMatches[subIndex];
                roleMatches[subIndex] = temp;
            }
        }
    }

    printf("\nPlayers of Role: %s (Descending by Performance Index)\n", roleInput);
    printf("==========================================================================\n");
    printf("ID   Name\tTeam\tRole\tRuns Avg SR Wkts ER Perf.Index\n");
    printf("==========================================================================\n");

    for (int index = 0; index < roleMatchesCount; index++)
    {
        PlayerNode* p = roleMatches[index];

        printf("%d  %s\t%s\t%s\t%d %.2f %.2f %d %.2f %.2f\n",
            p->playerId,
            p->name,
            teams[p->teamId - 1],
            p->role,
            p->totalRuns,
            p->battingAverage,
            p->strikeRate,
            p->wickets,
            p->economyRate,
            p->performanceIndex
        );
    }
}

void showMenu(PlayerNode **head)
{
    int choice;
    int running = 1;

    while (running)
    {
        printf("\n=============================================\n");
        printf("      ICC ODI Player Performance Analyzer\n");
        printf("=============================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Team (by Role)\n");
        printf("5. Display All Players of a Specific Role\n");
        printf("6. Exit\n");
        printf("=============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                addNewPlayer(head);
                break;
            }

            case 2:
            {
                displayTeamPlayers(*head);
                break;
            }

            case 3:
            {
                displayTeamsByStrikeRate(*head);
                break;
            }

            case 4:
            {
                displayTopKPlayers(*head);
                break;
            }

            case 5:
            {
                displayRoleAcrossTeams(*head);
                break;
            }

            case 6:
            {
                running = 0;
                break;
            }

            default:
            {
                printf("Invalid choice! Try again.\n");
            }
        }
    }
}

int main()
{
    PlayerNode* head = NULL;

    for (int index = 0; index < playerCount; index++)
    {
        insertPlayer(&head, players[index]);
    }

    showMenu(&head);

    return 0;
}
