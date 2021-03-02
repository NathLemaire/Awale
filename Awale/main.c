#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"

void proceed_mcts(Node* root, int iter){
    printf("MCTS proceeding\n");
    while(iter--){
        Node* chosen = selection(root);
        expansion(chosen);
        //printf("Iter %d\n", iter);
        //print_nodes(&root, 0);
        //scanf("%d", &he);
    }
    //print_nodes(root, 0);
    *root = *find_successor(root);
    root->father = NULL;
    print_board(root->game);
    print_score(root->game);
}

void human_plays(Node* root, int play){
    while(play >= root->nb_sons){
        printf("Play incorrect, new play:");
        scanf("%d", &play);
    }
    Node* n = root->sons[play];
    *root = *n;
    print_board(root->game);
    print_score(root->game);
    root->father = NULL;
}

int main()
{
    int iter = 200000;
    Node root = {NULL, NULL, NULL, 0, 0, 0};
    clock_t begin = clock();
    long tab[3];
    long board[] = {4,4,4,4,4,4,4,4,4,4,4,4};
    build_board(tab, board, 0, 0, 1);
    root.game = tab;
    srand(time(NULL));
    while(!is_terminal(root.game)){
        proceed_mcts(&root, iter);
        int w = is_terminal(root.game);
        if(w){
            printf("Winner %d", w);
            return 0;
        }
        int p = 0;
        scanf("%d", &p);
        human_plays(&root, p);
    }
    print_nodes(&root, 0);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Iter %d, time %f", iter, time_spent);
}
