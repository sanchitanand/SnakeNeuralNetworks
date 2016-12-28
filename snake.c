#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include "queue.h"
#include "snake.h"



int cycle = 0;
int end_game = 0;
int head;
int matrix[ WIDTH * HEIGHT];
int representation[WIDTH * HEIGHT + 1];
int snake_length, direction;
int killthread = 0;
pthread_mutex_t threadlock = PTHREAD_MUTEX_INITIALIZER;
struct termios old_tio;


struct_queue_t * queue ;



void initialize_snake_nn(int * output)
{
	end_game = 0;
	cycle = 0;
	int array[3] = {1 , 2 , 3 };
	time_t t;
	memset(matrix, 0, WIDTH*HEIGHT*4);
	snake_length = 3;
	memcpy(matrix + WIDTH*HEIGHT/2 - 1, array, 4*3); 
	head = WIDTH*HEIGHT/2 + 1;
	direction = RIGHT;
	srand((unsigned) time(&t));
	generate_food();
	generate_representation();
	memcpy(output,representation,(HEIGHT*WIDTH + 1)*4);

}
int test_iter(double * input, int * output , int  * num_iter, int * snake_len)
{

	int i;
	int ind = 0;
	double control;
	for(i=0;i<4;i++)
	{
		if (input[i] > input[ind])
			ind = i;

	}

	switch(ind)
	{	
		case UP:
			if(direction != DOWN)
				direction = UP;
			break;
		case LEFT:
			if(direction != RIGHT)
				direction = LEFT;
			break;
		case DOWN:
			if(direction != UP)
				direction = DOWN;
			break;
		case RIGHT:
			if(direction != LEFT)
				direction = RIGHT;
			break;


	}
	update_snake(direction);
	cycle = cycle + 1;
	generate_representation();
	*num_iter = cycle;
	*snake_len = snake_length;
	memcpy(output,representation,(HEIGHT*WIDTH + 1)*4);




	return (!end_game);






}


void generate_representation()
{
	int i, j;
	int num1, num2;
	int headx, heady;
	int xoff, yoff;
	headx = head%WIDTH;
	heady = (head - headx)/WIDTH;
	xoff = (WIDTH/2) - headx ;
	yoff = (HEIGHT/2)- heady ;

	for(i=0;i<HEIGHT;i++)
	{
		num1 = (i + yoff) % HEIGHT;
		if(num1 < 0)
			num1 = HEIGHT + num1;
		for(j=0;j<WIDTH;j++)
		{
			num2 = (j + xoff) % WIDTH;
			if (num2 < 0)
				num2 = WIDTH + num2;
			//representation[num1*WIDTH + num2 ] = matrix[i*WIDTH + j];
			switch(matrix[i*WIDTH + j])
			{
				case EMPTY:
					representation[num1*WIDTH + num2] = 0;
					break;
				case FOOD:
					representation[num1*WIDTH + num2] =  1;
					break;
				default:
					representation[num1*WIDTH + num2] = -1;
					break;
			}
		
		}


	}

	representation[HEIGHT*WIDTH] = 1;



}

/*void *gameloop(void * ptr)
{
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = INTERVAL_MS;
	initialize_snake();
	pthread_mutex_lock(&threadlock);
	while(!end_game)
	{
		update_snake(removeElement(queue));
		pthread_mutex_unlock(&threadlock);

		//generate_representation();

		clear_screen();
		display_matrix();
	
		//code to sleep our thread
		nanosleep(&tim, NULL);
		pthread_mutex_lock(&threadlock);

	}
	//kill the other thread too
	killthread = 1;
	pthread_mutex_unlock(&threadlock);

	clear_screen();
	display_score();





}
*/
void init_term()
{
	struct termios new_tio;
	unsigned char c;

	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);

	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;

	/* disable canonical mode (buffered i/o) and local echo */
	new_tio.c_lflag &=(~ICANON & ~ECHO);

	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

	queue = (struct_queue_t *) malloc(sizeof(struct_queue_t));
	queue->rear = -1;
	queue->front = 0;
	queue->count = 0;
	memset(queue->intArray, 0, MAX);

}

void restore_term()
{
	/* restore the former settings */
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}

void * inputloop(void * ptr)
{
//	struct timespec tim;
//	tim.tv_sec = 0;
//	tim.tv_nsec = 300*MILLISECOND;
	int input = NONE;
	pthread_mutex_lock(&threadlock);
	while(!killthread)
	{
		pthread_mutex_unlock(&threadlock);
		input = get_input();
		pthread_mutex_lock(&threadlock);
		switch(input)
		{
			case LEFT:
				if(peek(queue) != RIGHT)
					insertElement(queue, LEFT);
				break;
			case RIGHT:
				if(peek(queue) != LEFT)
					insertElement(queue, RIGHT);
				break;
			case UP:
				if(peek(queue) != DOWN)
					insertElement(queue, UP);
				break;
			case DOWN:
				if(peek(queue) != UP)
					insertElement(queue, DOWN);
				break;
			case END:
				end_game = 1;
				killthread = 1;
				break;

		}
//		pthread_mutex_unlock(&threadlock);
//		nanosleep(&tim, NULL);
//		pthread_mutex_lock(&threadlock);
		//code to sleep thread
	}

	pthread_mutex_unlock(&threadlock);



}


