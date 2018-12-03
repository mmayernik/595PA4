#include "zst.h"



//Functions to generate trees
node * Neighbor_Build(node * * Node_Array, int length,double r_unit, double c_unit){
	//int key = length;
	//Find the nearest neighbors to all nodes and put in a heap
	heap_node * * Heap_Array = find_neighbors(Node_Array, length,r_unit,c_unit);	//Likely works

	int heap_size = length;
	int Tree_size = length*2 - 1;
	int curr_key = length;

	//Temp pointer 
	node * new_node;
	//Main Loop
	heap_node * Extracted_Node;

	//Now the fun begins. Initialize the G-Set
	int * Graph = (int*)malloc(sizeof(int)*Tree_size);


	//Initialize Graph
	for(int i = 0; i < Tree_size; i++){
		if(i < heap_size){
			Graph[i] = 1;

		} else{
			Graph[i] = 0;
		}
	}


	
	while(heap_size > 1){
		//printf("Size Heap: %d\n",heap_size );
		//Extracted_Node
		Extracted_Node = Extract_Min(Heap_Array, heap_size - 1);
		heap_size--;


		//printf("Extracted_Node: (Node A: label %d | key: %d) (Node B: label %d | key: %d) (Cost:%le)\n",Extracted_Node->node_A->label,Extracted_Node->node_A->key,Extracted_Node->node_B->label,Extracted_Node->node_B->key,Extracted_Node->cost );
		//Check if nodes are in G'
		
		if(Graph[Extracted_Node->node_B->key] == 1 && Graph[Extracted_Node->node_A->key] == 1){
			//Merge into new node
			new_node = Initialize_Node(curr_key, Extracted_Node->node_A, Extracted_Node->node_B,r_unit, c_unit);
			//printf("NEW node: %d with %d to be %d\n",new_node->left_node->key,new_node->right_node->key,new_node->key );
			//printf("New_Node: Cost: %le\n",new_node->cost );
			Graph[curr_key] = 1;
			Graph[Extracted_Node->node_A->key] = 0;
			Graph[Extracted_Node->node_B->key] = 0;
			Node_Array[curr_key] = new_node;
			//printf("Final Node[%d]: A[%d] B[%d]\n",curr_key,new_node->left_node->key,new_node->right_node->key );
			curr_key++;
			if(curr_key == Tree_size){
				//printf("Final Node[%d]: A[%d] B[%d]\n",curr_key-1,new_node->left_node->key,new_node->right_node->key );
				break;
			}
		}else{
			if(Graph[Extracted_Node->node_A->key] == 1){
				//printf("Node_A: %d\n",Extracted_Node->node_A->key );
				new_node = Extracted_Node->node_A;
			}
			else if(Graph[Extracted_Node->node_B->key]==1){
				//printf("Node_B: %d\n",Extracted_Node->node_B->key );
				new_node = Extracted_Node->node_B;
			}else{
				continue;
			}

		}
		
		//Calculate the nearest neighbor of the current node
		node * node_prime;

		double cost_v = Calculate_Nearest_Neighbor(Node_Array,Graph,new_node,&node_prime,0,curr_key-1,r_unit, c_unit);
		//
		Heap_Array[heap_size]->node_A = new_node;
		Heap_Array[heap_size]->node_B = node_prime;
		Heap_Array[heap_size]->cost = cost_v;
		Insert_Heap(Heap_Array, heap_size);
		heap_size++;

		

	}
	for (int i = 0; i< length; i++){
		free(Heap_Array[i]);
	}
	free(Heap_Array);
	//Create final node
	//printf("Final Heap to merge:(Node A: label %d | key: %d) (Node B: label %d | key: %d) (Cost:%le)\n",Heap_Array[0]->node_A->label,Heap_Array[0]->node_A->key,Heap_Array[0]->node_B->label,Heap_Array[0]->node_B->key,Heap_Array[0]->cost);
	//Extracted_Node = Heap_Array[0];
	//new_node = Initialize_Node(curr_key, Extracted_Node->node_A, Extracted_Node->node_B,r_unit, c_unit);
	//Node_Array[curr_key] = new_node;
	//printf("Extracted_Node: (Node A: label %d | key: %d | Cost: %le) (Node B: label %d | key: %d | Cost: %le) (Cost:%le)\n",Extracted_Node->node_A->label,Extracted_Node->node_A->key,Extracted_Node->node_A->cost,Extracted_Node->node_B->label,Extracted_Node->node_B->key,Extracted_Node->node_B->cost,Extracted_Node->cost );
	//printf("Final Heap: %le \n",new_node->cost );


	//Deallocate Heap


	//printf("Extracted_Node: Cost: %le\n",Heap_Array[0]->cost);
	free(Graph);

	//Print_Heap(Heap_Array,0,length);
	return Node_Array[curr_key-1];

}



