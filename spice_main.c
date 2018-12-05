#include "spice.h"
#include "pa4.h"


int main(){

	//Test 2: Custom Netlist Test
	//Node * root = Build_Tree();

	double * results = parse_results();

	//int status = custom_netlist(root);

	//fprintf(stdout, "Status: %d\n",status);

	//printf("iRMS = %le\n",results[0]);

	free(results);

}
