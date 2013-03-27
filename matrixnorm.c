#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef struct matrix{
	unsigned int rows;
	unsigned int cols;
	double* elements;
} t_matrix;

typedef struct vector{
	unsigned int count;
	double* elements;
} t_vector;

double m_element(unsigned int i, unsigned int j, t_matrix m){
	return m.elements[i*(m.rows)+j];	
}

int verifyAntiSymmetry(t_matrix m){

	for(unsigned int i=0; i<m.rows; ++i){
		for(unsigned int j=0; j<m.cols; ++j){
			if(i==j && m_element(i, j, m)!=0.0){
				return 0;				//elements on diagonal must be 0
			} else if(i<j && m_element(i, j, m) != (-1 * m_element(j, i, m))){
				//right upper triangle
				return 0;
			}
			//no need to check for lower left triangle
		}
	}

	return 1;
}

double calcNorm(t_matrix m, t_vector v){
	t_vector tmp;
	tmp.count=v.count;
	tmp.elements=malloc(sizeof(double)*tmp.count);
	assert(tmp.elements!=NULL);

	//row*colum multiplication
	for(unsigned int i=0; i<m.rows; ++i){
		tmp.elements[i]=0;
		for(unsigned int j=0; j<m.cols; ++j){
			tmp.elements[i]+= m_element(i, j, m)*v.elements[j];
		}
	}

	double res=0;
	for(unsigned int i=0; i<tmp.count; ++i){
		res+=tmp.elements[i]*v.elements[i];
	}
	free(tmp.elements);

	return res;
}

int verifyAntiSymmetryCalcNorm(double* norm, t_matrix m, t_vector v){
	t_vector tmp;
	tmp.count=v.count;
	tmp.elements=malloc(sizeof(double)*tmp.count);
	assert(tmp.elements!=NULL);

	double res=0;

	for(unsigned int i=0; i<m.rows; ++i){
		tmp.elements[i]=0;
		for(unsigned int j=0; j<m.cols; ++j){
			double m_ele=m_element(i, j, m);

			if(i==j && m_ele!=0.0){
				free(tmp.elements);
				return 0;				//elements on diagonal must be 0
			} else if(i<j && m_ele != (-1 * m_element(j, i, m))){
				//right upper triangle
				free(tmp.elements);
				return 0;
			}
			//no need to check for lower left triangle

			tmp.elements[i]+= m_ele*v.elements[j];
		}
		res+=tmp.elements[i]*v.elements[i];
	}
	free(tmp.elements);

	*norm=res;
	return 1;
}

int main(int argc, char *argv[]){
	if(argc!=4){
		printf("USAGE: matrixnorm [naif | singlepass] matrix_file vector_file\n");
		exit(1);
	}

	//file open and file exists check
	FILE* f_matr=fopen(argv[2], "r");
	assert(f_matr!=NULL);

	FILE* f_vect=fopen(argv[3], "r");
	assert(f_vect!=NULL);


	t_matrix matrix;

	//matrix dimention read
	size_t count=fread(&(matrix.rows), sizeof(unsigned int), 1, f_matr);
	count+=fread(&(matrix.cols), sizeof(unsigned int), 1, f_matr);
	assert(count==2);

	//vector dimension read
	t_vector vector;
	count=fread(&(vector.count), sizeof(unsigned int), 1, f_vect);
	assert(count==1);


	//check1: square matrix
	if(matrix.rows!=matrix.cols){
		fclose(f_matr);
		fclose(f_vect);
		printf("The matrix is not antisymmetric, #rows!=#cols\n");
		exit(0);
	}


	double dim=matrix.rows*matrix.rows;
	matrix.elements=malloc(sizeof(double)*dim);
	assert(matrix.elements!=NULL);

	count=fread(matrix.elements, sizeof(double), dim, f_matr);
	fclose(f_matr);
	assert(count==dim);

	vector.elements=malloc(sizeof(double)*vector.count);
	assert(vector.elements!=NULL);

	count=fread(vector.elements, sizeof(double), vector.count, f_vect);
	fclose(f_vect);
	assert(count=vector.count);

	if(strcmp(argv[1], "naif")==0){
		//check2: antisymmetry
		if(!verifyAntiSymmetry(matrix)){
			printf("The matrix is not antisymmetric\n");
			free(matrix.elements);
			free(vector.elements);
			exit(0);
		}else{
			printf("The matrix is antisymmetric\n");
		}

		//check3: multiplicability
		assert(matrix.cols==vector.count);
		printf("Norm: %e\n", calcNorm(matrix, vector));

	}else if(strcmp(argv[1], "singlepass")==0){
		assert(matrix.cols==vector.count);
		double norm;

		if(!verifyAntiSymmetryCalcNorm(&norm, matrix, vector)){
			printf("The matrix is not antisymmetric\n");
			free(matrix.elements);
			free(vector.elements);
			exit(0);
		}else{
			printf("The matrix is antisymmetric\n");
			printf("Norm: %e\n", norm);
		}
	}
	
	free(matrix.elements);
	free(vector.elements);
	return 0;
}
