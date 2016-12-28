#include "snake.h"
#include <stdio.h>



main() {
	int rep[HEIGHT*WIDTH + 1];
	double input[4] = {1 ,0, 0 ,0};
	int num_iter;
	int i,j;
	FILE *fp;
	initialize_snake_nn(rep);
   	

   fp = fopen("mylogfile.txt", "w+");
   for(i=0;i<HEIGHT;i++)
   {
   		for(j=0;j<WIDTH;j++)
   		{
   			fprintf(fp,"%2d ",rep[i*WIDTH + j]);
   		}
   		fprintf(fp,"\n");
   	}
   
   	fprintf(fp, "\n\n\n");

   test_iter(input, rep, &num_iter);
   for(i=0;i<HEIGHT;i++)
   {
   		for(j=0;j<WIDTH;j++)
   		{
   			fprintf(fp,"%2d ",rep[i*WIDTH + j]);
   		}
   		fprintf(fp,"\n");
   	}
   fclose(fp);
}