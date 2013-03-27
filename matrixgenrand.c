#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]){
	int argok=0;
	unsigned int rows;
	unsigned int cols;
	if(argc==3){
		long int r, c;
		r=strtol(argv[1], NULL, 10);
		c=strtol(argv[2], NULL, 10);
		if(r>0 && c>0){
			if(r>UINT_MAX)
				r=UINT_MAX;
			if(c>UINT_MAX)
				c=UINT_MAX;
			rows=r;
			cols=c;
			argok=1;
		}
	}

	if(!argok){
		printf("USAGE: matrixgenrand numrows numcols\n");
		exit(1);
	}

	size_t count = fwrite(&rows, sizeof(unsigned int), 1, stdout);
	count += fwrite(&cols, sizeof(unsigned int), 1, stdout);
	assert(count==2);

	srand((unsigned)time(NULL));
	count=0;
	for(unsigned int i=0; i<rows; ++i){
		for (unsigned int j=0; j<cols; ++j){
			double val=((double)rand()/(double)RAND_MAX)*2 -1;
			count+=fwrite(&val, sizeof(double), 1, stdout);
		}
		assert(count==cols);
		count=0;
	}

	return 0;
}
