#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1000

typedef struct Node
{
    int key;
    char *value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct HashEntry
{
    int key;
    Node *node;
    int isUsed;
} HashEntry;

typedef struct LRUCache
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    HashEntry table[HASH_SIZE];
} LRUCache;

LRUCache *cacheInstance = NULL;

unsigned long hashFunction(int key)
{
    if (key < 0)
    {
        key = -key;
    }
    return (unsigned long)key % HASH_SIZE;
}

Node *createNode(int key, const char *valueText)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = malloc(strlen(valueText) + 1);
    strcpy(newNode->value, valueText);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

LRUCache *createCache(int capacity)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int slot = 0; slot < HASH_SIZE; slot++)
    {
        cache->table[slot].isUsed = 0;
        cache->table[slot].key = 0;
        cache->table[slot].node = NULL;
    }

    return cache;
}

void addToFront(LRUCache *cache, Node *node)
{
    if (cache->head == NULL)
    {
        cache->head = node;
        cache->tail = node;
        return;
    }

    node->next = cache->head;
    cache->head->prev = node;
    cache->head = node;
}

void removeNode(LRUCache *cache, Node *node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        cache->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        cache->tail = node->prev;
    }
}

void moveToFront(LRUCache *cache, Node *node)
{
    if (cache->head == node)
    {
        return;
    }
    removeNode(cache, node);
    addToFront(cache, node);
}

Node *hashGet(LRUCache *cache, int key)
{
    unsigned long index = hashFunction(key);
    unsigned long start = index;

    while (1)
    {
        if (cache->table[index].isUsed == 0)
        {
            return NULL;
        }

        if (cache->table[index].key == key)
        {
            return cache->table[index].node;
        }

        index = (index + 1) % HASH_SIZE;
        if (index == start)
        {
            return NULL;
        }
    }
}

void hashInsert(LRUCache *cache, int key, Node *node)
{
    unsigned long index = hashFunction(key);

    while (cache->table[index].isUsed == 1)
    {
        if (cache->table[index].key == key)
        {
            cache->table[index].node = node;
            return;
        }
        index = (index + 1) % HASH_SIZE;
    }

    cache->table[index].isUsed = 1;
    cache->table[index].key = key;
    cache->table[index].node = node;
}

void removeFromHash(LRUCache *cache, int key)
{
    unsigned long index = hashFunction(key);

    while (cache->table[index].isUsed == 1)
    {
        if (cache->table[index].key == key)
        {
            cache->table[index].isUsed = 0;
            cache->table[index].node = NULL;
            return;
        }
        index = (index + 1) % HASH_SIZE;
    }
}

void evict(LRUCache *cache)
{
    Node *victim = cache->tail;
    removeFromHash(cache, victim->key);
    removeNode(cache, victim);
    free(victim->value);
    free(victim);
    cache->size--;
}

char *cacheGet(int key)
{
    Node *node = hashGet(cacheInstance, key);
    if (node == NULL)
    {
        return NULL;
    }
    moveToFront(cacheInstance, node);
    return node->value;
}

void cachePut(int key, const char *valueText)
{
    Node *existing = hashGet(cacheInstance, key);

    if (existing != NULL)
    {
        free(existing->value);
        existing->value = malloc(strlen(valueText) + 1);
        strcpy(existing->value, valueText);
        moveToFront(cacheInstance, existing);
        return;
    }

    Node *newNode = createNode(key, valueText);
    addToFront(cacheInstance, newNode);
    hashInsert(cacheInstance, key, newNode);
    cacheInstance->size++;

    if (cacheInstance->size > cacheInstance->capacity)
    {
        evict(cacheInstance);
    }
}

void processCommand(const char *command)
{
    if (strcmp(command, "createCache") == 0)
    {
        int cap;
        scanf("%d", &cap);
        cacheInstance = createCache(cap);
    }
    else if (strcmp(command, "put") == 0)
    {
        int key;
        char buffer[200];
        scanf("%d %199s", &key, buffer);
        cachePut(key, buffer);
    }
    else if (strcmp(command, "get") == 0)
    {
        int key;
        scanf("%d", &key);
        char *result = cacheGet(key);

        if (result == NULL)
        {
            printf("NULL\n");
        }
        else
        {
            printf("%s\n", result);
        }
    }
    else if (strcmp(command, "exit") == 0)
    {
        exit(0);
    }
}

int main()
{
    char command[20];

    while (scanf("%19s", command) == 1)
    {
        processCommand(command);
    }

    return 0;
}