void clear_screen()
{
	printf("\033[2J\033[1;1H");
}

int get_input()
{
	int c;
	int input = NONE ;
	int state = 0;


	c =  getchar();
	{
	switch(c)
         {
         	case 'w':
         		input = UP;
         		break;
         	case 'a':
         		input = LEFT;
         		break;
         	case 's':
         		input = DOWN;
         		break;
         	case 'd':
         		input = RIGHT;
         		break;
         	case 27:
         		input = END;
         		break;
         }
    }
	return input;
}

void display_score()
{
	int i;
	for(i=0;i<WIDTH;i++)
	{
		printf("*");
	}
	printf("\n");
	for(i=0;i<HEIGHT/2;i++)
		printf("\n");
	printf("Your final length was %d\n",snake_length);
	for(i=0;i<HEIGHT/2;i++)
		printf("\n");
	for(i=0;i<WIDTH;i++)
	{
		printf("*");
	}
	printf("\n");

}
void initialize_snake()
{
	int end_game = 0;
	int array[3] = {1 , 2 , 3 };
	time_t t;
	memset(matrix, 0, WIDTH*HEIGHT);
	snake_length = 3;
	memcpy(matrix + WIDTH*HEIGHT/2 - 1, array, 4*3); 
	head = WIDTH*HEIGHT/2 + 1;
	insertElement(queue, RIGHT);
	srand((unsigned) time(&t));
	generate_food();

}

void generate_food()
{
	int i;
	do
	{

		i  =  rand() % (WIDTH*HEIGHT);	


	} while (matrix[i] != 0);

	matrix[i] = FOOD;
	return;

}

void update_snake(int direction)
{
	int i ;
	if(end_game)
		return;
	
	switch(direction)
	{
		case UP:
			move_snake(0,-1);
			break;
		case DOWN:
			move_snake(0,1);
			break;
		case LEFT:
			move_snake(-1,0);
			break;
		case RIGHT:
			move_snake(1,0);
			break;

	}

	for(i = 0 ; i < HEIGHT*WIDTH; i++)
	{
		if(matrix[i]>0)
			matrix[i]--;
	}

}

void move_snake(int horiz, int vert)
{
	if(horiz == 1)
	{
		if(head%(WIDTH) == WIDTH-1)
		{
			head = head - (WIDTH);
		}
		else
		{
			goto def;
		}
	}
	else if(horiz == -1)
	{
		if(head%(WIDTH) == 0)
		{
			head = head + (WIDTH);
		}
		else
		{
			goto def;
		}
	}
	else if (vert == 1)
	{
		if(head >= (HEIGHT-1)* WIDTH)
		{
			head = head - (HEIGHT)*WIDTH;
		}
		else
		{
			goto def;
		}
	}
	else if (vert == -1)
	{
		if(head < WIDTH)
		{
			head = head + (HEIGHT)*WIDTH;
		}
		else
		{
			goto def;
		}
	}

	def:
	head = head + horiz + vert*WIDTH;

	if(matrix[head] == FOOD)
	{		
		snake_length++;
		if (snake_length == WIDTH*HEIGHT)
			end_game = 1;
		else
			generate_food();
	}

	else if(matrix[head] > 0)
	{
		end_game = 1;
	}

	matrix[head] = snake_length + 1;




}




void display_matrix()
{
	int i,j;
	printf("Current length is : %d\n", snake_length);

	printf(" ");
	for(i=1;i<WIDTH;i++)
	{
		printf("-");
	}
	printf("\n");
	for(i=0;i<HEIGHT;i++)
	{
		printf("|");
		for(j=0;j<WIDTH;j++)
		{
			switch(matrix[i*WIDTH + j])
			{
				case EMPTY :
							printf(" ");
							break;
				case FOOD  :
							printf("*");
							break;
				default   :
							printf("#");
							break;


			}
			
		}

		printf("|\n");
	}

	printf(" ");
	for(i=1;i<WIDTH;i++)
	{
		printf("-");
	}
	printf("\n");



/*
	printf(" ");
	for(i=1;i<WIDTH;i++)
	{
		printf("-");
	}
	printf("\n");
	for(i=0;i<HEIGHT;i++)
	{
		printf("|");
		for(j=0;j<WIDTH;j++)
		{
			switch(representation[i*WIDTH + j])
			{
				case EMPTY :
							printf(" ");
							break;
				case FOOD  :
							printf("*");
							break;
				default   :
							printf("#");
							break;


			}
			
		}

		printf("|\n");
	}

	printf(" ");
	for(i=1;i<WIDTH;i++)
	{
		printf("-");
	}
	printf("\n");
*/

}

/*
int main ()
{
	//create threads
	int ret;
	init_term();

	pthread_t thread1,thread2;
	ret = pthread_create(&thread1, NULL, gameloop, NULL);
	if(ret)
	{
		printf("Failure to initiate gameloop");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&thread2 , NULL, inputloop, NULL);
	if(ret)
	{
		printf("Failure to initiate inputloop");
		exit(EXIT_FAILURE);
	}


	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	restore_term();
	free(queue);	

	return 0;
}
*/
