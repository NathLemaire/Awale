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
    float father_played;
    float temp_value;
    for(int i=0; i< current->nb_sons; i++){
        score = current->sons[i]->score;
        played = current->sons[i]->played;
        father_played = current->played;
        if(played){
            temp_value = score/played + 1.41 * sqrt(played)/father_played;
            //printf("Score : %f, Played: %f, Father %f, Value %f\n", score, played, father_played, temp_value);
            if (max_node < temp_value){
                best = current->sons[i];
                max_node = temp_value;
            }
        }else return current->sons[i];
    }
    //printf("Best value %f\n", temp_value);
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
    if(!new_son) printf("Couldnt malloc");
    new_son->father = current;
    long* a = (long*) malloc(sizeof(long)*3);
    if(!a) printf("Couldnt malloc");
    a[0] = current->game[0];
    a[1] = current->game[1];
    a[2] = current->game[2];
    new_son->game = a;
    new_son->score = 0;
    new_son->played = 0;
    new_son->nb_sons = 0;
    return new_son;
}

void expansion(Node* current){
    int w = 0;
    if(!current->played || get_winner(current->game)){
        w = rollout(current);
        //Si Sud doit jouer alors 3 bien et 2 nul (Nord doit gagner)
        //Si Nord doit jouer alors 2 bien et 3 nul (Sud doit gagner)
        int p = who_plays(current->game);
        if(w == 2) back_tracking(current, 2-2*p);
        else if(w == 3) back_tracking(current, 2*p);
        else back_tracking(current, 1);
        return;
    }
    else{
        int tab[6];
        int taille = get_available_moves(current->game, tab);
        if(!taille){
            w = rollout(current);
            set_winner(current->game, w);
            //Si Sud doit jouer alors 3 bien et 2 nul
            //Si Nord doit jouer alors 2 bien et 3 nul
            int p = who_plays(current->game);
            if(w == 2) back_tracking(current, 2-2*p);
            else if(w == 3) back_tracking(current, 2*p);
            else back_tracking(current, 1);
            return;
        }
        current->sons = (Node**) malloc(taille*sizeof(Node*));
        if(!current->sons) printf("Couldnt malloc");
        for(int i=0; i<taille; i++){
            current->sons[i] = clone(current);
            play(current->sons[i]->game, tab[i]);
            int w = is_terminal(current->sons[i]->game);
            if(w) set_winner(current->sons[i]->game, w);
        }
        current->nb_sons = taille;
        w = rollout(current->sons[0]);
        int p = who_plays(current->game);
        //Inverse puisque coup suivant
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

Node* find_successor(Node* current){
    int max = -1;
    Node* best = NULL;
    for(int i = current->nb_sons; i--;){
        if(max<current->sons[i]->played){
            max = current->sons[i]->played;
            best = current->sons[i];
        }
    }
    return best;
}

void print_nodes(Node* current, int profondeur){
    if(current->played<1000) return;
    if(profondeur > 1) return;
    profondeur++;
    for(int i=0; i<current->nb_sons; i++){
        print_nodes(current->sons[i], profondeur);
    }
    printf("Score %ld, Played %ld, Nb_sons %d, Profondeur %d, Who_plays %d\n", current->score, current->played, current->nb_sons, profondeur, who_plays(current->game));
    //print_score(current->game);
    print_board(current->game);
    //print_score(current->game);
    //printf("Sons\n");

}
