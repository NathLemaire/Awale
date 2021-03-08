#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"

int main()
{
    int iter = 500000;
    int w = 0;
    Node f_node = {NULL, NULL, NULL, 0, 0, 0};
    Node* root = &f_node;
    long tab[3];
    long board[] = {4,4,4,4,4,4,4,4,4,4,4,4};
    build_board(tab, board, 0, 0, 1);
    root->game = tab;
    srand(time(NULL));
    //clock_t begin = clock();
    while(!w){
        root = proceed_mcts(root, iter);
        //print_nodes(root, 0);
        w = is_terminal(root->game);
        if(w){
            //free_recursivly(root);
            printf("Winner %d", w);
            return 0;
        }
        int p = 0;
        scanf("%d", &p);
        Node* chosen = human_plays(root, p);
        root = chosen;
        w = is_terminal(root->game);
    }
    free_recursivly(root);
    printf("Winner %d", w);
    print_nodes(root, 0);
    //clock_t end = clock();
    //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}
