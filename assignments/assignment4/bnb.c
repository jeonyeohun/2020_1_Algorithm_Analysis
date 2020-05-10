#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_SIZE 100000

typedef struct item {
    int benefit;
    int weight;
    int level;
    double bpw;
    int bound;
}ITEM;

int heap_size=0;

void exchange (ITEM heap [], int a, int b){
    ITEM temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

void max_heapify (ITEM heap [], int n){
    int l = n*2;
    int r = n*2+1;
    int largest;
    
    if (l <= heap_size && heap[l].bound > heap[n].bound) largest = l;
    else largest = n;
        
    if (r <= heap_size && heap[r].bound > heap[largest].bound) largest = r;
    
    if (largest != n) {
        exchange(heap, largest, n);
        max_heapify(heap, largest);
    }
}


void push(ITEM heap[], ITEM num){
    heap_size++;
    int i = heap_size;
    
    heap[i] = num;
    while(i > 1 && heap[i/2].bound < heap[i].bound){
        exchange(heap, i, i/2);
        i = i/2;
    }
}

ITEM pop(ITEM heap[]){
    ITEM r = heap[1];
    
    exchange(heap, 1, heap_size);
    
    heap_size--;
    max_heapify(heap, 1);
    return r;
}
/* <-------- queue related functions ---------> */


int static compare (const void * first, const void *second){
    ITEM firstB = *(ITEM *)first;
    ITEM secondB = *(ITEM *)second;
    
    if (firstB.bpw > secondB.bpw)
        return -1;
    else if (firstB.bpw < secondB.bpw)
        return 1;
    else
        return 0;
}

/* Using Greeady Algorithm Approach */
void greedy(ITEM * itemList, int N , int maxW){
    qsort(itemList+1, N+1, sizeof(ITEM), compare);
    int curW = 0;
    double maxBenefit = 0;
    for (int i = 1 ; i <=N ; i++){
        if (curW + itemList[i].weight > maxW){
            maxBenefit += (double)(itemList[i].bpw) * (maxW-curW);
            break;
        }
        curW += itemList[i].weight;
        maxBenefit = maxBenefit + itemList[i].benefit;
        if (curW >= maxW) break;
    }
    printf("Max Benefit from Greedy with %d items: %f\n", N, maxBenefit);
}

/* Using Dynamic Programming Approach */
void dp(ITEM * itemList, int N, int W){
    int ** dpMemo = 0x0;
    dpMemo = (int **) malloc (sizeof(int*) * N+1);
    for (int i = 0 ; i <= W ; i++){
        dpMemo[i] = (int*) malloc (sizeof(int) * W);
    }
    for (int i = 0 ; i <=N ; i++){
        for(int w = 0 ; w <= W ; w++){
            if (i== 0 || w==0) {dpMemo[i][w] = 0;
                continue;}
            if (itemList[i].weight <= w){
                if(itemList[i].benefit + dpMemo[i-1][w-itemList[i].weight] > dpMemo[i-1][w]){ // 현재 물건만큼 가방을 비우고 현재 물건을 넣는게 이득인지 확인
                    dpMemo[i][w] = itemList[i].benefit + dpMemo[i-1][w-itemList[i].weight];
                    continue;
                }
            }
            dpMemo[i][w] = dpMemo[i-1][w]; // 이득이 아니라면 그냥 넣지말자
        }
    }
    printf("Max Benefit from DP with %d items: %d\n", N, dpMemo[N][W]);
    free(dpMemo[0]);
    free(dpMemo);
}

int calcBound (ITEM * itemList, ITEM node, int N, int W){
    int totWeight = node.weight;
    int bound;
    int level = node.level+1;
    if (node.weight >= W || node.level > N) return 0;

    else{
        bound = node.benefit;
        while(level<=N && totWeight + itemList[level].weight <= W){
            totWeight += itemList[level].weight;
            bound += itemList[level].benefit;
            level++;
        }
    }

    if (level > N) return bound;
    else return bound + (W - totWeight) * itemList[level].bpw;
}

/* Using Branch and Bound Approach */
void bandb(ITEM * itemList, int N, int W){
    ITEM root;
    ITEM pq [MAX_SIZE];
    root.weight = 0;
    root.benefit = 0;
    root.level = 0;
    root.bound = calcBound(itemList,root, N, W);
    
    int maxBenefit = 0;
    push(pq, root);

    while (heap_size != 0){
        ITEM node = pop(pq);
        ITEM temp;
        temp.level = node.level+1;
        if (temp.level > N) {
          temp.benefit = node.benefit;
          temp.weight = node.weight;
        }
        else {
          temp.benefit = node.benefit + itemList[temp.level].benefit;
          temp.weight = node.weight + itemList[temp.level].weight;
        }
        
        if (temp.weight <= W && temp.benefit > maxBenefit){
            maxBenefit = temp.benefit;
        }
        
        temp.bound =calcBound(itemList, temp, N, W);
        if (temp.bound > maxBenefit){
            push(pq, temp);
        }
        
        node.bound =calcBound(itemList, node, N, W);
        if (node.bound >maxBenefit){
            node.level += 1;
            push(pq, node);
        }
        
    }

    printf("Max Benefit from Branch & Bound with %d items: %d\n", N, maxBenefit);
    return;
}


/* Randomly Genrate Items */
ITEM randGeneration(){
    ITEM i;
    i.weight = rand()%100+1;
    i.benefit = rand()%300+1;
    i.level = 0;
    i.bpw = (double)i.benefit/i.weight;

    return i;
}

int main (){
      srand((unsigned int)time(NULL));
        int teseCases [9] = {10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000};
        int N, W;
        
        for (int i = 0 ; i < 9 ; i++){
            N = teseCases[i];
            W =  N * 40;
            ITEM itemList [N+1];
            for (int i = 1 ; i <= N ; i++){
                itemList[i] = randGeneration();
            }
            greedy(itemList, N, W);
            dp(itemList, N, W);
            bandb(itemList, N, W);
        }
}
