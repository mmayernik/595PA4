#include <stdio.h>
#include <stdlib.h>
#include "pa4.h"
#include <math.h>

int main(){
  //tset craxy loop
  Bounds bounds;
  Node head;
  Node child1;
  Node child2;

  bounds.c = 2*pow(10,-19);
  bounds.r = 1*pow(10,-4);
  bounds.inv_input_cap = 3.45 * pow(10,-14);
  bounds.inv_output_cap = 5.8 * pow(10, -14);
  bounds.inv_output_res = 61.4;
  bounds.tau_const = 1.2;
  
  int c1_x_1 = 2*pow(10,6);
  int c1_y_1 = 2*pow(10,6);
  int c1_x_2 = 2*pow(10,6);
  int c1_y_2 = 2*pow(10,6);
  child1.m_array[0] = c1_y_1+c1_x_1;
  child1.m_array[1] = c1_y_2+c1_x_2;
  child1.m_array[2] = c1_y_1-c1_x_1;
  child1.m_array[3] = c1_y_1-c1_x_1;
  child1.c = 34*pow(10,-14);
  child1.r = 0.8 * 50 * pow(10, -12) / child1.c;
  child1.polarity = 0;
  child1.t = 0;
  
  int c2_x_1 = 4*pow(10,6);
  int c2_y_1 = 4*pow(10,6);
  int c2_x_2 = 4*pow(10,6);
  int c2_y_2 = 4*pow(10,6);
  child2.m_array[0] = c2_y_1+c2_x_1;
  child2.m_array[1] = c2_y_2+c2_x_2;
  child2.m_array[2] = c2_y_1-c2_x_1;
  child2.m_array[3] = c2_y_1-c2_x_1;
  child2.c = 34*pow(10,-14);
  child2.t = 0;
  child2.r = 0.8 * 50 * pow(10, -12) / child2.c;
  child2.polarity = 0;

  head.left = &child1;
  head.right = &child2;
  head.wire_l = 0;
  merge_arcs(&head, bounds.r, bounds.c);
  double new_len = i_wire(&child2, &bounds);
  printf("%le %le\n", head.wire_l, new_len);
  double old = calc_tau(&child2, head.wire_r, &bounds);
  double new = calc_tau(&child2, new_len, &bounds);
  printf("old: %le, new: %le\n",old ,new);
  insert_i(&head, &child2, &bounds);
  insert_i(&head, &child1, &bounds);
  if(need_i(&head, &bounds)){
    printf("this is a problem\n");
  }
  printf("left %le, right %le\n", head.left -> wire_l,  head.right -> wire_l);
  merge_arcs(&head, bounds.r, bounds.c);
  printf("%le, %le\n", head.wire_l, head.wire_r);
  printf("%le\n", calc_tau(&head, 0, &bounds));
  if(need_i(&head, &bounds)){
    printf("this is a problem\n");
  }

  
  /*Testing i_loco and _choose_pt
  Node source;
  int s_x_1 = 4;
  int s_y_1 = 4;
  int s_x_2 = 2;
  int s_y_2 = 2;
  source.m_array[0] = s_y_1+s_x_1;
  source.m_array[1] = s_y_2+s_x_2;
  source.m_array[2] = s_y_1-s_x_1;
  source.m_array[3] = s_y_1-s_x_1;

  Node dest;
  int d_x_1 = 6;
  int d_y_1 = 6;
  int d_x_2 = 8;
  int d_y_2 = 4;
  dest.m_array[0] = d_y_1+d_x_1;
  dest.m_array[1] = d_y_2+d_x_2;
  dest.m_array[2] = d_y_1-d_x_1;
  dest.m_array[3] = d_y_1-d_x_1;

  Node i;
  i_loco(&i, &dest, &source, 4);
  printf("%f, %f\n", i.x, i.y);
  //printf("%d\n", _choose_pt(&source, dest.m_array)); //test _chhose_pt
  */
  
}
