#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX

clock_t begin, end;
char cities[30][30];
int dist[30][30] = {0};
int adj[30][30] = {0};

double getExecuteTime()
{
    end = clock();
    double diff = end - begin;
    return diff / CLOCKS_PER_SEC * 1000;
}

typedef struct heap
{
    int vertex;
    int weight;
} HEAP;

int heap_size = 1;

void swap(HEAP h[], int a, int b)
{
    HEAP temp;
    temp = h[a];
    h[a] = h[b];
    h[b] = temp;
}

void min_heapify(HEAP heap[], int idx)
{
    int left = idx * 2;
    int right = idx * 2 + 1;
    int smallest = 0;

    if (left <= heap_size && heap[left].weight < heap[idx].weight)
    {
        smallest = left;
    }
    else
    {
        smallest = idx;
    }

    if (right <= heap_size && heap[right].weight < heap[smallest].weight)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        swap(heap, idx, smallest);
        min_heapify(heap, smallest);
    }
}

void build_min_heap(HEAP heap[])
{
    for (int i = heap_size / 2; i > 0; i--)
    {
        min_heapify(heap, i);
    }
}

void push(HEAP heap[], int idx, int key)
{
    heap[heap_size].weight = key;
    heap[heap_size].vertex = idx;

    build_min_heap(heap);
    heap_size++;
}

HEAP pop(HEAP heap[])
{
    HEAP temp = heap[1];
    heap[1] = heap[heap_size - 1];
    heap_size--;
    min_heapify(heap, 1);
    return temp;
}

void print_heap(HEAP heap[])
{
    for (int i = 1; i <= heap_size; i++)
    {
        printf("[%d, %d]\n", heap[i].vertex, heap[i].weight);
    }
    printf("\n");
}

void increase_key(HEAP heap[], int idx, int val)
{
    heap[idx].weight = val;

    while (idx > 1 && heap[idx / 2].weight > heap[idx].weight)
    {
        swap(heap, idx, idx / 2);
        idx = idx / 2;
    }
}

int find(HEAP heap[], int idx)
{
    for (int i = 0; i <= heap_size; i++)
    {
        if (heap[i].vertex == idx)
            return i;
    }
    return -1;
}

int isWhiteSpace(char c)
{
    return c == '\n' || c == '\t' || c == '\n' || c == ' ' || c == '\r';
}

void printDist(int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i == 0)
            printf("           ");
        printf("%10s ", cities[i]);
    }
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        printf("%10s ", cities[i]);
        for (int j = 0; j < size; j++)
        {
            printf("%10d ", dist[i][j]);
        }
        printf("\n");
    }
}

void dijkstra(int size)
{
    HEAP pq[31];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j)
                dist[i][j] = 0;
            else
                dist[i][j] = INF;
        }
    }

    for (int start = 0; start < size; start++)
    {
        for (int i = 0; i < size; i++)
        {
            if (start == i)
                push(pq, i, 0);
            else
                push(pq, i, INF);
        }

        while (heap_size)
        {
            HEAP node = pop(pq);
            int w = node.weight;
            int v = node.vertex;

            for (int i = 0; i < size; i++)
            {
                if (adj[v][i] != INF)
                {
                    if (dist[start][i] > adj[v][i] + w)
                    {
                        dist[start][i] = adj[v][i] + w;
                        increase_key(pq, find(pq, i), adj[v][i] + w);
                    }
                }
            }
        }
    }
}

void floyd(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            dist[i][j] = adj[i][j];
        }
    }

    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (dist[i][k] == INF || dist[k][j] == INF)
                    continue;
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

int main()
{

    FILE *fp = fopen("hw7.data", "r");
    char line[256];

    fgets(line, sizeof(line), fp);

    int end = 0;
    int start = 0;
    int flag = 0;
    int size = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (isalpha(line[i]) && flag == 0)
        {
            start = i;
            flag = 1;
        }

        else if (!isalpha(line[i]) && flag == 1)
        {
            end = i;
            flag = 0;
            strncpy(cities[size++], line + start, end - start);
            cities[size - 1][end - start] = '\0';
        }
    }

    int row = 0;
    int col = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char temp[20];

        for (int i = 0; i < strlen(line); i++)
        {
            if (!isWhiteSpace(line[i]) && flag == 0)
            {
                start = i;
                flag = 1;
            }

            else if (isWhiteSpace(line[i]) && flag == 1)
            {
                end = i;
                flag = 0;
                strncpy(temp, line + start, end - start);
                temp[end - start] = '\0';

                if (!atoi(temp) && !strcmp(temp, "INF"))
                {
                    adj[row][col++] = INF;
                }
                else if (atoi(temp) || !strcmp(temp, "0"))
                {
                    adj[row][col++] = atoi(temp);
                }
            }
        }
        row++;
        col = 0;
    }

    begin = clock();
    dijkstra(size);
    double time = getExecuteTime();
    printf("It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows\n", time);
    printDist(size);
    printf("\n");

    begin = clock();
    floyd(size);
    time = getExecuteTime();
    printf("It took %f seconds to compute shortest path between cities with Floyd's algorithm as follows\n", time);
    printDist(size);
}
