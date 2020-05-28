#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main()
{
    FILE *fp = NULL;
    fp = fopen("hw6_data.txt", "r");
    char line[256];
    int x;
    int v = 0;
    int adj[20][20] = {0};
    int transadj[20][20] = {0};

    fgets(line, sizeof(line), fp);
    int idx = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int con = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '1')
            {
                adj[idx][con++] = 1;
            }
            else if (line[i] == '0')
            {
                adj[idx][con++] = 0;
            }
        }
        idx++;
    }

    for (int i = 0; i < idx; i++)
    {
        for (int j = 0; j < idx; j++)
        {
            if (adj[i][j] == 1)
            {
                transadj[j][i] = 1;
            }
        }
    }

    printf("=== Original Graph ===\n");
    for (int i = 0; i < idx; i++)
    {
        printf("%c : ", i + 65);
        for (int j = 0; j < idx; j++)
        {
            if (adj[i][j] == 1)
                printf("%c ", 65 + j);
        }
        printf("\n");
    }

    printf("\ntransposing...\n\n");
    printf("=== Transposed Graph ===\n");
    for (int i = 0; i < idx; i++)
    {
        printf("%c : ", i + 65);
        for (int j = 0; j < idx; j++)
        {
            if (transadj[i][j] == 1)
                printf("%c ", 65 + j);
        }
        printf("\n");
    }
}