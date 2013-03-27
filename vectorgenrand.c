#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]){
	int argok=0;
	unsigned int rows;
	if(argc==2){
		long int r;
		r=strtol(argv[1], NULL, 10);
		if(r>0){
			if(r>UINT_MAX)
				r=UINT_MAX;
			rows=r;
			argok=1;
		}
	}

	if(!argok){
		printf("USAGE: vectorgenrand numelements\n");
		exit(1);
	}

	size_t count = fwrite(&rows, sizeof(unsigned int), 1, stdout);
	assert(count==1);

	srand((unsigned)time(NULL));
	count=0;
	for(unsigned int i=0; i<rows; ++i){
		double val=((double)rand()/(double)RAND_MAX)*2 -1;
		count+=fwrite(&val, sizeof(double), 1, stdout);
	}
	assert(count==rows);

	return 0;
}