node * Merge_Build(node * * Node_Array, int lb, int ub, int * i){
	//printf("lb: %d ub: %d\n",lb,ub );
	if(lb >= ub){
			//Allocate space for Manhattan Arc Structure
		return Node_Array[ub];
	}
	//printf("Created Middle Node!!!\n");
	node * curr_node = (node *)malloc(sizeof(node));
	curr_node->Man_Arc = (M_ARC *)malloc(sizeof(M_ARC));
	curr_node->Man_Arc->y_intercepts = (double *)malloc(sizeof(double)*4);
	


/*
	int key;
	int label;
	double cost;
	int isLeaf;
	double x_co;
	double y_co;
	double e_a;
	double e_b;
	double capacitance;
	double T_delay;

*/
	curr_node->key = ++*i;	//Update Key
	curr_node->label = -1;
	curr_node->isLeaf = 0;
	curr_node->Man_Arc->y_intercepts[0] = 0.0;
	curr_node->Man_Arc->y_intercepts[1] = 0.0;
	curr_node->Man_Arc->y_intercepts[2] = 0.0;
	curr_node->Man_Arc->y_intercepts[3] = 0.0;
	curr_node->cost = 0;


	int m = lb + (ub-lb)/2;


	//Simply merge two trees
	curr_node->left_node = Merge_Build(Node_Array, lb, m, i);
	curr_node->right_node = Merge_Build(Node_Array, m+1,ub, i);
	


	//Consider adding code here to calculate tree as it goes up. Also, simulated annealing....

	return curr_node;
}

node * * Convert_File_to_Node_Array_PA2(FILE * Tree_File,int * length, double * r_out, double * r_unit, double * c_unit){
	
	//int length = 0;
	char c;
	//printf("Size of file: %d\n",*length);
	
	fseek(Tree_File,0,SEEK_SET);
	//Get number of lines in file
	for(c = getc(Tree_File); c != EOF; c = getc(Tree_File)){
		if(c == '\n'){
		*length = *length+1;
		}
		//printf("%c",c);
	}
	//RESET file
	fseek(Tree_File,0,SEEK_SET);

	//Get unit lengths
	fscanf(Tree_File,"%le %le %le\n",r_out,r_unit,c_unit);

	int string_arr_size = *length - 1;
	
	int loc;

	//Allocated array of strings
	node * * Node_Array = (node * * )malloc(string_arr_size * sizeof(node *));
	//char * line;
	for(int i = 0; i < string_arr_size; i++){
		Node_Array[i] = (node *)malloc(sizeof(node));
		loc = ftell(Tree_File);
		if(getc(Tree_File) == '('){
			//This is not a leaf node
			fseek(Tree_File,loc,SEEK_SET);
			fscanf(Tree_File,"()\n");
			//printf("Node_Array[%d]: Left Length = %le Right Length = %le\n",i,Node_Array[i]->left_length,Node_Array[i]->right_length);
			Node_Array[i]->label = -1;
			Node_Array[i]->T_delay = 0.0;
			Node_Array[i]->y_co = 0.0;
			Node_Array[i]->x_co = 0.0;
		} else {
			//This is a leaf node
			fseek(Tree_File,loc,SEEK_SET);
			fscanf(Tree_File,"%d(%le %le %le)\n",&Node_Array[i]->label,&Node_Array[i]->capacitance,&Node_Array[i]->x_co,&Node_Array[i]->y_co);
			//printf("Node_Array[%d]: Key = %d Capacitance = %le\n",i,Node_Array[i]->key,Node_Array[i]->capacitance);
			
		}

		Node_Array[i]->Man_Arc = (M_ARC *)malloc(sizeof(M_ARC));
		Node_Array[i]->Man_Arc->y_intercepts = (double *)malloc(sizeof(double)*4);

	}

	return Node_Array;
}


double Calculate_Nearest_Neighbor(node * * Node_Array,int * Graph, node * new_node, node * * node_prime,int low_bound,int high_bound,double r_unit, double c_unit){
	//NOT EFFICIENT
	double curr_cost = DBL_MAX;
	double temp_cost;
	for(int i = low_bound; i < high_bound; i++){
		
		if(Graph[i] && (i != new_node->key)){
			temp_cost = Calc_Cost_NonDestructive(Node_Array[i],new_node,r_unit,c_unit);
			//printf("Temp_cost[%d][%d]: %le\n",i,new_node->key,temp_cost);
			if (temp_cost < curr_cost)
			{
				//Change current candidate to this pair
				*node_prime = Node_Array[i];
				curr_cost = temp_cost;
			}
				
		}
	}

	return curr_cost;

	

}
//Creates a node and calculates the cost
node * Initialize_Node(int key, node * Node_A, node * Node_B,double r_unit, double c_unit){
	node * curr_node = (node *)malloc(sizeof(node));
	curr_node->Man_Arc = (M_ARC *)malloc(sizeof(M_ARC));
	curr_node->Man_Arc->y_intercepts = (double *)malloc(sizeof(double)*4);
	
	curr_node->label = -1;
	curr_node->key = key;	//Update Key
	curr_node->isLeaf = 0;
	curr_node->Man_Arc->y_intercepts[0] = 0.0;
	curr_node->Man_Arc->y_intercepts[1] = 0.0;
	curr_node->Man_Arc->y_intercepts[2] = 0.0;
	curr_node->Man_Arc->y_intercepts[3] = 0.0;
	curr_node->cost = 0;
	curr_node->left_node = Node_A;
	curr_node->right_node = Node_B;

	Calculate_Balancing_Point(curr_node, r_unit,c_unit);

	return curr_node;
}

