#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "string.h"
#include "mcts.h"
#include "awale.h"

int is_leaf(Node* n){
    return !(n->nb_sons);
}

Node* choose_best_leaf(Node* current){
    Node* best = NULL;
    float max_node = 0;
    float score;
    float played;
    float temp_value;
    for(int i=0; i< current->nb_sons; i++){
        score = current->sons[i]->score;
        played = current->sons[i]->played;
        if(played){
            temp_value = score/played + 2 * sqrt(log(current->played)/played);
            if (max_node < temp_value){
                best = current->sons[i];
                max_node = temp_value;
            }
        }else{
            max_node = 10000000;
            best = current->sons[i];
            break;
        }
    }
    return best;
}


int rollout(Node* current){
    int w = get_winner(current->game);
    if(w) return w;
    long* a = (long*) malloc(sizeof(long)*3);
    a[0] = current->game[0];
    a[1] = current->game[1];
    a[2] = current->game[2];
    int c = finish_randomly(a);
    free(a);
    return c;
}

Node* selection(Node* root){
    while(!is_leaf(root)){
        root = choose_best_leaf(root);
    }
    return root;
}

Node* clone(Node* current){
    Node* new_son = (Node*) malloc(sizeof(Node));
    new_son->father = current;
    new_son->game = (long*) malloc(sizeof(long)*3);
    new_son->game[0] = current->game[0];
    new_son->game[1] = current->game[1];
    new_son->game[2] = current->game[2];
    new_son->score = 0;
    new_son->played = 0;
    new_son->nb_sons = 0;
    return new_son;
}

void expansion(Node* current){
    int w = 0;
    if(!current->played || get_winner(current->game)){
        w = rollout(current);
        int p = who_plays(current->game);
        if(w == 2) back_tracking(current, 2-2*p);
        else if(w == 3) back_tracking(current, 2*p);
        else back_tracking(current, 1);
        return;
    }
    else{
        int tab[6];
        int taille = get_available_moves(current->game, tab);
        current->sons = (Node**) malloc(taille*sizeof(Node*));
        for(int i=0; i<taille; i++){
            current->sons[i] = clone(current);
            play(current->sons[i]->game, tab[i]);
            int w = is_terminal(current->sons[i]->game);
            if(w) set_winner(current->sons[i]->game, w);
        }
        current->nb_sons = taille;
        w = rollout(current->sons[0]);
        int p = who_plays(current->game);
        if(w == 2) back_tracking(current->sons[0], 2*p);
        else if(w == 3) back_tracking(current->sons[0], 2-2*p);
        else back_tracking(current->sons[0], 1);
        return;
    }
}

void back_tracking(Node* current, int value){
	current->score += value;
    current->played += 1;
	if(current->father) back_tracking(current->father, 2-value);
}

Node* find_successor(Node* current, int* place){
    int max = -1;
    Node* best = NULL;
    for(int i = current->nb_sons; i--;){
        if(max<current->sons[i]->played){
            max = current->sons[i]->played;
            best = current->sons[i];
            *place = i;
        }
    }
    return best;
}

void print_nodes(Node* current, int profondeur){
    //if(current->played<1000) return;
    //if(profondeur > 1) return;
    profondeur++;
    printf("%d\n", profondeur);
    for(int i=0; i<current->nb_sons; i++){
        print_nodes(current->sons[i], profondeur);
    }
    /*
    float score = current->score;
    float played = current->played;
    float temp_value;
    float father_played;
    if(current->father) father_played = current->father->played;
    else father_played = played;
    if(played)
        temp_value = score/played + 1.41 * sqrt(log(father_played)/played);
    //if(profondeur > 50) printf("Score %ld, Played %ld, Nb_sons %d, Profondeur %d, Who_plays %d, Score %f, father played %f\n", current->score, current->played, current->nb_sons, profondeur, who_plays(current->game), temp_value, father_played);
    print_board(current->game);*/
}

void free_recursivly(Node* current){
    for(int i=0; i<current->nb_sons; i++){
        free_recursivly(current->sons[i]);
    }
    free(current->game);
    free(current->sons);
    free(current);
}

void free_brother_father_node(Node* father, int place){
    for(int i=0; i<father->nb_sons; i++){
        if(i != place){
            free_recursivly(father->sons[i]);
        }
    }
    free(father->sons);
    free(father->game);
    free(father);
}

Node* proceed_mcts(Node* root, int iter){
    printf("MCTS proceeding\n");
    while(iter--){
        Node* chosen = selection(root);
        expansion(chosen);
        //printf("Iter %d\n", iter);
        //print_nodes(&root, 0);
        //scanf("%d", &he);
    }
    //print_nodes(root, 0);
    int place;
    Node* next = find_successor(root, &place);
    free_brother_father_node(root, place);
    next->father = NULL;
    print_board(next->game);
    print_score(next->game);
    return next;
}

Node* human_plays(Node* root, int play){
    while(play >= root->nb_sons){
        printf("Play incorrect, new play:");
        scanf("%d", &play);
    }
    Node* n = root->sons[play];
    free_brother_father_node(root, play);
    print_board(n->game);
    print_score(n->game);
    n->father = NULL;
    return n;
}
