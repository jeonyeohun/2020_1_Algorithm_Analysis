#include <stdio.h>
#include <time.h>

long long getComb_recursive (int n, int k){
    if ((n>=0 && k ==0) || (n==k)){
        return 1;
    }
    else if (k > 0 && n == 0){
        return 0;
    }
    else{
        return getComb_recursive(n-1, k-1)+getComb_recursive(n-1, k);
    }
}

long long getComb_dp (int n, int k){
    long long nk[n+1][k+1];
    
    for (int i = 0 ; i <= n ; i++){
        for (int j = 0 ; j <= k ; j++){
            if ((i>=0 && j ==0) || (j==i)) nk[i][j] = 1;
            else if (j > 0 && i == 0) nk[i][j] = 0;
            else nk[i][j] = nk[i-1][j-1]+nk[i-1][j];
        }
    }
    return nk[n][k];
}

int main (){
    int n, k;
    scanf("%d %d", &n, &k);
    
    if(n < 0 || k < 0){
        printf("Invalid Input.");
        return -1;
    }
    
    printf("==========================================\n");
    printf("Dynamic Programming Approach Compuatation Begins...\n\n");
    time_t start = time(NULL);
    
    long long result = getComb_dp(n, k);
    
    printf("Result: %lld\n\n", result);
    time_t end = time(NULL);
    printf("The computing time is: %ld seconds \n", (end-start));
    printf("==========================================\n");
    
    printf("Recursive Call Approach Compuatation Begins...\n\n");
    start = time(NULL);
    
    result = getComb_recursive(n, k);
    
    printf("Result: %lld\n\n", result);
    end = time(NULL);
    printf("The computing time is: %ld seconds \n", (end-start));
    printf("==========================================\n");

    return 0;
}