heap_node * Extract_Min(heap_node * * Heap_Array, int max_index){
	
	Swap_Heap(&Heap_Array[0],&Heap_Array[max_index]);
	
	Min_Heapify(Heap_Array,max_index,0);

	return Heap_Array[max_index];
	
}

void Min_Heapify(heap_node * * Heap_Array, int max_index,int curr_node) { 
    int l = 2*curr_node+1; 
    int r = 2*curr_node+2; 
    int smallest = curr_node; 
    if (l < max_index && Heap_Array[l]->cost < Heap_Array[curr_node]->cost) 
        smallest = l; 
    if (r < max_index && Heap_Array[r]->cost < Heap_Array[smallest]->cost) 
        smallest = r; 
    if (smallest != curr_node) 
    { 
        Swap_Heap(&Heap_Array[curr_node], &Heap_Array[smallest]); 
        Min_Heapify(Heap_Array,max_index,smallest); 
    } 
} 

void Insert_Heap(heap_node * * Heap_Array, int max_index){


	int curr_node = max_index;
	int parent = (curr_node - 1) / 2;
	while((curr_node != 0) && ((Heap_Array[parent]->cost) > (Heap_Array[curr_node]->cost))){
		//printf("Swapping: %d | %d\n",parent, curr_node );
		Swap_Heap(&Heap_Array[parent],&Heap_Array[curr_node]);
		curr_node = parent;
		parent = (curr_node - 1) / 2;
	}



}



void Swap_Heap(heap_node * * parent, heap_node * * curr_node){

	heap_node * temp_heap_node = *parent;

	*parent = *curr_node;
	*curr_node = temp_heap_node;



}





void Read_Binary_File_to_Output_File(FILE * Tree_File,FILE * Output_File){
	
	

	//Allocated array of strings
	//node * * Node_Array = (node * * )malloc((string_arr_size*2-1) * sizeof(node *));
	//char * line;
	int label;
	double capacitance;
	double e_a;
	double e_b;
	double x_co;
	double y_co;

	while(1 == fread(&label,sizeof(int),1,Tree_File)){

		if(label == -1){
			fread(&e_a,sizeof(double),1,Tree_File);
			fread(&e_b,sizeof(double),1,Tree_File);
			fread(&x_co,sizeof(double),1,Tree_File);
			fread(&y_co,sizeof(double),1,Tree_File);
			fprintf(Output_File,"(%le %le %le %le)\n",e_a,e_b,x_co,y_co);
		}else{
			fread(&capacitance,sizeof(double),1,Tree_File);
			fread(&x_co,sizeof(double),1,Tree_File);
			fread(&y_co,sizeof(double),1,Tree_File);
			fprintf(Output_File,"%d(%le %le %le)\n",label,capacitance,x_co,y_co);

		}

	}

}







node * * Convert_File_to_Node_Array(FILE * Tree_File,int * length, double * r_out, double * r_unit, double * c_unit){
	
	//int length = 0;
	char c;
	//printf("Size of file: %d\n",*length);
	
	fseek(Tree_File,0,SEEK_SET);

	//Get number of lines in file
	for(c = getc(Tree_File); c != EOF; c = getc(Tree_File)){
		if(c == '\n'){
		*length = *length+1;
		}
		//printf("%c",c);
	}
	//RESET file
	fseek(Tree_File,0,SEEK_SET);

	//Get unit lengths
	fscanf(Tree_File,"%le %le %le\n",r_out,r_unit,c_unit);

	int string_arr_size = *length - 1;

	//Allocated array of strings
	node * * Node_Array = (node * * )malloc((string_arr_size*2-1) * sizeof(node *));
	//char * line;
	for(int i = 0; i < string_arr_size; i++){
		Node_Array[i] = (node *)malloc(sizeof(node));
		fscanf(Tree_File,"%d(%le %le %le)\n",&Node_Array[i]->label,&Node_Array[i]->capacitance,&Node_Array[i]->x_co,&Node_Array[i]->y_co);
		Node_Array[i]->key = i;
		//Node_Array[i]->
		Node_Array[i]->T_delay = 0.0;
		Node_Array[i]->isLeaf = 1;
		Node_Array[i]->left_node = NULL;
		Node_Array[i]->right_node = NULL;
		Node_Array[i]->cost = 0.0;

		Node_Array[i]->Man_Arc = (M_ARC *)malloc(sizeof(M_ARC));
		Node_Array[i]->Man_Arc->y_intercepts = (double *)malloc(sizeof(double)*4);

		//Initialize Intercepts
		Node_Array[i]->Man_Arc->y_intercepts[0] = Node_Array[i]->y_co - Node_Array[i]->x_co;
		Node_Array[i]->Man_Arc->y_intercepts[1] = Node_Array[i]->Man_Arc->y_intercepts[0];

		Node_Array[i]->Man_Arc->y_intercepts[2] = Node_Array[i]->y_co + Node_Array[i]->x_co;
		Node_Array[i]->Man_Arc->y_intercepts[3] = Node_Array[i]->Man_Arc->y_intercepts[2];



		//printf("%d(%le %le %le)\n",Node_Array[i]->key,Node_Array[i]->capacitance,Node_Array[i]->x_co,Node_Array[i]->y_co);
	}

	return Node_Array;
}

