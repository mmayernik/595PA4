
#include "spice.h"
#include "pa4.h"
#include <float.h>

//#include "merge_func.c"

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
#define FEMTO (0.000000000000001)
#define FALLING_FILE "f.spice"

#define RISING_FILE "r.test.spice"

#define TEMP_LOCS "example.loc.b"

#define WIRE_PARAM "wire.param"

#define TEST_OUTPUT_FILE "Simulation_Return.txt"

#define DEBUG_SPICE 0

#define SALTY_BOI NULL

//Specify SPICE Data Structs and constants
#define SPICY_TEMP "* inverter characterization\n.options temp=75\n"

//Simulation Timing Constraints. Default to a total test time of 300ps and a time step of .001 nano seconds (1 pico second)
#define SPICY_TIME ".tran 0.001n 3.0n 0.0n 0.001n\n"

//Setup for VDD and input files
#define SPICY_SETUP ".include tuned_45nm_HP.pm\n.include clkinv0.subckt\nvdd vdd 0 1.000\n"

#define SPICY_INIT "vin n0 0 1.000 pwl(0n 0, 0.2n 0, 0.325n 1, 3.0n 1)\n"



/*
	The big-daddy main function for all your spice-y needs.
*/
double * simulate_netlist(Node * root, double length_to_parent){

	//1. Convert filename to spice netlist - Done
			char system_call[255];
			int status;
			double * results;
			

			if(DEBUG_SPICE){
				fprintf(stdout, "Beginning SPICE test-------------\n" );
				fprintf(stdout, "Step 1: Convert topology to SPICE\n" );

				status = create_spice_files(root, length_to_parent);

				fprintf(stdout, "Status return code: %d\n", status);

				//Status Code from System Call
				if(status == 0){
					fprintf(stdout, "Passed SPICE conversion. Spicy_boi is satisfied\n" );
				}
			} else {
				status = create_spice_files(root, length_to_parent);
			}

			//Print Error and return
			if(status != 0) {
					fprintf(stdout, "FAILED SPICE conversion.\n" );
					return NULL;
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

			if(DEBUG_SPICE){
				fprintf(stdout, "Step 3: Parse Simulation\n" );
				results = parse_results();
				
			} else {
				//status = system(system_call);
				results = parse_results();
			}

			
			

			return results;
}

/*
	Purpose: To create the SPICEy simulation files
	1. Write Header

*/
int create_spice_files(Node * root,double length_to_parent){

	return custom_netlist(root, length_to_parent);

}

int Spice_nelist_Koh(){

	char system_call[255];

	sprintf(system_call,"./loc2spice %s %s %s %s",WIRE_PARAM,TEMP_LOCS, RISING_FILE, FALLING_FILE);

	return system(system_call);
}

int custom_netlist(Node * root,double length_to_parent){

	//Open File
	FILE * Spicy_Boi = fopen(RISING_FILE,"w+"); //fopen(filename, "w+");

	

	if(Spicy_Boi == SALTY_BOI){
		printf("Unable to open 'Spicy_Boi'. Too salty. :( \n");
		return -1;
	}

	//Write Header
	fprintf(Spicy_Boi, SPICY_TEMP);
	fprintf(Spicy_Boi, SPICY_TIME);
	fprintf(Spicy_Boi, SPICY_SETUP);
	fprintf(Spicy_Boi, SPICY_INIT);

	//Initialize Source Inverter
	fprintf(Spicy_Boi, "xi_1 n0 ni_0 vdd inv0\n");
	fprintf(Spicy_Boi, ".ic v(n0)=0"

	print_spice_netlist(Spicy_Boi, root, "ni_0", length_to_parent, 1.0000000000e-04, 2.0000000000e-19);

	fprintf(Spicy_Boi, ".measure TRAN iavg AVG i(vdd) FROM=0.0n TO=3.0n\n.measure TRAN irms RMS i(vdd) FROM=0.0n TO=3.0n\n.end\n" );

	//Close File and Return Success
	fclose(Spicy_Boi);
	return 0;
}


//Creates a test tree to build spice simulation
Node * Build_Tree(){

	Node * source = create_node(0, 0, 0, 34.0f * FEMTO);

	Node * temp;

	Node * last;

	last = source;

	 

	
	/*
	for(int i = 1; i < 10; i++){
		
		last->wire_l = 1850000;
		last->wire_r = -1;
		last -> parallel = 0;
		temp = create_node(i, 0, 0, 34.0f * FEMTO);
		last->left = temp;
		last->polarity = 1;
		last = temp;
	}


		last->wire_l = 1850000;
		last->wire_r = -1;
		last -> parallel = 0;
		temp = create_node(i, 0, 0, 34.0f * FEMTO);
		last->left = temp;
		last->polarity = i;
		last
		last = temp;
*/
	

	return source;
}

//Given an output spice file from the MACRO, parse the results from the macro.
double * parse_results(){

	//Open Results file
	FILE * SPICY_BOI = fopen("Simulation_Return.txt", "r");

	//Initialize Double Array
	int n = 4;
	double * results = (double *)malloc(n * sizeof(double));
	char start_line[255];
	char ch;
	char d;
	double temp_max = 0.0f;
	double max_delay = 0.0f;
	double max_slew = 0.0f;
	double min_delay = DBL_MAX;


	//Search for iavg
	while((ch = getc(SPICY_BOI)) != EOF){
    	fseek(SPICY_BOI, -1, SEEK_CUR);
    	fscanf(SPICY_BOI, "%s\n",start_line);
    	//fprintf(stdout, "%s\n",start_line );
    	if(strcmp(start_line, "irms") == 0){
    		//Found it!
    		fscanf(SPICY_BOI, "= %le\n",&results[0]);
    		//fprintf(stdout, "Results: %le\n",results[0]);
    	}


    	d = start_line[5];
    	start_line[5] = '\0';
    	if(strcmp(start_line, "delay") == 0){
    		fscanf(SPICY_BOI, "= %le\n",&temp_max);
    		max_delay = (temp_max > max_delay) ? temp_max : max_delay;
		min_delay = (temp_max < min_delay) ? temp_max : min_delay;
    	}
    	start_line[4] = '\0';
    	if((strcmp(start_line, "slew") == 0)){ //|| (strcmp(start_line, "rslew") == 0) || (strcmp(start_line, "fslew") == 0)){
    		fscanf(SPICY_BOI, "= %le\n",&temp_max);
    		max_slew = (temp_max > max_slew) ? temp_max : max_slew;
    	}

    	//fscanf(SPICY_BOI, "%s\n",start_line);
    
   }
   fprintf(stdout, "Max Delay : %le\n",max_delay );
   fprintf(stdout, "Min Delay : %le\n", min_delay);
   fprintf(stdout, "Tree Skew : %le\n", max_delay - min_delay);
   fprintf(stdout, "Max Slew : %le\n",max_slew );

   	results[1] = max_delay;
	results[2] = min_delay;
   	results[3] = max_slew;
	


	/*Return Vector
	results[0] = iRMS
	results[1] = max_delay;
   	results[2] = max_slew;
	results[3] = max_delay - min_delay = tree skew

	*/
	fclose(SPICY_BOI);


	//Return results array
	return results;

}

//Primary function to print the spice netlist from topology. Build Later for optimizations.
int print_spice_netlist(FILE* print_file, Node * root, char *  input_label, double length_to_parent, double r_unit, double c_unit){
	char pass_label[255] = "";

	strcpy(pass_label, input_label);

	//Return if NULL
	if(root == NULL){
		return 0;
	}

	//Insert PI Model
	if(length_to_parent > 0.0f){
			//Pi model wire
			fprintf(print_file, "r%d %s n%d_1 %le\n", root->label, input_label, root->label, length_to_parent * r_unit);
			fprintf(print_file, "c%d_1 %s 0 %le \n", root->label, input_label,length_to_parent * c_unit);
			fprintf(print_file, "c%d_2 n%d_1 0 %le \n", root->label, root->label, length_to_parent * c_unit);
			
			sprintf(pass_label,"n%d_1", root->label);

	}

	//Is current node a sink?
	if((root -> parallel == 0) && (root -> left == NULL) && (root -> right == NULL)){
		//For Delay Measurement
		fprintf(print_file, "c%d_3 %s 0 %le \n", root->label, pass_label, root->c);
		fprintf(print_file, ".measure tran delay%d trig v(n0) val='0.5' cross=1 targ v(%s) val='0.5' cross=1\n",root -> label, pass_label );
		

	} 
	//Is current node a buffer?
	else if (root -> parallel > 0){

		for(int i = 0; i < root->parallel; i++){
				//Print the Inverters
				fprintf(print_file, "x%d_%d %s n%d_0 vdd inv0\n", root->label, i, pass_label, root->label);

			}
			
			fprintf(print_file,".ic v(n%d_1)=1\n",root->label);

			if(((root->polarity)% 2) == 1){
				fprintf(print_file, ".measure tran slew%d trig v(n%d_1) val='0.9' fall=1 targ v(n%d_1) val='0.1' fall=1\n",root->label,root->label,root->label );
			} else {
				fprintf(print_file, ".measure tran slew%d trig v(n%d_1) val='0.1' rise=1 targ v(n%d_1) val='0.9' rise=1\n",root->label,root->label,root->label );
			}
		
	} 
	//Current node must be a joint
	else {

		if(print_spice_netlist(print_file, root->left, pass_label,root->wire_l, r_unit, c_unit) != 0){
			return -1;
		}

		if(root->wire_r != -1.0f){
			if(print_spice_netlist(print_file, root->right, pass_label,root->wire_r, r_unit, c_unit) != 0){
				return -1;
			}
		}

		
	
	}
		/*
		Create Inverters
		
			Format: label input output vdd inv0 | example -> x1 nout0 nout1 vdd inv0
		*/

		/*

		Measurements

		Format: label input output vdd inv0 | example -> x1 nout0 nout1 vdd inv0

		*/

		/*
		Create Resistors

		Format: ('r' + label) input output resistance | example -> r6_3 n5_0 n6_3 4.166667e+01
		*/


		/*

		Create Capacitors

		Format: ('c' + label) input output capacitance | example -> cout2 nout2 0 34.5f

		*/

		/*

		Create Initial Conditions (Based on Polarity)

		Format: .ic v(node_label) = (polarity + initial_condition) % 2 | example -> .ic v(n0_0)=1

		*/



		/*

		Recursive Step


		*/


	return 0;
}
