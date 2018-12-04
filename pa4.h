#ifndef PA4_H
#define PA4_H
#include <stdio.h>

/*
  Node:
  Structure to represent a sink/merged point in the topology
 */
typedef struct _Node{
  int label;
  double x, y, wire_l, wire_r, c, t;
  double m_array[4];
  struct _Node * right;
  struct _Node * left;
  struct _Node * next;
  struct _HeapPoint * len_list; //hold list of all lens
  int lens;
  int merged;
  int polarity;
  int parallel;
} Node;

/*
  Heaper:
  Structure used in heap to represent merging cost
 */
typedef struct _Heaper{
  double tot_len;
  struct _Node * node1;
  struct _Node * node2;
  int loc;
} Heaper;

/*
  HeapPoint
  Structure stored in node so that an item in the heap
  can be located in O(1) time
 */
typedef struct _HeapPoint{ //goes in node's len_list
  struct _Heaper * heaper;
  struct _HeapPoint * next;
} HeapPoint;

/*
  Bucket
  Structure used to hold nodes that reside in a designated area
 */
typedef struct _Bucket{
  struct _Node * node;
  struct _Bucket * next;
} Bucket;

/*
  Bounds
  Structure that contains commonly used values
 */
typedef struct _Bounds{
  int dim, heap_max;
  double m1_min,m1_max,rd,r,c,m2_min,m2_max,m1_interval,m2_interval;
  double inv_input_cap; //from first input file
  double inv_output_cap; //from first input file
  double inv_output_res; //from first input file
} Bounds;



//********PA3 functions ********
void enlarge_heap(Heaper *** heap, Bounds * bounds);
void despan_Node(Bucket ** bucket_list, Node * node, Bounds * bounds);
void span_Node(Bucket ** bucket_list, Node * node, Bounds * bounds);
void downward_hepify(Heaper ** heap, int * heap_size, int i);
void init_heap(Node * node, Heaper ** heap, int * heap_size, Bounds * bounds, Bucket ** bucket_list);
int get_total_count(Bucket ** bucket_list, Node * head, Bounds * bounds);
void calc_span(Node * node, Bounds * bounds, int * x1, int * x2, int *y1, int * y2);
void insert_sinks(Bucket ** bucket_list, Node * head, Bounds * bounds);
void Merge_bucket(Bucket ** bucket_list, Bucket bucket, Bounds * bounds);
void count_neighbors(Bucket ** bucket_list, Node * node, Bounds * bounds);
int init_sinks(Bucket ** bucket_list, Node * head, Bounds * bounds);
double get_dist(Node * n_l, Node * n_r, double rd, double r, double c);
void create_heap(Heaper ** heap, Node * head, Bucket ** bucket_list, Bounds * bounds, int * heap_size);
void min_heap(Heaper ** heap, int * heap_size);
Heaper * extract_loc(Heaper ** heap, int * heap_size, int loc);
Node * greedy_merge(Heaper ** heap, int * heap_size, Bucket ** bucket_list, Bounds * bounds);
void upward_hepify(Heaper ** heap, int i);
void free_bucket_array(Bucket ** bucket_list, Bounds * bounds);
void free_heap(Heaper ** heap, int * heap_size);
int get_new_head(Bucket ** bucket_list, Bounds * bounds, Node ** new_node);
void greedy_master(Bounds * bounds, Node ** head);
void get_xy(Node * head);
void free_hp(Node * node);
void total_wire_len(Node * node, int * tot_len);


//******PA2 Functions ********
Node * pop(Node ** head);
void push(Node ** stack, Node * item);
Node * create_node(int label, double rd, double r, double c);
Node * create_array(FILE * in_fp, Bounds * bounds);
void det_bucket_dim(Node * head, Bounds * bounds);
void create_bucket_list(Bucket *** list, int dim);
void assign_xy(Node * child, Node * parent, double len);
void pre_order_merge(Node *n, double rd, double r, double c);
double det_len(double * m1, double * m2);
void destroy_tree(Node * head);
void build_sq(double * m, double r, double * sq);
void det_mvalues(double * m1, double * m2, double r1, double r2, double * output);
void merge_arcs(Node * n, double rd, double r, double c);
void check_l(Node * head, int * count);
double absolute(double x);
int intersect(double m1_0, double m1_1, double m2_0, double m2_1);


//***Printing functions ****
void print_test(FILE * fp, Node * head);
void print_heap(Heaper ** heap, int heap_size);
void print_binary(FILE * fp, Node * head);
void pre_order(FILE * fp, Node * head);
void post_order_len(FILE * fp, Node * head);
void post_order_xy(FILE * fp, Node * head);
void print_pre_order(Node * head);
void print_test(FILE * fp, Node * head);
void print_spicey(FILE * fp, Node * head);


//***PA 4 FUNCTIONS ***
void insert_source(Node ** head);
//int print_spice_netlist(int * curr_label, Node * root);
void parameter_input_files(char* input1, char* input2, Bounds *);

#endif
