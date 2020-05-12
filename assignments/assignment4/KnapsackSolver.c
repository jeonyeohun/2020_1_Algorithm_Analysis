// This program works with Greedy, DP, and Branch and Bound approaches

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h> 
#include <sys/time.h>

#define MAX_SIZE 50000
#define TIME_LIMIT_MS 900000

int dp [2][400001];
int heap_size=0;
clock_t start, end;

typedef struct item {
    int benefit;
    int weight;
    int level;
    double bpw;
    int bound;
}ITEM;
ITEM itemLists [10001];

double getExecuteTime(){
  end = clock();
  double diff = end - start;
  return diff/CLOCKS_PER_SEC * 1000;
}

int isTimeOver(){
  double time = getExecuteTime();
  return time > TIME_LIMIT_MS;
}

/*-------- priority queue related functions --------- */
void exchange (ITEM heap [], int a, int b){
    ITEM right = heap[a];
    heap[a] = heap[b];
    heap[b] = right;
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
    while(i > 1 && heap[i / 2].bound < heap[i].bound){
        exchange(heap, i, i / 2);
        i = i / 2;
    }
}

ITEM pop(ITEM heap[]){
    ITEM r = heap[1];
    
    exchange(heap, 1, heap_size);
    
    heap_size--;
    max_heapify(heap, 1);
    return r;
}
/* ---------------- until here --------------------- */

/* sorting condition for quick sort */
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
double greedy(ITEM * itemList, int N , int maxW){
    qsort(itemList + 1, N + 1, sizeof(ITEM), compare);
    int curW = 0;
    double maxBenefit = 0;
    for (int i = 1 ; i <= N ; i++){
        if (isTimeOver()) return -1;
        if (curW + itemList[i].weight > maxW){
            maxBenefit += (double)(itemList[i].bpw) * (maxW - curW);
            break;
        }
        curW += itemList[i].weight;
        maxBenefit = maxBenefit + itemList[i].benefit;
        if (curW >= maxW) break;
    }
    return maxBenefit;
}

/* Using Dynamic Programming Approach */
int slidingDP(ITEM * itemList, int N, int W){
  int maxBenefit = 0;
  for (int i = 1 ; i <= W ; i++) {
      dp[0][i] = 0;
  }

  for (int i = 1 ; i <= N ; i++){
      if (isTimeOver()) return -1;
      for(int w = 0 ; w <= W ; w++){
          int wi = itemList[i].weight;
          int bi = itemList[i].benefit;
          if(w < wi) dp[i % 2][w] = dp[1 - i % 2][w];
          else{
              if (dp[1 - i % 2][w] > dp[1 - i % 2][w - wi] + bi) {
                  dp[i % 2][w] = dp[1 - i % 2][w];
              }
              else dp[i % 2][w] = dp[1 - i % 2][w - wi] + bi;
            }
            if (maxBenefit < dp[i % 2][w]) maxBenefit = dp[i % 2][w];
        }
    }
    return maxBenefit;    
}

/* calculate bound of each node  */
int calcBound (ITEM * itemList, ITEM node, int N, int W){
    int totWeight = node.weight;
    int bound = node.benefit;
    int level = node.level+1;
    if (node.weight >= W || node.level > N) return 0;

    while(level <= N && totWeight + itemList[level].weight <= W){
        totWeight += itemList[level].weight;
        bound += itemList[level].benefit;
        level++;
    }

    if (level > N) return bound;
    else return bound + (W - totWeight) * itemList[level].bpw;
}

/* Using Branch and Bound Approach */
int bandb(ITEM * itemList, int N, int W){
    ITEM root;
    ITEM pq [MAX_SIZE];
    root.weight = 0;
    root.benefit = 0;
    root.level = 0;
    root.bound = calcBound(itemList,root, N, W);
    heap_size = 0;
    int maxBenefit = 0;
    push(pq, root);

    while (heap_size != 0){
        if (isTimeOver()) return -1;
        ITEM node = pop(pq);
        ITEM left, right = node;
        left.level = node.level + 1;
        
        if (left.level > N) {
          left.benefit = node.benefit;
          left.weight = node.weight;
        }
        else {
          left.benefit = node.benefit + itemList[left.level].benefit;
          left.weight = node.weight + itemList[left.level].weight;
        }
        
        if (left.weight <= W && left.benefit > maxBenefit){
            maxBenefit = left.benefit;
        }
        
        left.bound = calcBound(itemList, left, N, W);
        if (left.bound > maxBenefit){
            push(pq, left);
        }
      
        right.bound = calcBound(itemList, node, N, W);
        if (right.bound > maxBenefit){
            right.level = node.level + 1;
            push(pq, right);
        }
        
    }
    return maxBenefit;
}


/* Randomly Genrate Items */
ITEM randGeneration(){
    ITEM i;
    i.weight = rand() % 100 + 1;
    i.benefit = rand() % 300 + 1;
    i.level = 0;
    i.bpw = (double)i.benefit / i.weight;

    return i;
}

int main (){
    srand((unsigned int)time(NULL));
    double time;
    FILE *fp = fopen("output.txt", "w");
    fputs("# items\t | \t\tgreedy\t\t| \t\tDP\t\t| \t\tB&B\t\t |", fp);
    fputs("# items\t | \t\tgreedy\t\t | \t\t\tDP\t\t | \t\tB&B\t\t |", stdout);
    fputs("\n===================================================================\n", fp);
    fputs("\n===================================================================\n", stdout);
    fclose(fp);
    int teseCases [] = {10, 100, 500, 1000, 3000,5000, 7000, 9000, 10000};
    int N, W;
         
    for (int i = 0 ; i < sizeof(teseCases)/sizeof(teseCases[0]) ; i++){
      FILE *fp = fopen("output.txt", "a");
        N = teseCases[i];
        W =  N * 40;
        /* generate random N items */
       for (int i = 1 ; i <= N ; i++){
            itemLists[i] = randGeneration();
        }
        /* begin Greedy approach */
        start = clock();
	
        double result = greedy(itemLists, N, W);        
        time = getExecuteTime();

        if (result == -1) {
          fprintf(fp, "%5d\t\t time over\t\t", N);
          fprintf(stdout, "%5d\t\t time over\t\t", N);
        }
        else {
          fprintf(fp, "%6d\t |\t %.3f ms / %-.3f ", N, time, result);
          fprintf(stdout, "%6d\t |\t %.3f ms / %.3f ", N, time, result);
        }

        /* begin DP approach */
        start = clock();
        result = slidingDP(itemLists, N, W);
        time = getExecuteTime();

        if (result == -1) {
          fprintf(fp, "\ttime over\t\t");
          fprintf(stdout, "\ttime over\t\t");
        }
        else {
          fprintf(fp, "\t\t%.3f ms / %-10d ", time, (int)result);
          fprintf(stdout, "\t\t%.3f ms / %-10d ", time, (int)result);
        }

        /* begin Branch and Bound approach */
        start = clock();
        result = bandb(itemLists, N, W);
        time = getExecuteTime();

        if (result == -1) {
          fprintf(fp, "\ttime over\n");
          fprintf(stdout, "\ttime over\n");
        }
        else {
          fprintf(fp, "\t\t%.3f ms / %d \n ", time, (int)result);
          fprintf(stdout, "\t\t%.3f ms / %d \n ", time, (int)result);
        }
        fclose(fp);
    }
    return 0;
}