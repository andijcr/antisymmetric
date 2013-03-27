#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char *argv[]){
	if(argc!=2){
		printf("USAGE: matrixdump matrix_file\n");
		exit(1);
	}

	//file open and file exists check
	FILE* f_matr=fopen(argv[1], "r");
	if(!f_matr){
		printf("error opening file");
		exit(1);
	}

	unsigned int rows, cols;

	//matrix dimention read
	size_t count=fread(&rows, sizeof(unsigned int), 1, f_matr);
	count+=fread(&cols, sizeof(unsigned int), 1, f_matr);
	if(count!=2){
		fclose(f_matr);
		printf("error reading matrix dimensions");
		exit(1);
	}

	count=0;
	for(unsigned int i=0; i<rows; ++i){
		for(unsigned int j=0; j<cols; ++j){
			double val;
			count+=fread(&val, sizeof(double), 1, f_matr);
			printf("%+.3e ", val);
		}
		assert(count==cols);
		count=0;
		printf("\n");
	}

	fclose(f_matr);

	return 0;
}
