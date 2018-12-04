#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pa4.h"



//compile with zst.c zst_main.c and merge_func.c
int main(int argc, char ** argv){
  //intput handling
  if(argc != 4){
    printf("Incorrect number of arguements");
    printf("%d\n", argc);
    return EXIT_FAILURE;
  }
  FILE * in_fp = fopen(argv[1], "r");
  if(in_fp == NULL){
    printf("Could not open input file");
    return EXIT_FAILURE;
  }
  FILE * out_fp1 = fopen(argv[2], "w");
  if(out_fp1 ==  NULL){
    printf("Could not open first output file");
    fclose(in_fp);
    return EXIT_FAILURE;
  }
  FILE * out_fp2 = fopen(argv[3], "w");
  if(out_fp2 == NULL){
    printf("Could not open second output file");
    fclose(in_fp);
    fclose(out_fp1);
    return EXIT_FAILURE;
  }

  //create struc for global use
  Bounds * bounds = malloc(sizeof(Bounds));
  bounds -> dim = 0;

  //create list of sinks
  Node * head = create_array(in_fp, bounds);

  //preform merging
  greedy_master(bounds, &head);

  //calculate xy coords
  get_xy(head);
  //insert_source(&head);
  //verify topology
  //int count = 0;
  //check_l(head, &count);
  //int tot_len = 0;
  //total_wire_len(head, &tot_len);
  //printf("total length: %d\n", tot_len);
  
  //print output
  post_order_len(out_fp1, head);
  print_spicey(out_fp2, head);
  //post_order_xy(out_fp2, head);
  
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
  printf("%d\n", head -> label);
  print_tree(head -> left);
  print_tree(head -> right);
}
