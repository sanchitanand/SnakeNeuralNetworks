#ifndef SNAKE_C
#define SNAKE_C


#define WIDTH 	20
#define HEIGHT	11

#define EMPTY	0
#define FOOD   -1

#define UP 		0
#define LEFT 	1
#define DOWN 	2
#define RIGHT 	3
#define END 	4
#define NONE 	-1

#define MILLISECOND 1000000
#define INTERVAL_MS 100 * MILLISECOND



void restore_term();
void init_term();
void initialize_snake();
void update_snake(int direction);
void move_snake(int horiz, int vert);
void display_matrix();
void generate_food();
void clear_screen();
void *gameloop(void * ptr);
void *inputloop(void * ptr);
int  get_input();
void display_score();
void generate_representation();
void initialize_snake_nn(int * output);
int test_iter(double * input , int * output, int * num_iter, int* snake_len);
#endif