/*
node * * Convert_Binary_to_Node_Array(FILE * Tree_File,int * length, double * r_out, double * r_unit, double * c_unit){
	
	//int length = 0;
	char c;
	//printf("Size of file: %d\n",*length);
	
	fseek(Tree_File,0,SEEK_SET);

	//Get number of lines in file
	for(c = getc(Tree_File); c != EOF; c = getc(Tree_File)){
		if(c == '\n'){
		*length = *length+1;
		}
		//printf("%c",c);
	}
	//RESET file
	fseek(Tree_File,0,SEEK_SET);

	//Get unit lengths
	//fscanf(Tree_File,"%le %le %le\n",r_out,r_unit,c_unit);

	int string_arr_size = *length - 1;

	//Allocated array of strings
	node * * Node_Array = (node * * )malloc((string_arr_size*2-1) * sizeof(node *));
	//char * line;
	for(int i = 0; i < string_arr_size; i++){
		Node_Array[i] = (node *)malloc(sizeof(node));
		fscanf(Tree_File,"%d(%le %le %le)\n",&Node_Array[i]->label,&Node_Array[i]->capacitance,&Node_Array[i]->x_co,&Node_Array[i]->y_co);
		Node_Array[i]->key = i;
		Node_Array[i]->T_delay = 0.0;
		Node_Array[i]->isLeaf = 1;
		Node_Array[i]->left_node = NULL;
		Node_Array[i]->right_node = NULL;
		Node_Array[i]->cost = 0.0;
		Node_Array[i]->Man_Arc = (M_ARC *)malloc(sizeof(M_ARC));
		Node_Array[i]->Man_Arc->y_intercepts = (double *)malloc(sizeof(double)*4);

		//Initialize Intercepts
		Node_Array[i]->Man_Arc->y_intercepts[0] = Node_Array[i]->y_co - Node_Array[i]->x_co;
		Node_Array[i]->Man_Arc->y_intercepts[1] = Node_Array[i]->Man_Arc->y_intercepts[0];

		Node_Array[i]->Man_Arc->y_intercepts[2] = Node_Array[i]->y_co + Node_Array[i]->x_co;
		Node_Array[i]->Man_Arc->y_intercepts[3] = Node_Array[i]->Man_Arc->y_intercepts[2];



		//printf("%d(%le %le %le)\n",Node_Array[i]->key,Node_Array[i]->capacitance,Node_Array[i]->x_co,Node_Array[i]->y_co);
	}

	return Node_Array;
}*/

int print_test_PA2(node * root,FILE * output_file){
	if(root == NULL)
		return 0;
	if(root->label == -1){
		print_test_PA2(root->left_node,output_file);
		print_test_PA2(root->right_node,output_file);
		fprintf(output_file,"()\n");

	} else{
		fprintf(output_file,"%d(%le %le %le)\n",root->label,root->capacitance,root->x_co,root->y_co);
		return 0;

	}
		return 1;
}


int print_postorder(node * root,FILE * output_file){
	if(root == NULL)
		return 0;
	if(root->label == -1){
		print_postorder(root->left_node,output_file);
		print_postorder(root->right_node,output_file);
		fprintf(output_file,"(%le %le %le %le)\n",root->e_a,root->e_b,root->x_co,root->y_co);


	} else{
		fprintf(output_file,"%d(%le %le %le)\n",root->label,root->capacitance,root->x_co,root->y_co);
		return 0;

	}
		return 1;
}
int print_final_order(node * root,FILE * output_file){
	if(root == NULL){
		return 0;
	}
	int neg_1 = -1;
	if(root->label == -1){
		print_final_order(root->left_node,output_file);
		print_final_order(root->right_node,output_file);
		//fprintf(output_file,"(%le %le %le %le)\n",root->e_a,root->e_b,root->x_co,root->y_co);
		fwrite(&neg_1,sizeof(int),1,output_file);
		fwrite(&root->e_a,sizeof(double),1,output_file);
		fwrite(&root->e_b,sizeof(double),1,output_file);
		fwrite(&root->x_co,sizeof(double),1,output_file);
		fwrite(&root->y_co,sizeof(double),1,output_file);

	} else{
		//fprintf(output_file,"%d(%le %le %le)\n",root->label,root->capacitance,root->x_co,root->y_co);
		fwrite(&root->label,sizeof(int),1,output_file);
		fwrite(&root->capacitance,sizeof(double),1,output_file);
		fwrite(&root->x_co,sizeof(double),1,output_file);
		fwrite(&root->y_co,sizeof(double),1,output_file);
		return 0;

	}
		return 1;
}

node * Create_Tree(node * * Node_Array,int * i){

	//Root is equal to the current index
	//printf("i:%d\n",*i);
	int temp_i = *i;
	if(Node_Array[*i]->label != -1){
		//printf("FIND THINGSi:%d\n",*i);
		Node_Array[*i]->T_delay = 0;
		Node_Array[*i]->right_node = NULL;
		Node_Array[*i]->left_node = NULL;
	}
	else {

		*i = *i - 1;	
		//printf("i:%d\n",*i);
		Node_Array[temp_i]->right_node = Create_Tree(Node_Array,i);
		*i = *i - 1;
		//printf("i:%d\n",*i);
		Node_Array[temp_i]->left_node = Create_Tree(Node_Array,i);
	}

	return Node_Array[temp_i];
}

