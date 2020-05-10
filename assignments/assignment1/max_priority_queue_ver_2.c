/* This assignment was conducted on repl.it */

#include <stdio.h>
#include <string.h>

#define MAX_HEAP_SIZE 30

typedef struct heap
{
    char name[21];
    int key;
} HEAP;

int heap_size = 0; // counter variable to count number of elements in heap.

// exchange elements in array h[a] and h[b] //
void swap(HEAP h[], int a, int b)
{
    HEAP temp;
    temp = h[a];
    h[a] = h[b];
    h[b] = temp;
}

// max-heapify the given heap //
void max_heapify(HEAP heap[], int idx)
{
    int left = idx * 2;
    int right = idx * 2 + 1;
    int largest = 0;

    // find largest key value from left child
    if (left <= heap_size && heap[left].key > heap[idx].key)
    {
        largest = left;
    }
    else
    {
        largest = idx;
    }

    // find largest key value from right child
    if (right <= heap_size && heap[right].key > heap[largest].key)
    {
        largest = right;
    }

    // exchange the largest node and its parent and max-heapify the heap
    if (largest != idx)
    {
        swap(heap, idx, largest);
        max_heapify(heap, largest);
    }
}

// printing main menu //
void print_menu()
{
    printf("************  MENU  ****************\n");
    printf("I : Insert new element into queue.\nD : Delete element with largest key from queue.\nR : Retrieve element with largest key from queue.\nK : Increase key of element in queue.\nP : Print out all elements in queue.\nQ : Quit\n");
}

// get index of parent node of node with index idx //
int parent(int idx)
{
    return idx / 2;
}

// Return the maximum key. It should be placed in the index 1 //
HEAP heap_maximum(HEAP A[])
{
    return A[1];
}

// Delete the largest key. It is the node in index 1 //
HEAP heap_extract_max(HEAP A[])
{
    HEAP max = A[1];
    A[1] = A[heap_size];
    heap_size--;
    // we should re-max-heapify the heap after get rid of the first node.
    max_heapify(A, 1);

    // return the deleted node.
    return max;
}

// increase key of given //
void heap_increase_key(HEAP A[], int i, int key)
{
    // exception handler. The given node should be valid node.
    if (i > heap_size || i < 1)
    {
        printf("given index is empty!\n");
        return;
    }

    // exception handler. the new key should be bigger than the current key.
    if (key < A[i].key)
    {
        printf("new key is smaller than current key.\n");
        return;
    }

    // set new key into given node.
    A[i].key = key;

    // compare the key value of the node and its parent to maintain the max-heap.
    while (i > 1 && (A[parent(i)].key < A[i].key))
    {
        swap(A, i, parent(i));
        i = parent(i);
    }
}

// insert new node into the heap //
void max_heap_insert(HEAP A[], char *name, int key)
{
    heap_size++;
    strcpy(A[heap_size].name, name);
    A[heap_size].key = -1;
    heap_increase_key(A, heap_size, key);

    printf("New element [%s, %d] is inserted\n", name, key);
}

// print current heap elements //
void print_heap(HEAP heap[])
{
    for (int i = 1; i <= heap_size; i++)
    {
        printf("[%s, %d] ", heap[i].name, heap[i].key);
    }
    printf("\n");
}

// the input key awlays needs to be between 1 to 10.
int get_valid_key()
{
    int key = 0;
    while (key < 1 || key > 10)
    {
        printf("Enter key value of element: ");
        scanf("%d", &key);
        getchar();
        if (key < 1 || key > 10)
        {
            printf("invalid key. Put a number between 1 and 10.\n");
        }
    }
    return key;
}

int main()
{

    HEAP heap[31];

    while (1)
    {
        char name[21];
        int key = 0;
        char op;
        int i;
        print_menu();
        printf("Choose Menu: ");
        scanf("%c", &op);
        getchar();

        switch (op)
        {
        // insert
        case 'I':
            if (heap_size >= MAX_HEAP_SIZE)
            {
                printf("The queue is full! Delete first.\n");
                break;
            }
            printf("Enter name of element: ");
            scanf("%s", name);
            key = get_valid_key();
            max_heap_insert(heap, name, key);

            break;
        // delete
        case 'D':
            // exception handler. To delete node, there should be at least one node in the heap.
            if (heap_size < 1)
            {
                printf("Heap underflow.\n");
                break;
            }
            HEAP del = heap_extract_max(heap);
            printf("[%s, %d] is deleted.\n", del.name, del.key);
            break;
        // retrieve
        case 'R':
            // exception handler. To retrieve, there should be at least one node.
            if (heap_size < 1)
            {
                printf("No element to retrieve.\n");
                break;
            }
            HEAP max = heap_maximum(heap);
            printf("[%s, %d]\n", max.name, max.key);
            break;
        // increase
        case 'K':
            printf("Enter index of element: ");
            scanf("%d", &i);
            key = get_valid_key();
            heap_increase_key(heap, i, key);
            break;
        // print
        case 'P':
            print_heap(heap);
            break;
        // quit
        case 'Q':
            break;
        // exception for the invalid input
        default:
            printf("invalid option. try again.\n");
        }

        if (op == 'Q')
        {
            printf("Thank you. Bye!\n");
            break;
        }

        printf("\n");
    }
}
