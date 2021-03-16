#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "awale.h"

void print_board(long* a){
    printf("%ld ", a[1] & 0x3F);
    printf("%ld ", a[0] >> 24 & 0x3F);
    printf("%ld ", a[0] >> 18 & 0x3F);
    printf("%ld ", a[0] >> 12 & 0x3F);
    printf("%ld ", a[0] >> 6 & 0x3F);
    printf("%ld \n", a[0] & 0x3F);
    printf("%ld ", a[1] >> 6 & 0x3F);
    printf("%ld ", a[1] >> 12 & 0x3F);
    printf("%ld ", a[1] >> 18 & 0x3F);
    printf("%ld ", a[1] >> 24 & 0x3F);
    printf("%ld ", a[2] & 0x3F);
    printf("%ld \n\n", a[2] >> 6 & 0x3F);
}

void print_score(long *a){
    printf("Sud: %ld \n", a[2] >> 12 & 0x3F);
    printf("Nord: %ld \n\n", a[2] >> 18 & 0x3F);
}

void add_score(long* a, int score, int player){
    a[2] += score << (3-player)*6;
}

void increment(long* a, int place){
    if(place < 5) a[0] += (1 << 6 * place);
    else if(place < 10) a[1] += (1 << 6 * (place-5));
    else a[2] += (1 << 6 * (place-10));
}

void set_place_zero(long* a, int place){
    if(place < 5) a[0] &= ~(0x3F << 6*place);
    else if(place < 10) a[1] &= ~(0x3F << 6*(place-5));
    else a[2] &= ~(0x3F << 6*(place-10));
}

int get_value(long* a, int place){
    if(place < 5) return a[0] >> 6*(place) & 0x3F;
    else if(place < 10) return a[1] >> 6*(place-5) & 0x3F;
    else return a[2] >> 6*(place-10) & 0x3F;
}

int compute_score(long* a, int place, int nb_graines, int player){
    int mid = place-6*(1-player);
    int score = 0;
    if(mid < 0 || mid > 5) return 0;
    int value = get_value(a, place);
    if(1 < value && value < 4){
        score += value;
        set_place_zero(a, place);
        while(nb_graines-- > 0){
            if(!place--) place = 11;
            mid = place-6*(1-player);
            if(mid < 0 || mid > 5) continue;
            value = get_value(a, place);
            if(1< value && value < 4){
                score += value;
                set_place_zero(a, place);
            }
        }
    }
    return score;
}

void play(long* a, int place){
    int value = get_value(a, place);
    int var = value;
    int begin = place;
    int player = who_plays(a);
    set_place_zero(a, place);
    while(value > 0){
        if(++place == 12) place = 0;
        if(place == begin) continue;
        increment(a, place);
        value--;
    }
    add_score(a, compute_score(a, place, var, player), player);
    set_who_plays(a, 1-player);
}

int get_available_moves(long* a, int* tab){
    int player = who_plays(a);
    int taille=0;
    int has_to_feed = 1;
    int seeds0 = (a[0] & 0x3FFFFFFF) + (a[1] & 0x3F);
    int seeds1 = (a[1] >> 6 & 0xFFFFFF) + (a[2] & 0xFFF);
    if(seeds0 + seeds1 <= 5) return 0;
    if (player) has_to_feed = !seeds0;
    else has_to_feed = !seeds1;
    if(has_to_feed){
        for(int i=6*player; i<6*(1+player); i++){
            int v = get_value(a, i);
            if(v && i+v-6*(1+player)>=0){
                tab[taille] = i;
                taille++;
            }
        }
    }else{
    	for(int i=6*player; i<6*(1+player); i++){
            if(get_value(a, i)){
                tab[taille] = i;
                taille++;
            }
        }
    }
    return taille;
}

int get_random_move(long* a){
    int tab[6];
    int taille = get_available_moves(a, tab);
    if(taille) return tab[rand() % taille];
    else return -1;
}

int is_terminal(long* a){
    int score_player1 = (a[2] >> 12 & 0x3F);
    int score_player0 = (a[2] >> 18 & 0x3F);
    if (score_player0 > 24) return 3;
    if (score_player1 > 24) return 2;
    int sum = 0;
    for(int i=0; i<12; i++){
        sum += get_value(a, i);
    }
    if(sum <= 5){
        if(score_player0 < score_player1) return 2;
        else if(score_player0 > score_player1) return 3;
        else return 1;
    }
    int tab[6];
    int taille = get_available_moves(a, tab);
    if(!taille){
        if(score_player0 < score_player1) return 2;
        else if(score_player0 > score_player1) return 3;
        else return 1;
    }
    return 0;
}

int finish_randomly(long* a){
    int w = is_terminal(a);
    while(!w){
    	int m = get_random_move(a);
        play(a, m);
        w = is_terminal(a);
    }
    return w;
}

void set_winner(long* a, int winner){
    switch(winner){
	case 1:
	    a[2] |= 0x1 << 24;
	    break;
	case 2:
        a[2] |= 0x2 << 24;
	    break;
	case 3:
	    a[2] |= 0x3 << 24;
	    break;
    }
}

void build_board(long* a, long* b, long scoreS, long scoreN, long player){
    long a_t = b[0] + (b[1] << 6) + (b[2] << 12) + (b[3] << 18) + (b[4] << 24);
    long b_t = b[5] + (b[6] << 6) + (b[7] << 12) + (b[8] << 18) + (b[9] << 24);
    long c_t = b[10] + (b[11] << 6)+ (scoreS << 12) + (scoreN << 18) + (player << 26);
    a[0] = a_t;
    a[1] = b_t;
    a[2] = c_t;
}

int get_winner(long* a){
    return a[2] >> 24 & 0x3;
}

void set_who_plays(long* a, int player){
    if(player) a[2] |= (player << 26);
    else a[2] &= ~((1-player) << 26);
}

int who_plays(long* a){
    return (a[2] >> 26) & 0x1;
}
