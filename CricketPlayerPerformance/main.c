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

void removeNewline(char *text)
{
    int l = strlen(text);
    if (text[l - 1] == '\n') text[l - 1] = '\0';
}

float calculatePerformanceIndex(PlayerNode *p)
{
    if (strcmp(p->role, "Batsman") == 0)
        return (p->battingAverage * p->strikeRate) / 100.0f;
    if (strcmp(p->role, "Bowler") == 0)
        return (p->wickets * 2) + (100.0f - p->economyRate);
    return ((p->battingAverage * p->strikeRate) / 100.0f) + (p->wickets * 2);
}

TeamID getTeamId(const char* name)
{
    for (int i = 0; i < 10; i++)
        if (strcmp(name, teams[i]) == 0)
            return (TeamID)(i + 1);
    return -1;
}

PlayerNode* convertToNode(const Player data)
{
    PlayerNode* n = malloc(sizeof(PlayerNode));
    n->playerId = data.id;

    strncpy(n->name, data.name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';

    strncpy(n->teamName, data.team, sizeof(n->teamName) - 1);
    n->teamName[sizeof(n->teamName) - 1] = '\0';

    strncpy(n->role, data.role, sizeof(n->role) - 1);
    n->role[sizeof(n->role) - 1] = '\0';
    n->totalRuns = data.totalRuns;
    n->battingAverage = data.battingAverage;
    n->strikeRate = data.strikeRate;
    n->wickets = data.wickets;
    n->economyRate = data.economyRate;
    n->teamId = getTeamId(data.team);
    n->performanceIndex = calculatePerformanceIndex(n);
    n->next = n->prev = NULL;
    return n;
}

void insertPlayer(PlayerNode** head, const Player data)
{
    PlayerNode* n = convertToNode(data);

    if (*head == NULL)
    {
        *head = n;
        n->next = n->prev = n;
        return;
    }

    PlayerNode* last = (*head)->prev;
    n->next = *head;
    n->prev = last;
    last->next = n;
    (*head)->prev = n;
}

void safeReadString(char* buf, int size)
{
    if (fgets(buf, size, stdin) != NULL)
    {
        removeNewline(buf);
    }
}

PlayerNode* createNewPlayerNode()
{
    PlayerNode *n = malloc(sizeof(PlayerNode));

    printf("\nEnter Player ID: ");
    scanf("%d", &n->playerId);
    getchar();

    printf("Enter Name: ");
    safeReadString(n->name, sizeof(n->name));

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int r;
    scanf("%d", &r);
    getchar();

    if (r == 1) strcpy(n->role, "Batsman");
    else if (r == 2) strcpy(n->role, "Bowler");
    else strcpy(n->role, "All-rounder");

    printf("Enter Total Runs: ");
    scanf("%d", &n->totalRuns);

    printf("Enter Batting Average: ");
    scanf("%f", &n->battingAverage);

    printf("Enter Strike Rate: ");
    scanf("%f", &n->strikeRate);

    printf("Enter Wickets: ");
    scanf("%d", &n->wickets);

    printf("Enter Economy Rate: ");
    scanf("%f", &n->economyRate);

    printf("Enter Team ID (1-10): ");
    int tid;
    scanf("%d", &tid);

    n->teamId = tid;
    strcpy(n->teamName, teams[tid - 1]);

    n->performanceIndex = calculatePerformanceIndex(n);
    n->next = n->prev = NULL;

    return n;
}

void addNewPlayer(PlayerNode **head)
{
    PlayerNode *n = createNewPlayerNode();

    if (*head == NULL)
    {
        *head = n;
        n->next = n->prev = n;
        return;
    }

    PlayerNode *last = (*head)->prev;
    n->next = *head;
    n->prev = last;
    last->next = n;
    (*head)->prev = n;
}

void displayTeamPlayers(PlayerNode *head)
{
    if (head == NULL) return;

    int tid;
    printf("Enter Team ID: ");
    scanf("%d", &tid);

    printf("\nPlayers of Team %s:\n", teams[tid - 1]);
    printf("================================================================================================================\n");
    printf("%-4s %-20s %-13s %6s %6s %6s %6s %6s %12s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("================================================================================================================\n");

    PlayerNode *c = head;
    int total = 0;
    float srSum = 0;
    int cnt = 0;

    do
    {
        if (c->teamId == tid)
        {
            printf("%-4d %-20s %-13s %6d %6.2f %6.2f %6d %6.2f %12.2f\n",
                   c->playerId, c->name, c->role, c->totalRuns,
                   c->battingAverage, c->strikeRate, c->wickets,
                   c->economyRate, c->performanceIndex);

            total++;

            if (strcmp(c->role, "Batsman") == 0 || strcmp(c->role, "All-rounder") == 0)
            {
                srSum += c->strikeRate;
                cnt++;
            }
        }
        c = c->next;

    } while (c != head);

    printf("================================================================================================================\n");
    printf("Total Players: %d\n", total);
    printf("Average Batting Strike Rate: %.2f\n", (cnt ? srSum / cnt : 0.0));
}

void sortTeams(float a[], int o[])
{
    for (int i = 0; i < 10; i++)
        for (int j = i + 1; j < 10; j++)
            if (a[o[j]] > a[o[i]])
            {
                int t = o[i];
                o[i] = o[j];
                o[j] = t;
            }
}

void displayTeamsByStrikeRate(PlayerNode* head)
{
    float sr[10] = {0};
    int c[10] = {0};
    int p[10] = {0};

    PlayerNode* cur = head;
    do
    {
        int idx = cur->teamId - 1;

        if (strcmp(cur->role, "Batsman") == 0 || strcmp(cur->role, "All-rounder") == 0)
        {
            sr[idx] += cur->strikeRate;
            c[idx]++;
        }

        p[idx]++;
        cur = cur->next;

    } while (cur != head);

    float avg[10];
    for (int i = 0; i < 10; i++)
        avg[i] = (c[i] ? sr[i] / c[i] : 0);

    int order[10];
    for (int i = 0; i < 10; i++) order[i] = i;

    sortTeams(avg, order);

    printf("\nID  Team Name           Avg Bat SR  Total Players\n");
    printf("=================================================\n");

    for (int i = 0; i < 10; i++)
    {
        int idx = order[i];
        printf("%-3d %-20s %10.1f %12d\n", idx + 1, teams[idx], avg[idx], p[idx]);
    }
}

void displayTopKPlayers(PlayerNode* head)
{
    int tid, K, r;
    printf("Enter Team ID: ");
    scanf("%d", &tid);

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &r);

    printf("Enter K value: ");
    scanf("%d", &K);

    char role[20];
    if (r == 1) strcpy(role, "Batsman");
    else if (r == 2) strcpy(role, "Bowler");
    else strcpy(role, "All-rounder");

    PlayerNode* list[300];
    int n = 0;

    PlayerNode* c = head;

    do
    {
        if (c->teamId == tid && strcmp(c->role, role) == 0)
        {
            list[n++] = c;
        }
        c = c->next;

    } while (c != head);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (list[j]->performanceIndex > list[i]->performanceIndex)
            {
                PlayerNode* t = list[i];
                list[i] = list[j];
                list[j] = t;
            }

    if (K > n) K = n;

    printf("\nTop %d %s of Team %s:\n", K, role, teams[tid - 1]);
    printf("================================================================================================================\n");
    printf("%-4s %-20s %-13s %6s %6s %6s %6s %6s %12s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("================================================================================================================\n");

    for (int i = 0; i < K; i++)
    {
        PlayerNode* p = list[i];
        printf("%-4d %-20s %-13s %6d %6.2f %6.2f %6d %6.2f %12.2f\n",
               p->playerId, p->name, p->role, p->totalRuns,
               p->battingAverage, p->strikeRate, p->wickets,
               p->economyRate, p->performanceIndex);
    }
}

void displayRoleAcrossTeams(PlayerNode* head)
{
    int r;
    printf("Enter Role (1-Batsman / 2-Bowler / 3-All-rounder): ");
    scanf("%d", &r);

    char role[20];
    if (r == 1) strcpy(role, "Batsman");
    else if (r == 2) strcpy(role, "Bowler");
    else strcpy(role, "All-rounder");

    PlayerNode* arr[500];
    int n = 0;

    PlayerNode* c = head;

    do
    {
        if (strcmp(c->role, role) == 0)
        {
            arr[n++] = c;
        }
        c = c->next;

    } while (c != head);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[j]->performanceIndex > arr[i]->performanceIndex)
            {
                PlayerNode* t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }

    printf("\nPlayers of Role: %s\n", role);
    printf("================================================================================================================\n");
    printf("%-4s %-20s %-15s %-13s %6s %6s %6s %6s %12s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "Perf.Index");
    printf("================================================================================================================\n");

    for (int i = 0; i < n; i++)
    {
        PlayerNode* p = arr[i];
        printf("%-4d %-20s %-15s %-13s %6d %6.2f %6.2f %6d %12.2f\n",
               p->playerId, p->name, teams[p->teamId - 1], p->role,
               p->totalRuns, p->battingAverage, p->strikeRate, p->wickets,
               p->performanceIndex);
    }
}

void showMenu(PlayerNode **head)
{
    int ch;

    while (1)
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

        scanf("%d", &ch);

        if (ch == 1) addNewPlayer(head);
        else if (ch == 2) displayTeamPlayers(*head);
        else if (ch == 3) displayTeamsByStrikeRate(*head);
        else if (ch == 4) displayTopKPlayers(*head);
        else if (ch == 5) displayRoleAcrossTeams(*head);
        else if (ch == 6) break;
    }
}

int main()
{
    PlayerNode* head = NULL;

    for (int i = 0; i < playerCount; i++)
        insertPlayer(&head, players[i]);

    showMenu(&head);

    return 0;
}
