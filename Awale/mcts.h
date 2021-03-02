#ifndef MCTS_H
#define MCTS_H

typedef struct Node Node;
struct Node
{
    Node* father;
    Node** sons;
    long* game;
    long score;
    long played;
    int nb_sons;

};

int is_leaf(Node* n);
Node* choose_best_leaf(Node* current);
int rollout(Node* current);
Node* find_successor(Node* current);
Node* selection(Node* root);
void expansion(Node* current);
void back_tracking(Node* current, int value);
void print_nodes(Node* root, int profondeur);

#endif
