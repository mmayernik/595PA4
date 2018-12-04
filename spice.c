#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include "spice.h"
//#include "merge_func.c"
#include <stdio.h>
#include <stdlib.h>

#include <sys/syscall.h>
#include <unistd.h>
/*
	Library for Verifying Designs using SPICE
	Goal: Using the output from our project, build a SPICE simulation that verifies our design meets requirements. If it doesn't meet requirements, then say why.

	1. Print Topology to a file
	2. Convert Topology into SPICE netlist using script from Professor Koh
	3. Run Spice Simulation
	4. Check values returned from SPICE
*/


/*
	Simulates the netlist and returns an array of doubles with the results. Which results are currently unspecified.
*/
#define FALLING_FILE "f.spice"

#define RISING_FILE "r.spice"

#define TEMP_LOCS "output.b"

#define WIRE_PARAM "wire.param"

#define TEST_OUTPUT_FILE "Simulation_Return.txt"

#define DEBUG_SPICE 1


double * simulate_netlist(){

	//1. Convert filename to spice netlist - Done
			char system_call[255];
			int status;
			sprintf(system_call,"./loc2spice %s %s %s %s",WIRE_PARAM,TEMP_LOCS, RISING_FILE, FALLING_FILE);
			

			if(DEBUG_SPICE){
				fprintf(stdout, "Beginning SPICE test-------------\n" );
				fprintf(stdout, "Step 1: Convert topology to SPICE\n" );
				//fprintf(stdout, "%s\n",system_call );


				status = system(system_call);


				fprintf(stdout, "Status return code: %d\n", status);
				if(status == 0){
					fprintf(stdout, "Passed SPICE conversion.\n" );
				} else {
					fprintf(stdout, "FAILED SPICE conversion.\n" );
				}
			} else {
				status = system(system_call);
			}

	//2. Run netlist. Rising is more important so only run that.
			//Run rising netlist

			sprintf(system_call,"ngspice -b %s > %s", RISING_FILE, TEST_OUTPUT_FILE );
			
			if(DEBUG_SPICE){
				fprintf(stdout, "Step 2: Run Simulation.\n" );
				fprintf(stdout, "%s\n",system_call );
				status = system(system_call);
				fprintf(stdout, "Status return code: %d\n", status);
			} else {
				status = system(system_call);
			}

	//3. Parse simulation
			FILE *result_file = fopen(TEST_OUTPUT_FILE,"r");


			if(DEBUG_SPICE){
				fprintf(stdout, "Step 3: Parse Simulation\n" );
				fprintf(stdout, "Current Draw: %le\n", 3.0f);
				//fprintf(stderr, "%s\n", );
				
				
			} else {
				status = system(system_call);
			}

			status = fclose(result_file);
			if(DEBUG_SPICE){
				fprintf(stdout, "File Pointer Close Result: %d\n",status);
			}

			return NULL;
}

int create_spice_files(char * filename){

	//syscall(SYS_write)

	return 0;
}

//Primary function to print the spice netlist from topology.
int print_spice_netlist(int * curr_label){

	//Update current node label 
	int label = *curr_label;


	return label;
}
