#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"
#include "windows.h"
#include "psapi.h"

int main()
{
    int iter = 100000;
    Node f_node = {NULL, NULL, NULL, 0, 0, 0};
    Node* root = &f_node;
    clock_t begin = clock();
    long tab[3];
    long board[] = {4,4,4,4,4,4,4,4,4,4,4,4};
    build_board(tab, board, 0, 0, 1);
    root->game = tab;
    srand(time(NULL));
    while(!is_terminal(root->game)){
        root = proceed_mcts(root, iter);
        int w = is_terminal(root->game);
        if(w){
            free_recursivly(root);
            printf("Winner %d", w);
            return 0;
        }
        int p = 0;
        scanf("%d", &p);
        Node* chosen = human_plays(root, p);
        root = chosen;
        w = is_terminal(root->game);
        if(w){
            free_recursivly(root);
            printf("Winner %d", w);
            return 0;
        }
    }
    print_nodes(root, 0);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Iter %d, time %f", iter, time_spent);
}