void Tree_Destroy(node * Tree)
{
	//Base case
    	if(Tree == NULL)
    		return;
    //Destroy left and right trees
    Tree_Destroy(Tree->left_node);
    Tree_Destroy(Tree->right_node);
	free(Tree->Man_Arc->y_intercepts);
    free(Tree->Man_Arc);
	free(Tree);
   
 
}

//Zero Skew Specific Functions

//Top Level Function to Calculate Locations of Manhattan Arcs. returns cost of tree in wirelength
double Populate_Manhattan_Arcs(node * root,double r_unit, double c_unit){

	//Allocate space for Manhattan Arc Structure
	
	//Check if node sink. If so, set vector = 0 and location of manhattan arc equal to the sink location
	if(root->label != -1){

		//Initialize Intercepts
		root->Man_Arc->y_intercepts[0] = root->y_co - root->x_co;
		root->Man_Arc->y_intercepts[1] = root->Man_Arc->y_intercepts[0];

		root->Man_Arc->y_intercepts[2] = root->y_co + root->x_co;
		root->Man_Arc->y_intercepts[3] = root->Man_Arc->y_intercepts[2];

		//printf("\nKey:%d, B2: %le B0: %le",root->key,root->Man_Arc->y_intercepts[2],root->Man_Arc->y_intercepts[0]);
		
	return 0.0; 
	}
	
	//Calculate bottom up
	Populate_Manhattan_Arcs(root->left_node,r_unit,c_unit);
	Populate_Manhattan_Arcs(root->right_node,r_unit,c_unit);

	//Calculate Manhattan Arc
	Calculate_Balancing_Point(root, r_unit, c_unit);		//Equivalent to merge(root)

	
	return root->e_a + root->e_b + root->cost;
	
}

void Calculate_Locations(node * root, double  * V_box){
//Check if node sink. If so, set vector = 0 and location of manhattan arc equal to the sink location
	if(root->label != -1){
	return; }

	root->y_co = (V_box[0] + V_box[3]) / 2.0;
	root->x_co = root->y_co - V_box[0];

	double V_arr[4];
	double A_arr[4];

	A_arr[0] = V_box[0] - root->e_a;
	A_arr[1] = V_box[0] + root->e_a;

	A_arr[2] = V_box[3] - root->e_a;
	A_arr[3] = V_box[3] + root->e_a;


	Calculate_Manhattan_Arc(A_arr,root->left_node->Man_Arc->y_intercepts,V_arr);
	
	//Calculate bottom up
	Calculate_Locations(root->left_node,V_arr);


	A_arr[0] = V_box[0] - root->e_b;
	A_arr[1] = V_box[0] + root->e_b;

	A_arr[2] = V_box[3] - root->e_b;
	A_arr[3] = V_box[3] + root->e_b;

	Calculate_Manhattan_Arc(A_arr,root->right_node->Man_Arc->y_intercepts,V_arr);

	Calculate_Locations(root->right_node,V_arr);



	
	return;

}

