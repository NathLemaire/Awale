#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    printf("Nord: %ld \n", a[2] >> 12 & 0x3F);
    printf("Sud: %ld \n\n", a[2] >> 18 & 0x3F);
}

void add_score(long* a, int score, int player){
    a[2] += score << (2+player)*6;
}

void increment(long* a, int place){
    if(place < 5) a[0] += 1 << 6 * place;
    else if(place < 10) a[1] += 1 << 6 * (place-5);
    else a[2] += 1 << 6 * (place-10);
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

void play(long* a, int place, int player){
    int value = get_value(a, place);
    int var = value;
    set_place_zero(a, place);
    while(value > 0){
        if(++place == 12) place = 0;
	increment(a, place);
        value--;
    }
    add_score(a, compute_score(a, place, var, player), player); 
}

int get_random_move(long* a, int player){
    int taille=0;
    int tab[6];
    int has_to_feed = 1;
    if (player) has_to_feed = !((a[0] & 0x3FFFFFFF) + (a[1] & 0x3F));
    else has_to_feed = !((a[1] >> 6 & 0xFFFFFF) + (a[2] & 0xFFF));
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
            int v = get_value(a, i);
            if(v){
                tab[taille] = i;
                taille++;
            }
        }
    }
    if(taille) return tab[rand() % taille];
    else return -1;
}

int check_end(long* a){
    int score_player0 = (a[2] >> 12 & 0x3F);
    int score_player1 = (a[2] >> 18 & 0x3F);
    if (score_player0 > 24) return 3;
    if (score_player1 > 24) return 2;
    int sum = 0;
    for(int i=0; i<12; i++){
        sum += get_value(a, i);
    }
    if(sum <= 3){
        if(score_player0 < score_player1) return 2;
        else if(score_player0 > score_player1) return 3;
        else return 1;
    }
    /*
    if(get_random_move(a, player) == -1){
        if(score_player0 < score_player1) return 2;
        else if(score_player0 > score_player1) return 3;
        else return 1;
    }*/
    return 0;
}

int choose_winner_from_score(long* a){
    int score_player0 = (a[2] >> 12 & 0x3F);
    int score_player1 = (a[2] >> 18 & 0x3F);
    if(score_player0 < score_player1) return 2;
    else if(score_player0 > score_player1) return 3;
    else return 1;
}

int finish_randomly(long* a, int player){
    int w = check_end(a);
    int nb = 0;
    while(!w){
    	int m = get_random_move(a, player);
        if(m == -1){
	    w = choose_winner_from_score(a);
            break;
        }
        //printf("Next move %d, player %d\n", m, player);
        play(a, m, player);
        //print_board(a);
        //print_score(a);
        w = check_end(a);
	if(w) break;
	m = get_random_move(a, 1-player);
	if(m == -1){
	    w = choose_winner_from_score(a);
	    break;
        }
        //printf("Next move %d, player %d\n", m, 1-player);
        play(a, m, 1-player);
        //print_board(a);
        //print_score(a);
        nb += 2;
    }
    //printf("Iterations : %d\n", nb);
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

int get_winner(long* a){
    return a[2] >> 24 & 0x3;
}

int main(void)
{
    clock_t begin = clock();
    //long a = 0xC4104104;
    //long b = 0xC4104104;
    //long c = 0xFC000104;
    //long d = 0xC1085200;
    //long e = 0xC70010C4;
    //long f = 0xFC000182;

    srand(time(NULL));
    long a = 0xC4104104;
    long b = 0xC4104104;
    long c = 0xFC000104;
    for(int i=0; i<100000; i++){
        a = 0xC4104104;
        b = 0xC4104104;
        c = 0xFC000104;
        long tab[] = {a,b,c};
        finish_randomly(tab, 1);
    } 
    //long tab[] = {a,b,c};
    //long tab2[] = {d, e, f};

    /*
    print_board(tab);    
    int w = finish_randomly(tab, 1);
    set_winner(tab, w);*/
    //printf("Winner %d\n\n", get_winner(tab)); 
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f", time_spent);
    return 0; 
}