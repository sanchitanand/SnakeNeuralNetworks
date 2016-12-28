#include "snake_test.h"
#include "snake.h"
#include <time.h>
#include <cmath>
#include <cstring>



double snake_test_fn(Network * net, int * cycles)
{

  Organism * org;
  struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 100* MILLISECOND;	 	
  int i,j;
  int computations = 1;
	double f1, f2, fitness[computations];
	int MAX_ITER = 20000;
	int num_iter = 0;
	int snake_len[computations];
	int input[HEIGHT*WIDTH + 1];
  double myinp[HEIGHT*WIDTH + 1];
	vector<NNode*>::iterator out_iter;
	double output[4];
  double avg_fit=0;
  double avg_len=0;
  for(j=0;j<computations; j++ )
{  
  num_iter = 0;
  snake_len[j] = 0;
	initialize_snake_nn(input);
	do
	{
		clear_screen();
		display_matrix();
		nanosleep(&tim, NULL);
		if(num_iter >= MAX_ITER)
			break;


    for(i = 0; i<HEIGHT*WIDTH + 1;i++)
      myinp[i] = input[i];

		 net->load_sensors(myinp);




		if (!(net->activate())) return 0;

		out_iter = net->outputs.begin();

		for(i=0;i<4;i++)
		{
			output[i] = (*out_iter)->activation;
			out_iter++;

		}



	}while(test_iter(output,input,&num_iter,&snake_len[j]));
  net->flush();
	f1 = pow(snake_len[j] -3, 2);
	//f2 =  (snake_len -3) /log(num_iter);
	fitness[j] = f1;
}

  for(j=0;j<computations;j++)
  {
    avg_fit = avg_fit + (fitness[j]);
    avg_len = avg_len + snake_len[j];
  }	

  avg_len = avg_len/computations;
  avg_fit = avg_fit/computations;

  //*snake_l = avg_len;
  *cycles = num_iter;
  return avg_fit;

}

int snake_test_routine(int winnerid, char * filename)
{
	Organism * org;
	Genome * start_genome;
	int cycles;
	init_term();
	char curword[20];
	int id = 0;
	ifstream iFile(filename,ios::in);
	do
	{
		iFile >> curword;
		if (strcmp(curword, "genomestart") == 0)
		{
			iFile >> id;
		}


	} while(id != winnerid);
	start_genome=new Genome(id,iFile);
    iFile.close();
    org = new Organism(0,start_genome,1, NULL);
    org->fitness = snake_test_fn(org->net, &cycles);
    clear_screen();
    int i;
	for(i=0;i<WIDTH;i++)
	{
		printf("*");
	}
	printf("\n");
	for(i=0;i<HEIGHT/2;i++)
		printf("\n");
	printf("Num. cycles : %d\n",cycles);
	for(i=0;i<HEIGHT/2;i++)
		printf("\n");
	for(i=0;i<WIDTH;i++)
	{
		printf("*");
	}
	printf("\n");

	restore_term();

	return 1;



}