double Calc_Cost_NonDestructive(node * Node_A, node * Node_B, double r_unit,double c_unit){

	double L = Min_Manhattan_Arcs(Node_A, Node_B);

	double x;
	double temp_1,temp_2,a,b,c,e_a,e_b;

	//printf("\nNode: %d | Shortest Length: %le",root->key, L); 
	
	x = ((Node_B->T_delay - Node_A->T_delay) + r_unit*L*(Node_B->capacitance + c_unit*L/2.0)) / (r_unit * L * (c_unit * L + Node_B->capacitance + Node_A->capacitance));
	
		
	if((L != 0.0) && (x <= 1) && (x >= 0)) {
		//printf("L: %le\n",L );

		e_a = x * L;
		e_b = (1-x) * L;
		
		//root->T_delay = r_unit * e_b * (Node_B->capacitance + c_unit * root->e_b / 2.0) + Node_B->T_delay;
		//printf("\nNode: %d | Shortest Length: %le | X: %le | Delay A: %le | Delay B: %le",root->key, L,x,r_unit * root->e_a * (Node_A->capacitance + c_unit * root->e_a / 2) + Node_A->T_delay,r_unit * root->e_b * (Node_B->capacitance + c_unit * root->e_b / 2) + Node_B->T_delay); 

	} 
	//Check Detouring
	else if(Node_B->T_delay < Node_A->T_delay) {
		//This implies V is at A with slope of A
		e_a = 0.0;
		a = r_unit*c_unit/2.0;
		b = r_unit*Node_B->capacitance;
		c = Node_B->T_delay - Node_A->T_delay;
		temp_1 = ((-1 * b) + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
		temp_2 = ((-1 * b) - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);

		//printf("\nTemp1: %le Temp2: %le a: %le b: %le c: %le Delay A: %le Delay B:%le\n",temp_1,temp_2,a,b,c,Node_A->T_delay,Node_B->T_delay);
		e_b = (temp_1 > temp_2) ? temp_1 : temp_2;
		
		

	} else {
		//This implies V is at B with slope of B
		a = r_unit*c_unit/2;
		b = r_unit*Node_A->capacitance;
		c = Node_A->T_delay - Node_B->T_delay;
		
		temp_1 = ((-1 * b) + sqrt(b * b - 4 * a * c)) / (2 * a);
		temp_2 = ((-1 * b) - sqrt(b * b - 4 * a * c)) / (2 * a);
		e_a = (temp_1 > temp_2) ? temp_1 : temp_2 ;
		//printf("\nTemp1: %le Temp2: %le a: %le b: %le c: %le Delay A: %le Delay B:%le\n",temp_1,temp_2,a,b,c,Node_A->T_delay,Node_B->T_delay);
		e_b = 0.0;
		//T_delay = Node_B->T_delay;
//printf("\nNode: %d | Shortest Length: %le | X: %le",root->key, L,x); 
	}

	return e_b + Node_A->cost + e_a + Node_B->cost;
}

//Calculates the lengths e_a and e_b. Then, depending on the situation it will 
void Calculate_Balancing_Point(node * root, double r_unit, double c_unit){


	node * Node_A = root->left_node;
	node * Node_B = root->right_node;

	//Find minimum distance between manhattan arc A and manhattan arc B
	double L = Min_Manhattan_Arcs(Node_A, Node_B);

	double x;
	double temp_1,temp_2,a,b,c;

	//printf("\nNode: %d | Shortest Length: %le",root->key, L); 
	
	x = ((Node_B->T_delay - Node_A->T_delay) + r_unit*L*(Node_B->capacitance + c_unit*L/2.0)) / (r_unit * L * (c_unit * L + Node_B->capacitance + Node_A->capacitance));
	
		
	if((L != 0.0) && (x <= 1) && (x >= 0)) {
		//printf("L: %le\n",L );

		root->e_a = x * L;
		root->e_b = (1-x) * L;
		
		root->T_delay = r_unit * root->e_b * (Node_B->capacitance + c_unit * root->e_b / 2.0) + Node_B->T_delay;
		//printf("\nNode: %d | Shortest Length: %le | X: %le | Delay A: %le | Delay B: %le",root->key, L,x,r_unit * root->e_a * (Node_A->capacitance + c_unit * root->e_a / 2) + Node_A->T_delay,r_unit * root->e_b * (Node_B->capacitance + c_unit * root->e_b / 2) + Node_B->T_delay); 

	} 
	//Check Detouring
	else if(Node_B->T_delay < Node_A->T_delay) {
		//This implies V is at A with slope of A
		root->e_a = 0.0;
		a = r_unit*c_unit/2.0;
		b = r_unit*Node_B->capacitance;
		c = Node_B->T_delay - Node_A->T_delay;
		temp_1 = ((-1 * b) + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
		temp_2 = ((-1 * b) - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);

		//printf("\nTemp1: %le Temp2: %le a: %le b: %le c: %le Delay A: %le Delay B:%le\n",temp_1,temp_2,a,b,c,Node_A->T_delay,Node_B->T_delay);
		root->e_b = (temp_1 > temp_2) ? temp_1 : temp_2;
		root->T_delay = Node_A->T_delay;
		

	} else {
		//This implies V is at B with slope of B
		a = r_unit*c_unit/2;
		b = r_unit*Node_A->capacitance;
		c = Node_A->T_delay - Node_B->T_delay;
		
		temp_1 = ((-1 * b) + sqrt(b * b - 4 * a * c)) / (2 * a);
		temp_2 = ((-1 * b) - sqrt(b * b - 4 * a * c)) / (2 * a);
		root->e_a = (temp_1 > temp_2) ? temp_1 : temp_2 ;
		//printf("\nTemp1: %le Temp2: %le a: %le b: %le c: %le Delay A: %le Delay B:%le\n",temp_1,temp_2,a,b,c,Node_A->T_delay,Node_B->T_delay);
		root->e_b = 0.0;
		root->T_delay = Node_B->T_delay;
//printf("\nNode: %d | Shortest Length: %le | X: %le",root->key, L,x); 
	}

	//Calculate Capacitance at root
	root->capacitance = (root->e_a * c_unit) + (root->e_b * c_unit)  + Node_B->capacitance + Node_A->capacitance;
	if(0){
		printf("-----------------------------Calculated Node[%d]---------------------------\n",root->key );

		printf("					Before Merge Locations\n");
		printf("A_Box: {3,2,1,0}:{%le,%le,%le,%le}\n",Node_A->Man_Arc->y_intercepts[3],Node_A->Man_Arc->y_intercepts[2],Node_A->Man_Arc->y_intercepts[1],Node_A->Man_Arc->y_intercepts[0]);
		printf("B_Box: {3,2,1,0}:{%le,%le,%le,%le}\n",Node_B->Man_Arc->y_intercepts[3],Node_B->Man_Arc->y_intercepts[2],Node_B->Man_Arc->y_intercepts[1],Node_B->Man_Arc->y_intercepts[0]);



		printf("Node A: Key = %d\n",Node_A->key);
		printf("Node B: Key = %d\n",Node_B->key);
		printf("L: %le\n",L );
		printf("X: %le\n",x );
		printf("A Branch Length: %le\n",root->e_a);
		printf("B branch Length: %le\n",root->e_b);

		printf("Root Intercepts = {3,2,1,0}:{%le,%le,%le,%le}\n",root->Man_Arc->y_intercepts[3],root->Man_Arc->y_intercepts[2],root->Man_Arc->y_intercepts[1],root->Man_Arc->y_intercepts[0] );
		}
	//Create Bounding Projections
	double A_box[4];
	double B_box[4];

	A_box[0] = Node_A->Man_Arc->y_intercepts[0] - root->e_a;
	A_box[1] = Node_A->Man_Arc->y_intercepts[1] + root->e_a;
	A_box[2] = Node_A->Man_Arc->y_intercepts[2] - root->e_a;
	A_box[3] = Node_A->Man_Arc->y_intercepts[3] + root->e_a;


	B_box[0] = Node_B->Man_Arc->y_intercepts[0] - root->e_b;
	B_box[1] = Node_B->Man_Arc->y_intercepts[1] + root->e_b;
	B_box[2] = Node_B->Man_Arc->y_intercepts[2] - root->e_b;
	B_box[3] = Node_B->Man_Arc->y_intercepts[3] + root->e_b;

	//Calculate Manhattan Arc
	Calculate_Manhattan_Arc(A_box,B_box,root->Man_Arc->y_intercepts);
	root->cost = root->e_a + root->left_node->cost + root->e_b + root->right_node->cost;


}

void Calculate_Manhattan_Arc(double * A_box, double * B_box, double * V_box){
	//Given B_box[0] B_box[1] A_box[0] A_box[1]

	double A_arr[2];
	double B_arr[2];
	double V_arr[2];

	A_arr[0] = A_box[0];
	B_arr[0] = B_box[0];
	A_arr[1] = A_box[1];
	B_arr[1] = B_box[1];

	Calc_M_Arc(A_arr,B_arr,V_arr);

	A_arr[0] = A_box[2];
	B_arr[0] = B_box[2];
	A_arr[1] = A_box[3];
	B_arr[1] = B_box[3];

	V_box[0] = V_arr[0];
	V_box[1] = V_arr[1];
	

	Calc_M_Arc(A_arr,B_arr,V_arr);

	V_box[2] = V_arr[0];
	V_box[3] = V_arr[1];
	if(0){
		printf("---------------------Merging Segment---------------\n");
		printf("A = {3,2,1,0}:{%le,%le,%le,%le}\n",A_box[3],A_box[2],A_box[1],A_box[0]);
		printf("B = {3,2,1,0}:{%le,%le,%le,%le}\n",B_box[3],B_box[2],B_box[1],B_box[0]);
		printf("V = {3,2,1,0}:{%le,%le,%le,%le}\n",V_box[3],V_box[2],V_box[1],V_box[0]);
	}
}

void Calc_M_Arc(double * A_box, double * B_box, double * V_box){
	//Implies A is in B
	if(((A_box[1] < B_box[1]) && (A_box[0] > B_box[0])				)){
		V_box[0] = A_box[0];
		V_box[1] = A_box[1];
	} 
	//Implies A is in B
	else if((		(A_box[1] > B_box[1]) && (A_box[0] < B_box[0])		)){
		V_box[0] = B_box[0];
		V_box[1] = B_box[1];
	}else if(abs(A_box[0] - B_box[1]) < abs(A_box[1] - B_box[0])		){
		V_box[0] = (A_box[0] < B_box[1]) ? A_box[0] : B_box[1];
		V_box[1] = (A_box[0] < B_box[1]) ? B_box[1] : A_box[0];
	} else {
		V_box[0] = (A_box[1] < B_box[0]) ? A_box[1] : B_box[0];
		V_box[1] = (A_box[1] < B_box[0]) ? B_box[0] : A_box[1];
	}

}

double Calc_Distance (double * intercepts_A, double * intercepts_B){
	if((intercepts_B[0] > intercepts_A[0] && intercepts_B[0] < intercepts_A[1]) || (intercepts_A[0] > intercepts_B[0] && intercepts_A[0] < intercepts_B[1])){
		return 0.0;
	}
	else{
		double B_to_A = abs(intercepts_B[0] - intercepts_A[1]);
		double A_to_B = abs(intercepts_A[0] - intercepts_B[1]);
		return (B_to_A) > (A_to_B) ? A_to_B: B_to_A ;
	}



}

double Min_Manhattan_Arcs(node * node_A, node * node_B){	
	int i;
	double A_arr[2];
	double B_arr[2];

	for (i = 0; i < 2; i++){
		A_arr[i] = node_A->Man_Arc->y_intercepts[i];
		B_arr[i] = node_B->Man_Arc->y_intercepts[i];
	}
	//printf("Intercepts: A[0] = %le A[1] = %le B[0] = %le B[1] = %le\n",A_arr[0],A_arr[1],intercepts_B[0],intercepts_B[1] );

	double Min_L = Calc_Distance(A_arr,B_arr);
	//printf("MinL: %le\n",Min_L );

	for (i = 2; i < 4; i++){
		A_arr[i-2] = node_A->Man_Arc->y_intercepts[i];
		B_arr[i-2] = node_B->Man_Arc->y_intercepts[i];
	}

	double Min_U = Calc_Distance(A_arr,B_arr);
	//printf("MinU: %le\n",Min_U );
	
	//printf("Min_L: %le Min_U: %le\n",Min_L,Min_U );


	//printf("\nMin Determined: %le, MinU: %le, MinL: %le",(Min_U > Min_L) ? Min_U : Min_L,Min_U,Min_L);
	return (Min_U > Min_L) ? Min_U : Min_L;


	

}

heap_node * * find_neighbors(node * * Node_Array, int length,double r_unit, double c_unit){
	int i,j;
	double curr_cost,temp_cost;
	node * Temp_Left_Node;
	node * Temp_Right_Node;
	//This is our heap
	heap_node * * Heap_Array = (heap_node * * )malloc((length) * sizeof(heap_node *));
	for (i = 0; i < length; i++){
		curr_cost = DBL_MAX;
		Heap_Array[i] = (heap_node * )malloc(sizeof(heap_node));
		for(j = 0; j < length; j++){
			if(i != j){
				temp_cost = Calc_Cost_NonDestructive(Node_Array[i],Node_Array[j],r_unit,c_unit);
				//printf("Temp_cost[%d][%d]: %le\n",i+1,j+1,temp_cost);
				if (temp_cost < curr_cost)
				{
					//Change current candidate to this pair
					Temp_Left_Node = Node_Array[i];
					Temp_Right_Node = Node_Array[j];
					curr_cost = temp_cost;
				}
			}
		}

		//Insert Heap_Array[i] into heap

		//Initialize Heap Variable
		Heap_Array[i]->cost = curr_cost;
		Heap_Array[i]->node_A = Temp_Left_Node;
		Heap_Array[i]->node_B = Temp_Right_Node;
		Heap_Array[i]->left_node = NULL;
		Heap_Array[i]->right_node = NULL;
		Insert_Heap(Heap_Array,i);
	}

	return Heap_Array;

}

void Print_Heap(heap_node * * Heap_Node, int key,int length){
	if(key > length-1){
		return;
	}
	printf("----------------------Heap Node-------------------\n");
	printf("Key: %d\n",key );
	printf("Heap Cost: %le \n",Heap_Node[key]->cost );
	printf("Node A :%d\n",Heap_Node[key]->node_A->key);
	printf("Node B :%d\n",Heap_Node[key]->node_B->key);

	Print_Heap(Heap_Node,key*2+1,length);
	Print_Heap(Heap_Node,key*2+2,length);

}



int min(int a, int b){
	if(a < b){	return a;}
	else{		return b;}
}

//Open Source Code 



/*


	//Calculate Delta Values
	int Delta_Y = (S1->y_co) - (S2->y_co);
	int Delta_X = (S1->x_co) - (S2->x_co);

	//Calculate the length of the arc
	int Manhattan_Length = abs(Delta_Y) + abs(Delta_X);

	//Calculate sign. This is the opposite of the sign of the slope between S1 and S2
	int sign = -(Delta_Y / Delta_X) / abs(Delta_Y / Delta_X);

	//Calculate the vector value of the arc. I.e. the manhattan magnitude of the arc
	int vector = min(abs(Delta_Y),abs(Delta_Y)) * sign; 

	//Calculate the balance point between two points





int print_elmore(node * root,FILE * output_file){
	
	if(root == NULL)
		return 0;
	if(root->key == -1){
		//fprintf(output_file,"(%le %le)\n",root->left_length,root->right_length);
		print_elmore(root->left_node,output_file);
		print_elmore(root->right_node,output_file);

	} else{
		fprintf(output_file,"%d(%le)\n",root->key);
		return 0;

	}
		return 1;
}*/

/*void calculate_RC(node * root, double r_unit, double c_unit, double parent_length){
	if(root->key != -1){
	root->capacitance = root->capacitance + (parent_length * c_unit/2.0f);
	//root->resistance = parent_length * r_unit;
	//printf("Node[%d]: Cap = %le\n",root->key,root->capacitance);

	return; 
	}
	calculate_RC(root->left_node,r_unit, c_unit,root->left_length);
	calculate_RC(root->right_node,r_unit, c_unit,root->right_length);
	
	root->capacitance = (root->left_length * c_unit/2.0f) + (root->right_length * c_unit/2.0f) + (parent_length* c_unit/2.0f) + root->left_node->capacitance + root->right_node->capacitance;
	//root->resistance = (root->left_length * r_unit) + (root->right_length * r_unit)+ root->left_node->resistance + root->right_node->resistance;
	

	return;

}

void calculate_Elmore(node * root, double delay,double parent_resistance,double r_unit){
	if(root != NULL){
		

	root->delay = delay + parent_resistance * root->capacitance;
	calculate_Elmore(root->left_node, root->delay,root->left_length*r_unit,r_unit);
	calculate_Elmore(root->right_node, root->delay,root->right_length*r_unit,r_unit);}

	
}*/



/*
node * Create_Tree(File * Tree_File){
	node * root = (node *)malloc(sizeof(node));
	root->key = 0;
	
	return root;
	
}*/

