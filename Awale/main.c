#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"


int main()
{
    int iter = 0;
    Node root = {NULL, NULL, NULL, 0, 0, 0};
    int he = 0;
    clock_t begin = clock();
    long a = 0xC4104104;
    long b = 0xC4104104;
    long c = 0xFC000104;
    long tab[] = {a,b,c};
    root.game = tab;
    srand(time(NULL));
    while((clock() - begin) / CLOCKS_PER_SEC < 2){
        Node* chosen = selection(&root);
        expansion(chosen);
        //printf("Iter %d\n", iter);
        //print_nodes(&root, 0);
        //scanf("%d", &he);
        iter++;
    }
    print_nodes(&root, 0);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Iter %d", iter);
}
