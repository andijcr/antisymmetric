#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

typedef struct {
	unsigned int n_rows;
	double* elements;
}	t_asm;		//short for anti symmetric matrix


//maping formula: col+row*n_rows - (row+1)(row+2)*0.5
double getElement(t_asm m, unsigned int r, unsigned int c){
	double element;
	if(r==c){
		element=0;
	}
	if(r<c){
		//right upper triangle
		unsigned int index= c + r*m.n_rows - ((r+1)*(r+2))/2;
		element= m.elements[index];
	}
	if(r>c){
		//left lower triangle - invert col with row index
		unsigned int index= r + c*m.n_rows - ((c+1)*(c+2))/2;
		element= -m.elements[index];
	}
	return element;
}

int main(int argc, char *argv[]){
	int argok=0;
	t_asm m;
	if(argc==2){
		unsigned int r;
		r=strtol(argv[1], NULL, 10);
		if(r>0){
			if(r>UINT_MAX)
				r=UINT_MAX;							//realistically, this program cannot handle more than r=0.5*(UINT_MAX)^0.5 
			m.n_rows=r;
			argok=1;
		}
	}

	if(!argok){
		printf("USAGE: matrixgenantisym numrows\n");
		exit(1);
	}

	//gauss formula to count the number of elements in the uptter triangular matrix (minus the diagunal);
	unsigned int n_upper = (m.n_rows*(m.n_rows-1))/2;
	m.elements=malloc(sizeof(double)*n_upper);
	
	srand((unsigned)time(NULL));
	for(unsigned int i=0; i<n_upper; i++){
		(m.elements)[i]=((double)rand()/(double)RAND_MAX)*2 -1;
	}

	size_t count = fwrite(&(m.n_rows), sizeof(unsigned int), 1, stdout);
	count += fwrite(&(m.n_rows), sizeof(unsigned int), 1, stdout);
	assert(count==2);

	count=0;
	for(unsigned int i=0; i<m.n_rows; ++i){
		for (unsigned int j=0; j<m.n_rows; ++j){
			double val=getElement(m, i, j);
			count+=fwrite(&val, sizeof(double), 1, stdout);
		}
		assert(count==m.n_rows);
		count=0;
	}

	free(m.elements);
	return 0;
}
