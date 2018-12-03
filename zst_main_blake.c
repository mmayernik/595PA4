
#include "zst.h"

/* Game Plan
	1. Error Checking for Inputs	(Main Function Simple checks)
	2. File opening checking	(Open Function return value)
	3. Read file as post order traversal (Calculate as you go and save results)
	4. Create a computation tree	(Post order traversal from file)


*/

int main (int argc, char * argv[]){

	if(argc != 4){
	printf("FAILED");
	return EXIT_FAILURE;

	} //Error Checking 
	else {
		//Open file
		FILE * Tree_File_Pointer = fopen(argv[1],"r");
		FILE * O1_File_Pointer = fopen(argv[2],"w+");
		FILE * O2_File_Pointer = fopen(argv[3],"w+");
		//FILE * PA_2_FILE = fopen("PA_2_FILE.txt","w+");
		//Check if file opened properly
		if(Tree_File_Pointer == NULL){
			fprintf(stderr,"Unable to open file: %s",argv[1]);
			return EXIT_FAILURE;}

		double r_out;
	 	double r_unit;
 		double c_unit;

		int size = 0;
		int start_key;

		//node * * Node_Array = Convert_File_to_Node_Array_PA2(Tree_File_Pointer,&size,&r_out,&r_unit,&c_unit);
		//int i = size - 2;
		//node * root_neighbor = Create_Tree(Node_Array,&i);
		//Populate_Manhattan_Arcs(root_neighbor,r_unit,c_unit);


//-----------------Tree Now exists and is valid---------------
		//Create all Manhattan Arcs
		//Populate_Manhattan_Arcs(root,r_unit,c_unit);

		

		//Convert File to Node array
		node * * Node_Array = Convert_File_to_Node_Array(Tree_File_Pointer,&size,&r_out,&r_unit,&c_unit);
		
		//printf("Length: %d\n",size - 1);

		
		int length = size - 1;
		start_key = length;

		//node * root_neighbor = Merge_Build(Node_Array, 0, size - 2, &start_key);	//Return a complete tree

		//double cost_merge = 

		//Populate_Manhattan_Arcs(root_neighbor,r_unit,c_unit);

		//printf("Cost for Easy Merging O(V*log(V)): %le\n",cost_merge );



		node * root_neighbor = Neighbor_Build(Node_Array, length,r_unit,c_unit);

		Populate_Manhattan_Arcs(root_neighbor,r_unit,c_unit);

		//double cost_neighbor = root_neighbor->cost;

		//printf("Cost for Nearest Neighbor O(V^2*log(V)): %le\n",cost_neighbor );

		double * root_V = root_neighbor->Man_Arc->y_intercepts;
		Calculate_Locations(root_neighbor,root_V );





		//print_test_PA2(root_neighbor,PA_2_FILE);
		print_postorder(root_neighbor,O1_File_Pointer);
		print_final_order(root_neighbor,O2_File_Pointer);
		//fclose(O2_File_Pointer);
		//FILE * O4_File_Pointer = fopen(argv[3],"r");
		//FILE * Write_Binary = fopen("Write_Binary.txt","w+");
		//Read_Binary_File_to_Output_File(O4_File_Pointer,Write_Binary);

		Tree_Destroy(root_neighbor);
		fclose(Tree_File_Pointer);
		fclose(O1_File_Pointer);
		fclose(O2_File_Pointer);
		///fclose(PA_2_FILE);
		free(Node_Array);

		//This is for the 
		//print_postorder(root,O2_File_Pointer);
		/*
		//Tree_Destroy(root_merge);
		free(Node_Array);
		fclose(Tree_File_Pointer);
		fclose(O1_File_Pointer);
		fclose(O2_File_Pointer);
		*/
		//Tree_Destroy(root);
		//free(Node_Array);
		/*
		//Connect all the nodes in a BST
		int i = size - 2;
		node * root = Create_Tree(Node_Array,&i);
		


//-----------------Tree Now exists and is valid---------------
		//Create all Manhattan Arcs
		


//-----------------Manhattan Arcs are created-----------------
		Calculate_Locations(root);


//----------------------Print Trees---------------------------
		//Print Preorder Traversal 
		print_preorder(root,O1_File_Pointer);
		//Print Postorder Traversal 
		print_postorder(root,O2_File_Pointer);
		//Print final traversal
		print_final_order(root,O3_File_Pointer);


//--------------------Free Allocated Memory-------------------
		Tree_Destroy(root);
		free(Node_Array);
		fclose(Tree_File_Pointer);
		fclose(O1_File_Pointer);
		fclose(O2_File_Pointer);
		fclose(O3_File_Pointer);*/
	}

	
	
	return EXIT_SUCCESS;
}
