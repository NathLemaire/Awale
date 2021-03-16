#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"

int main()
{
    int iter = 10000;
    int w = 0;
    Node f_node = {NULL, NULL, NULL, 0, 0, 0};
    Node* root = &f_node;
    long tab[3];
    long board[] = {4,4,4,4,4,4,4,4,4,4,4,4};
    build_board(tab, board, 0, 0, 1);
    root->game = tab;
    srand(time(NULL));
    int time_limit = 2;

    while(!w){
        root = proceed_mcts(root, iter, time_limit);
        //print_nodes(root, 0);
        w = is_terminal(root->game);
        if(w){
            break;
            return 0;
        }
        int p = rand() % root->nb_sons;
        Node* chosen = human_plays(root, p);
        root = chosen;
        w = is_terminal(root->game);
    }
    free_recursivly(root);
    if(w == 2){
    	printf("Winner Sud\n");
    }else if(w == 1){
        printf("Partie Nulle\n");
    }else{
    	printf("Winner Nord\n");
    }
    print_nodes(root, 0);
}
