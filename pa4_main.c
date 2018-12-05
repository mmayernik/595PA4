#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pa4.h"
 
void print_tree(Node * head);

//compile with zst.c zst_main.c and merge_func.c
int main(int argc, char ** argv){
  //intput handling
  if(argc != 6){
    printf("Incorrect number of arguements");
    printf("%d\n", argc);
    return EXIT_FAILURE;
  }

  //create struc for global use
  Bounds * bounds = malloc(sizeof(Bounds));
  bounds -> dim = 0;
  parameter_input_files(argv[1], argv[2], bounds);

  FILE * in_fp = fopen(argv[3], "r");
	if(in_fp == NULL){
	  return EXIT_FAILURE;
	}
  FILE * out_fp1 = fopen(argv[4], "w");
	if(out_fp1 == NULL){
		return EXIT_FAILURE;
	}
  FILE * out_fp2 = fopen(argv[5], "w");
	if(out_fp2 == NULL){
		return EXIT_FAILURE;
	}


 

  //create list of sinks
  Node * head = create_array(in_fp, bounds);
  int count = bounds -> dim;
  //preform merging
  greedy_master(bounds, &head);

  //calculate xy coords
  get_xy(head,&count);
  insert_source(&head);
  //verify topology
  //int count = 0;
  //check_l(head, &count);
  //int tot_len = 0;
  //total_wire_len(head, &tot_len);
  //printf("total length: %d\n", tot_len);
  print_tree(head);
  //print output
  post_order_len(out_fp1, head);
  //print_spicey(out_fp2, head);
  print_binary(out_fp2, head);
  //post_order_xy(out_fp2, head);
  printf("%le\n", head -> wire_l);
  free(bounds);
  destroy_tree(head);
 
  //exit gracefully
  fclose(in_fp);
  fclose(out_fp1);
  fclose(out_fp2);
  return 0;
}

//count number of sinks in tree "debug"
void count_it(Node * head, int * count){
  if(head == NULL){
    return;
  }
  if(head -> label != -1){
    *count+=1;
  }
  count_it(head -> left, count);
  count_it(head -> right, count);
}

//pre order print tree "debug"
void print_tree(Node * head){
  if(head == NULL){
    return;
  }
  print_tree(head -> left);
  print_tree(head -> right);
  printf("label: %d  polarity: %d  ", head -> label, head -> polarity);
  printf("parallel: %d\n", head->parallel);
}
