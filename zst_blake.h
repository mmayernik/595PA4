
#ifndef ZST_H
#define ZST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <math.h>
typedef struct Node{
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
	
	struct Node * left_node;
	struct Node * right_node;
	struct M_arc * Man_Arc;
} node;

typedef struct HEAP_NODE{
	struct Node * node_A;
	struct Node * node_B;
	struct HEAP_NODE * left_node;
	struct HEAP_NODE * right_node;
	double cost;
} heap_node;

typedef struct M_arc{
	double * y_intercepts;		//Y intercepts of Manhattan Arc
} M_ARC;

typedef struct Point{
	double x;
	double y;

}point;



//void calculate_RC(node * root, double r_unit, double c_unit, double parent_length);
node * * Convert_File_to_Node_Array_PA2(FILE * Tree_File,int * length, double * r_out, double * r_unit, double * c_unit);
int print_preorder(node * root,FILE * output_file);
int print_postorder(node * root,FILE * output_file);
int print_test_PA2(node * root,FILE * output_file);
void Read_Binary_File_to_Output_File(FILE * Tree_File,FILE * Output_File);
//int print_elmore(node * root,FILE * output_file);

//void calculate_Elmore(node * root, double delay,double parent_resistance,double r_unit);

//node * Create_Node(int key);

node * Create_Tree(node * * Node_Array,int * i);

//node * Create_Tree(FILE * Fp);

void Tree_Destroy(node * Tree);

node * * Convert_File_to_Node_Array(FILE * Tree_File,int * length, double * r_out, double * r_unit, double * c_unit);

double Populate_Manhattan_Arcs(node * root,double r_unit, double c_unit);
void Calculate_Manhattan_Arc(double * A_box, double * B_box, double * V_box);
void Calculate_Locations(node * root, double  * V_box);
double Min_Manhattan_Arcs(node * node_A, node * node_B);
void Calculate_Balancing_Point(node * root, double r_unit, double c_unit);
int Colinear_Intersection(point A1, point A2, point B1, point B2, point * R1, point * R2);
int print_final_order(node * root,FILE * output_file);
void Calc_M_Arc(double * A_box, double * B_box, double * V_box);


node * Neighbor_Build(node * * Node_Array, int length,double r_unit, double c_unit);
double Calc_Cost_NonDestructive(node * Node_A, node * Node_B,double r_unit,double c_unit);

double Calc_Distance (double * intercepts_A,double * intercepts_B);
heap_node * * find_neighbors(node * * Node_Array, int length,double r_unit, double c_unit);

//All method functions

node * Merge_Build(node * * Node_Array, int lb, int ub,int * key);

//node *  Convert_to_Node_Array(FILE * Tree_File,int * length, double * r_out, double r_unit, double c_unit);

//-------------------------------Helper Functions-----------------------
int min(int a, int b);

//HEAP FUNCTIONS
void Insert_Heap(heap_node * * Heap_Array, int max_index);
void Swap_Heap(heap_node * * parent, heap_node * * curr_node);
void Print_Heap(heap_node * * Heap_Node, int key,int length);
heap_node * Extract_Min(heap_node * * Heap_Array, int max_index);
void Min_Heapify(heap_node * * Heap_Array, int max_index,int curr_node);
double Calculate_Nearest_Neighbor(node * * Node_Array,int * Graph, node * new_node, node * * node_prime,int low_bound,int high_bound,double r_unit, double c_unit);

node * Initialize_Node(int key, node * Node_A, node * Node_B,double r_unit, double c_unit);
#endif
