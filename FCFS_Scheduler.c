#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME 32
#define HASH_SIZE 101

typedef struct
{
    int pid;
    char processName[MAX_NAME];

    int burstTime;
    int remainingBurst;

    int ioStartTime;
    int ioDuration;
    int remainingIo;
    int ioStarted;

    int arrivalTime;
    int completionTime;

    int waitingTime;
    int turnaroundTime;

    int isKilled;

} PCB;

typedef struct Node
{
    PCB *process;
    struct Node *next;
} Node;

typedef struct
{
    Node *front;
    Node *rear;
} Queue;

void initQueue(Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

int isQueueEmpty(Queue *q)
{
    return q->front == NULL;
}

void enqueue(Queue *q, PCB *p)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->process = p;
    temp->next = NULL;

    if (q->rear == NULL)
    {
        q->front = temp;
        q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

PCB *dequeue(Queue *q)
{
    if (isQueueEmpty(q))
    {
        return NULL;
    }

    Node *temp = q->front;
    PCB *p = temp->process;

    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);
    return p;
}

void removeFromQueue(Queue *q, PCB *target)
{
    Node *prev = NULL, *cur = q->front;

    while (cur != NULL)
    {
        if (cur->process == target)
        {
            if (prev == NULL)
                q->front = cur->next;
            else
                prev->next = cur->next;

            if (cur == q->rear)
                q->rear = prev;

            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

typedef struct HashNode
{
    int key;
    PCB *value;
    struct HashNode *next;
} HashNode;

typedef struct
{
    HashNode *table[HASH_SIZE];
} HashMap;

int hash(int key)
{
    return key % HASH_SIZE;
}

void initHashMap(HashMap *map)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        map->table[i] = NULL;
    }
}

void insertProcess(HashMap *map, PCB *p)
{
    int h = hash(p->pid);

    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = p->pid;
    newNode->value = p;
    newNode->next = map->table[h];

    map->table[h] = newNode;
}

PCB *findProcess(HashMap *map, int pid)
{
    int h = hash(pid);
    HashNode *cur = map->table[h];

    while (cur != NULL)
    {
        if (cur->key == pid)
        {
            return cur->value;
        }
        cur = cur->next;
    }
    return NULL;
}

void runScheduler(Queue *readyQueue, Queue *waitingQueue, Queue *terminatedQueue, HashMap *map, int killPid[], int killTime[], int killCount)
{
    int clock = 0;
    PCB *running = NULL;

    while (!isQueueEmpty(readyQueue) || !isQueueEmpty(waitingQueue) || running != NULL)
    {
        for (int i = 0; i < killCount; i++)
        {
            if (killTime[i] == clock)
            {
                PCB *p = findProcess(map, killPid[i]);
                if (p != NULL && !p->isKilled)
                {
                    p->isKilled = 1;
                    p->completionTime = clock;

                    if (running == p)
                        running = NULL;

                    removeFromQueue(readyQueue, p);
                    removeFromQueue(waitingQueue, p);

                    enqueue(terminatedQueue, p);
                }
            }
        }

        if (running == NULL)
        {
            running = dequeue(readyQueue);
        }

        if (running != NULL && !running->isKilled && running->remainingBurst > 0)
        {
            running->remainingBurst--;

            int executed = running->burstTime - running->remainingBurst;

            if (!running->ioStarted && executed == running->ioStartTime)
            {
                running->ioStarted = 1;
                running->remainingIo = running->ioDuration;
            }
        }

        Queue tempQueue;
        initQueue(&tempQueue);

        while (!isQueueEmpty(waitingQueue))
        {
            PCB *p = dequeue(waitingQueue);
            p->remainingIo--;

            if (p->remainingIo == 0)
            {
                enqueue(readyQueue, p);
            }
            else
            {
                enqueue(&tempQueue, p);
            }
        }

        waitingQueue->front = tempQueue.front;
        waitingQueue->rear = tempQueue.rear;

        if (running != NULL && running->remainingBurst == 0 && !running->isKilled)
        {
            running->completionTime = clock + 1;
            enqueue(terminatedQueue, running);
            running = NULL;
        }
        else if (running != NULL && running->ioStarted)
        {
            enqueue(waitingQueue, running);
            running = NULL;
        }

        sleep(1);
        clock++;
    }
}

void printReport(Queue *terminatedQueue)
{
    printf("\nPID\tName\tBurst\tIO\tTAT\tWait\n");

    while (!isQueueEmpty(terminatedQueue))
    {
        PCB *p = dequeue(terminatedQueue);

        p->turnaroundTime = p->completionTime - p->arrivalTime;
        p->waitingTime = p->turnaroundTime - p->burstTime - p->ioDuration;

        printf("%d\t%s\t%d\t%d\t%d\t%d\n",
               p->pid,
               p->processName,
               p->burstTime,
               p->ioDuration,
               p->turnaroundTime,
               p->waitingTime);
    }
}

int main()
{
    Queue readyQueue, waitingQueue, terminatedQueue;
    HashMap map;

    initQueue(&readyQueue);
    initQueue(&waitingQueue);
    initQueue(&terminatedQueue);
    initHashMap(&map);

    int processCount;
    scanf("%d", &processCount);

    for (int i = 0; i < processCount; i++)
    {
        PCB *p = (PCB *)malloc(sizeof(PCB));

        scanf("%s %d %d %d %d",
              p->processName,
              &p->pid,
              &p->burstTime,
              &p->ioStartTime,
              &p->ioDuration);

        p->remainingBurst = p->burstTime;
        p->ioStarted = 0;
        p->remainingIo = 0;
        p->arrivalTime = 0;
        p->isKilled = 0;

        insertProcess(&map, p);
        enqueue(&readyQueue, p);
    }

    int killCount;
    scanf("%d", &killCount);

    int killPid[killCount], killTime[killCount];
    for (int i = 0; i < killCount; i++)
    {
        scanf("%d %d", &killPid[i], &killTime[i]);
    }

    runScheduler(&readyQueue, &waitingQueue, &terminatedQueue, &map, killPid, killTime, killCount);

    printReport(&terminatedQueue);

    return 0;
}
