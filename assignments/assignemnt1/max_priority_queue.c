#include <stdio.h>
#include <string.h>

typedef struct heap
{
    char name[21];
    int key;
} HEAP;

int heap_size = 1;

void print_menu()
{
    printf("************  MENU  ****************\n");
    printf("I : Insert new element into queue.\nD : Delete element with largest key from queue.\nR : Retrieve element with largest keu from queue.\nK : Increase key of element in queue.\nP : Print out all elements in queue.\nQ : Quit\n");
}

void swap(HEAP h[], int a, int b)
{
    HEAP temp;
    temp = h[a];
    h[a] = h[b];
    h[b] = temp;
}

void max_heapify(HEAP heap[], int idx)
{
    int left = idx * 2;
    int right = idx * 2 + 1;
    int largest = 0;

    if (left <= heap_size && heap[left].key > heap[idx].key)
    {
        largest = left;
    }
    else
    {
        largest = idx;
    }

    if (right <= heap_size && heap[right].key > heap[largest].key)
    {
        largest = right;
    }

    if (largest != idx)
    {
        swap(heap, idx, largest);
        max_heapify(heap, largest);
    }
}

void build_max_heap(HEAP heap[])
{
    for (int i = heap_size / 2; i > 0; i--)
    {
        max_heapify(heap, i);
    }
}

void print_heap(HEAP heap[])
{
    for (int i = 1; i < heap_size; i++)
    {
        printf("[%s, %d] ", heap[i].name, heap[i].key);
    }
    printf("\n");
}

void insert(HEAP heap[])
{
    char name[21];
    int key;

    printf("Enter name of element: ");
    scanf("%s", name);
    printf("Enter key value of element: ");
    scanf("%d", &key);

    strcpy(heap[heap_size].name, name);
    heap[heap_size].key = key;

    getchar();
    printf("New element [%s, %d] is inserted\n", heap[heap_size].name, heap[heap_size].key);
    heap_size++;
}

void delete (HEAP heap[])
{
    HEAP temp = heap[1];
    for (int i = 2; i <= heap_size; i++)
    {
        heap[i - 1] = heap[i];
    }
    heap_size--;

    printf("[%s, %d] is deleted.\n", temp.name, temp.key);
}

void retrieve(HEAP heap[])
{
    printf("[%s, %d]\n", heap[1].name, heap[1].key);
}

void increase_key(HEAP heap[])
{
    int idx;
    int val;
    printf("Enter index of element: ");
    scanf("%d", &idx);
    printf("Enter key value of element: ");
    scanf("%d", &val);
    getchar();
    heap[idx].key = val;
}

int main()
{

    HEAP heap[31];

    while (1)
    {
        char op;
        print_menu();
        printf("Choose Menu: ");
        scanf("%c", &op);
        getchar();

        switch (op)
        {
        case 'I':
            insert(heap);
            build_max_heap(heap);
            break;
        case 'D':
            delete (heap);
            build_max_heap(heap);
            break;
        case 'R':
            retrieve(heap);
            break;
        case 'K':
            increase_key(heap);
            build_max_heap(heap);
            break;
        case 'P':
            print_heap(heap);
            break;
        case 'Q':
            break;
        }

        if (op == 'Q')
        {
            printf("Thank you. Bye!");
            break;
        }

        printf("\n");
    }
}
