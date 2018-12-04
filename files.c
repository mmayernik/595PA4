//Three input files
#include<stdlib.h>
#include<stdio.h>
#include "zst.h"
//First file is parameters of inv0

typedef struct _givens {
	double inv_input_cap; //from first input file
	double inv_output_cap; //from first input file
	double inv_output_res; //from first input file
	double res_per_u_length; //from second input file
	double cap_per_u_length; //from second input file
}givens;

givens* parameter_input_files(char* input1, char* input2){
	//I'm putting these in the same function because theyre all just givens so 
	//it makes sense to me to put them in a single struct
	//Shouldn't be difficult to separate if we choose to
	FILE * fp1 = fopen(input1, "r");
	if(fp1 == NULL){
		return NULL;
	}

	FILE * fp2 = fopen(input2, "r");
	if(fp2 ==NULL){
		fclose(fp1);
		return NULL;
	}

	givens* param = malloc(sizeof param);
	if(param == NULL){
		return NULL;
	}

	fscanf(fp1, "%le %le %le\n", &param -> inv_input_cap, &param -> inv_output_cap, &param -> inv_output_res);
	fscanf(fp2, "%le %le\n", &param -> res_per_u_length, &param -> cap_per_u_length);
	
	fclose(fp1);
	fclose(fp2);

	return param;
}



int main(int argc, char* argv[]){
	//main for testing should be able to just copy paste this into
	//our final main, function calls shouldnt change much
	
	if(argc < 3){ //CHANGE THIS ONCE OUTPUT FILES INCLUDED 
		return EXIT_FAILURE;
	}
	givens * parameters = parameter_input_files(argv[1], argv[2]);
	if(parameters == NULL){
		return EXIT_FAILURE;
	}
	free(parameters);
	return EXIT_SUCCESS;	
}
