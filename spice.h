#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include "pa4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

//Spice Primary Functions

int custom_netlist(Node * root,double length_to_parent);

int create_spice_files(Node * root,double length_to_parent);

double * simulate_netlist(Node * root, double length_to_parent);

//Spice Helper Functions

int print_spice_netlist(FILE* print_file, Node * root, char *  input_label, double length_to_parent, double r_unit, double c_unit);

int Spice_nelist_Koh();

double * parse_results();

Node * Build_Tree();


