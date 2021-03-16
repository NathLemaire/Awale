#ifndef AWALE_H
#define AWALE_H

void print_board(long* a);
void print_score(long *a);
void add_score(long* a, int score, int player);
void increment(long* a, int place);
void set_place_zero(long* a, int place);
int get_value(long* a, int place);
int compute_score(long* a, int place, int nb_graines, int player);
void play(long* a, int place);
int get_available_moves(long* a, int* tab);
int get_random_move(long* a);
int is_terminal(long* a);
int choose_winner_from_score(long* a);
int finish_randomly(long* a);
void set_winner(long* a, int winner);
int get_winner(long* a);
void set_who_plays(long* a, int player);
int who_plays(long* a);
void build_board(long* a, long* b, long scoreS, long scoreN, long player);

#endif
