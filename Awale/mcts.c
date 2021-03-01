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
    float wins;
    float played;
    float father_played;
    float temp_value;
    for(int i=0; i< current->nb_sons; i++){
        wins = current->sons[i]->wins;
        played = current->sons[i]->played;
        father_played = current->played;
        if(played) temp_value = wins/played + 5 * sqrt(played)/father_played;
        else temp_value = 1000000000;
        if (max_node < temp_value){
            best = current->sons[i];
            max_node = temp_value;
        }
    }
    return best;
}


int rollout(Node* current){
    int w = get_winner(current->game);
    if(w) return w;
    long* a = (long*) malloc(sizeof(long)*3);
    if(!a) printf("Couldnt malloc");
    a[0] = current->game[0];
    a[1] = current->game[1];
    a[2] = current->game[2];
    int c = finish_randomly(a, who_plays(a));
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
    if(!new_son) printf("Couldnt malloc");
    new_son->father = current;
    long* a = (long*) malloc(sizeof(long)*3);
    if(!a) printf("Couldnt malloc");
    a[0] = current->game[0];
    a[1] = current->game[1];
    a[2] = current->game[2];
    new_son->game = a;
    new_son->wins = 0;
    new_son->played = 0;
    new_son->nb_sons = 0;
    return new_son;
}

void expansion(Node* current){
    int w = 0;
    if(!current->played){
        w = rollout(current);
        back_tracking(current, w<3?1:0);
    }
    else{
        int tab[6];
        int taille = get_available_moves(current->game, who_plays(current->game), tab);
        if(!taille){
            w = rollout(current);
            set_winner(current->game, w);
            back_tracking(current, w<3?1:0);
            return;
        }
        current->sons = (Node**) malloc(taille*sizeof(Node*));
        if(!current->sons) printf("Couldnt malloc");
        for(int i=0; i<taille; i++){
            current->sons[i] = clone(current);
            play(current->sons[i]->game, tab[i], 1-who_plays(current->game));
        }
        current->nb_sons = taille;
        w = rollout(current->sons[0]);
        back_tracking(current->sons[0], w<3?1:0);
    }
}

void back_tracking(Node* current, int value){
	current->wins += value;
    current->played += 1;
	if(current->father) back_tracking(current->father, value);
}

void print_nodes(Node* current, int profondeur){
    printf("Wins %ld, Played %ld, Nb_sons %d, Profondeur %d, Who_plays %d\n", current->wins, current->played, current->nb_sons, profondeur, who_plays(current->game));
    print_board(current->game);
    //print_score(current->game);
    //printf("Sons\n");
    profondeur++;
    for(int i=0; i<current->nb_sons; i++){
        print_nodes(current->sons[i], profondeur);
    }
}
