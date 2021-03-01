#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"
#include "mcts.h"


int main()
{
    int iter = 1000000;
    Node root = {NULL, NULL, NULL, 0, 0, 0};
    long a = 0xC4104104;
    long b = 0xC4104104;
    long c = 0xFC000104;
    long tab[] = {a,b,c};
    root.game = tab;
    srand(time(NULL));
    while(iter--){
        Node* chosen = selection(&root);
        expansion(chosen);
        //printf("Iter %d\n", iter);
        //print_nodes(&root, 0);
    }
    print_nodes(&root, 0);
}
