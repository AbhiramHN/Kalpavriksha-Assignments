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
    int state; 
} HashEntry;

typedef struct LRUCache
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    HashEntry table[HASH_SIZE];
} LRUCache;

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
    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->value = malloc(strlen(valueText) + 1);
    if (newNode->value == NULL)
    {
        free(newNode);
        return NULL;
    }

    newNode->key = key;
    strcpy(newNode->value, valueText);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

LRUCache *createCache(int capacity)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    if (cache == NULL)
    {
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    for (int slot = 0; slot < HASH_SIZE; slot++)
    {
        cache->table[slot].state = 0;
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
        if (cache->table[index].state == 0)
        {
            return NULL;
        }

        if (cache->table[index].state == 1 && cache->table[index].key == key)
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

    while (cache->table[index].state == 1)
    {
        if (cache->table[index].key == key)
        {
            cache->table[index].node = node;
            return;
        }
        index = (index + 1) % HASH_SIZE;
    }

    cache->table[index].state = 1;
    cache->table[index].key = key;
    cache->table[index].node = node;
}

void removeFromHash(LRUCache *cache, int key)
{
    unsigned long index = hashFunction(key);

    while (cache->table[index].state != 0)
    {
        if (cache->table[index].state == 1 && cache->table[index].key == key)
        {
            cache->table[index].state = 2;
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

char *cacheGet(LRUCache *cache, int key)
{
    Node *node = hashGet(cache, key);
    if (node == NULL)
    {
        return NULL;
    }
    moveToFront(cache, node);
    return node->value;
}

void cachePut(LRUCache *cache, int key, const char *valueText)
{
    Node *existing = hashGet(cache, key);

    if (existing != NULL)
    {
        char *newVal = malloc(strlen(valueText) + 1);
        if (newVal == NULL)
        {
            return;
        }

        free(existing->value);
        existing->value = newVal;
        strcpy(existing->value, valueText);

        moveToFront(cache, existing);
        return;
    }

    Node *newNode = createNode(key, valueText);
    if (newNode == NULL)
    {
        return;
    }

    addToFront(cache, newNode);
    hashInsert(cache, key, newNode);
    cache->size++;

    if (cache->size > cache->capacity)
    {
        evict(cache);
    }
}

void deleteCache(LRUCache *cache)
{
    Node *temp = cache->head;
    while (temp != NULL)
    {
        Node *next = temp->next;
        free(temp->value);
        free(temp);
        temp = next;
    }
    free(cache);
}

LRUCache *cacheInstance = NULL;

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
        cachePut(cacheInstance, key, buffer);
    }
    else if (strcmp(command, "get") == 0)
    {
        int key;
        scanf("%d", &key);
        char *result = cacheGet(cacheInstance, key);

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
        deleteCache(cacheInstance);
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
