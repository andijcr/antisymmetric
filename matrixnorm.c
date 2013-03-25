#include <stdio.h>
#include <stdlib.h>


typedef struct matrix{
	unsigned int rows;
	unsigned int cols;
	double* elements;
} t_matrix;

typedef struct vector{
	unsigned int count;
	double* elements;
} t_vector;

double v_element(unsigned int i, t_vector v){
	return v.elements[i];
}

double m_element(unsigned int i, unsigned int j, t_matrix m){
	return m.elements[i*(m.rows)+j];	
}

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("USAGE: matrixnorm matrix_file vector_file\n");
		exit(1);
	}

	//file open and file exists check
	FILE* f_matr=fopen(argv[0], "r");
	if(!f_matr){
		printf("error opening file");
		exit(1);
	}

	t_matrix* matrix=malloc(sizeof(t_matrix));

	//matrix dimention read
	size_t count=fread(&(matrix->rows), sizeof(unsigned int), 1, f_matr);
	count+=fread(&(matrix->cols), sizeof(unsigned int), 1, f_matr);
	if(count!=2){
		free(matrix);
		fclose(f_matr);
		printf("error reading matrix dimensions");
		exit(1);
	}

	return 0;
}