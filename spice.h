#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include "pa4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

//Spice Primary Functions

int custom_netlist(Node * root,double length_to_parent, int * size);

int create_spice_files(Node * root,double length_to_parent, int * size);

double simulate_netlist(Node * root, double length_to_parent);

//Spice Helper Functions

int print_spice_netlist(FILE* print_file, Node * root, char *  input_label, double length_to_parent, double r_unit, double c_unit, int * size);

void post_order_traversal(Node * root, double * array, int * idx, double * min, double * max);

int Spice_nelist_Koh();

double * parse_results(int size);

Node * Build_Tree();


