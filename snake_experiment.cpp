
#include <cmath>
#include <cstring>
#include "snake_experiment.h"
#include "snake.h"




Population *snake_test(int gens, int gen_id, char * filename, char * temp_name) {
    Population *pop=0;
    Genome *start_genome;
    char curword[20];
    int id = 0;

    ostringstream *fnamebuf;
    int gen;

    int expcount;
    int status;
    int runs[NEAT::num_runs];
    int totalevals;
    int samples;  //For averaging

    memset (runs, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile(filename,ios::in);

    cout<<"START SNAKE EVOLUTION"<<endl;

    cout<<"Reading in the start genome"<<endl;
    //Read in the start Genome
    do{
    iFile>>curword;
    if(strcmp(curword,"genomestart") == 0)
    iFile>>id;
    }while (id!= gen_id);
    cout<<"Reading in Genome id "<<id<<endl;
    start_genome=new Genome(id,iFile);
    iFile.close();
  
    //Run multiple experiments
    for(expcount=0;expcount<NEAT::num_runs;expcount++) {

      cout<<"EXPERIMENT #"<<expcount<<endl;

      cout<<"Start Genome: "<<start_genome<<endl;
      
      //Spawn the Population
      cout<<"Spawning Population off Genome"<<endl;
      
      pop=new Population(start_genome,NEAT::pop_size);
      
      cout<<"Verifying Spawned Pop"<<endl;
      pop->verify();

      for (gen=1;gen<=gens;gen++) {
	cout<<"Generation "<<gen<<endl;
	
	fnamebuf=new ostringstream();
	(*fnamebuf)<<"gen_"<<gen<<ends;  //needs end marker

#ifndef NO_SCREEN_OUT
	cout<<"name of fname: "<<fnamebuf->str()<<endl;
#endif	

	char temp[50];
        sprintf (temp, "%s_%d",temp_name, gen);

	status=snake_epoch(pop,gen,temp);
	//status=(pole1_epoch(pop,gen,fnamebuf->str()));
	
	if (status) {
	  runs[expcount]=status;
	  gen=gens+1;
	}
	
	fnamebuf->clear();
	delete fnamebuf;
	
      }

      if (expcount<NEAT::num_runs-1) delete pop;
    }

    totalevals=0;
    samples=0;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) {
      cout<<runs[expcount]<<endl;
      if (runs[expcount]>0)
      {
        totalevals+=runs[expcount];
        samples++;
      }
    }

    cout<<"Failures: "<<(NEAT::num_runs-samples)<<" out of "<<NEAT::num_runs<<" runs"<<endl;
    cout<<"Average evals: "<<(samples>0 ? (double)totalevals/samples : 0)<<endl;

    return pop;

}



int snake_epoch(Population *pop,int generation,char *filename) {
  vector<Organism*>::iterator curorg;
  vector<Species*>::iterator curspecies;
  //char cfilename[100];
  //strncpy( cfilename, filename.c_str(), 100 );

  //ofstream cfilename(filename.c_str());

  bool win=false;
  bool satisfy = false;
  int winnernum;
 
  //Evaluate each organism on a test

 

  
  for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) {
    if (snake_evaluate(*curorg)) win=true;
    else if ((*curorg)->fitness >= 5) satisfy = true;
  }
  
  //Average and max their fitnesses for dumping to file and snapshot
  for(curspecies=(pop->species).begin();curspecies!=(pop->species).end();++curspecies) {

    //This experiment control routine issues commands to collect ave
    //and max fitness, as opposed to having the snapshot do it, 
    //because this allows flexibility in terms of what time
    //to observe fitnesses at

    (*curspecies)->compute_average_fitness();
    (*curspecies)->compute_max_fitness();
  }

  //Take a snapshot of the population, so that it can be
  //visualized later on
  //if ((generation%1)==0)
  //  pop->snapshot();

  //Only print to file every print_every generations
  if  (win|| satisfy || 
       ((generation%(NEAT::print_every))==0))
    pop->print_to_file_by_species(filename);

  if (win) {
    for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) {
      if ((*curorg)->winner) {
	winnernum=((*curorg)->gnome)->genome_id;
	cout<<"WINNER IS #"<<((*curorg)->gnome)->genome_id<<endl;
      }
    }    
  }

  //Create the next generation
  pop->epoch(generation);

  if (win) return ((generation-1)*NEAT::pop_size+winnernum);
  else return 0;

}

bool snake_evaluate(Organism *org) {
  Network *net;
  double snake_l;
  int numnodes;  /* Used to figure out how many nodes
		    should be visited during activation */
  int thresh;  /* How many visits will be allowed before giving up 
		  (for loop detection) */

  //  int MAX_STEPS=120000;
  double MAX_FIT = pow(HEIGHT*WIDTH - 3, 2);
  
  net=org->net;
  numnodes=((org->gnome)->nodes).size();
  thresh=numnodes*2;  //Max number of visits allowed per activation
  
  //Try to balance a pole now
  org->fitness = snake_fitness(net ,&snake_l);

#ifndef NO_SCREEN_OUT
  cout<<"Org "<<(org->gnome)->genome_id<<" fitness: "<<org->fitness<< ", length:"<< snake_l <<endl;
#endif

  //Decide if its a winner
  if (org->fitness>=MAX_FIT) { 
    org->winner=true;
    return true;
  }
  else {
    org->winner=false;
    return false;
  }

}



double snake_fitness(Network * net, double* snake_l)
{
  int i,j;
  int prev_len, time_between_score = 0;
  double avg_time_between_scores;
  int computations = 3;
	double f1, f2, fitness[computations];
	int MAX_ITER = 1000;
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
  snake_len[j] = 3;
  prev_len = 0;
  avg_time_between_scores = 0;
  time_between_score = 0 ;
	initialize_snake_nn(input);

	do
	{
		if(num_iter >= MAX_ITER)
			break;
    if (snake_len[j] == prev_len)
      time_between_score++;
    else
    {
      prev_len = snake_len[j];
      avg_time_between_scores += time_between_score;
      time_between_score = 0;
    }
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
  avg_time_between_scores =(snake_len[j] - 3)? avg_time_between_scores / (snake_len[j] - 3) : avg_time_between_scores;
	f1 = pow(snake_len[j] -3, 2);
	f2 = (avg_time_between_scores > 5)? pow(avg_time_between_scores, 2) : pow(5000,2);
	fitness[j] = f1*f1/ f2;
}

  for(j=0;j<computations;j++)
  {
    avg_fit = avg_fit + (fitness[j]);
    avg_len = avg_len + snake_len[j];
  }	

  avg_len = avg_len/computations;
  avg_fit = avg_fit/computations;

  *snake_l = avg_len;
  return avg_fit;

}