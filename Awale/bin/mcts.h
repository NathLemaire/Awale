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
Node* find_successor(Node* current, int* place);
Node* selection(Node* root);
void expansion(Node* current);
void back_tracking(Node* current, int value);
void print_nodes(Node* root, int profondeur);
void free_recursivly(Node* current);
void free_brother_father_node(Node* father, int place);
Node* proceed_mcts(Node* root, int iter, int time_limit);
Node* human_plays(Node* root, int play);


#endif
