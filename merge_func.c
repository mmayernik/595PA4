#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pa4.h"
#include <stdbool.h>

Node * create_node(int label, double x, double y, double c){
  Node * node =  malloc(sizeof(Node));
  node -> label = label;
  node -> c = c;
  node -> next = NULL;
  node -> left = NULL;
  node -> right = NULL;
  node -> lens = 0;
  node -> len_list = NULL;
  node -> x = x;
  node -> y = y;
  node -> wire_l = -1;
  node -> wire_r = -1;
  node -> t = 0;
  node -> merged = 0;
  node -> parallel = 0;
  node -> polarity = 0;
  return node;
}

Node * pop(Node ** head){
  if(head == NULL){
    return NULL;
  }
  Node * dummy = *head;
  *head = (*head) -> next;
  dummy -> next = NULL;
  return dummy; 
}

void push(Node ** stack, Node * item){
  item -> next = *stack;
  *stack = item;
}

double get_dist(Node * n_l, Node * n_r, double rd, double r, double c){
  double x = 0;
  double L = det_len(n_l -> m_array, n_r -> m_array);
  if(L == 0){
    if(n_l -> t < n_r -> t){
      x = -1;
    }else{
      x = 2;
    }
  }
  else{ //left = b, right = a
    x = (n_l -> t - n_r -> t + r*L*(n_l->c + c * L / 2.0)) /(r*L*(c*L + n_l->c + n_r->c));
  }
  double  wire_l = (1-x)*L;
  double  wire_r = x*L;
  if(x < 0){ // doulbe backing ta > tb
    wire_r = 0;
    double ctb = n_l -> c;
    double tb = n_l -> t;
    double ta = n_r -> t;
    wire_l = (-ctb*r+sqrt(pow(ctb*r,2)-2*c*r*(tb-ta)))/(c*r); 
  }
  else if(x > 1){ // rlc > ta
    wire_l = 0;
    double ctb = n_r -> c;
    double tb = n_r -> t;
    double ta = n_l -> t;
    wire_r = (-ctb*r+sqrt(pow(ctb*r,2)-2*c*r*(tb-ta)))/(c*r);
  }
  return wire_r + wire_l;
}

void pre_order(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  if(head -> label == -1){
    fprintf(fp, "()\n");
  }
  else{
    fprintf(fp, "%d(%le %le %le)\n", head -> label, head -> c,  head -> x, head -> y);
  }
  pre_order(fp, head -> left);
  pre_order(fp, head -> right);
}

void post_order_len(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  post_order_len(fp, head -> left);
  post_order_len(fp, head -> right);
  if(head -> label == -1){
    fprintf(fp, "(%le %le)\n", head -> wire_l, head -> wire_r);
  }
  else{
    fprintf(fp, "%d(%le %le %le)\n", head -> label, head -> c, head -> x, head -> y);
  }
}

void post_order_xy(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  post_order_xy(fp, head -> left);
  post_order_xy(fp, head -> right);
  if(head -> label == -1){
    fprintf(fp, "(%le %le %le %le)\n", head -> wire_l, head -> wire_r, head -> x, head -> y);
  }
  else{
    fprintf(fp, "%d(%le %le %le)\n", head -> label, head -> c, head -> x, head -> y);
  }
}

void build_sq(double * m, double r, double * sq){
  if(m[0] > m[1]){
    sq[0] = m[0] + r;
    sq[1] = m[1] - r;
  }
  else{
    sq[0] = m[1] + r;
    sq[1] = m[0] - r;
  }
  if(m[2] > m[3]){
    sq[2] = m[2] + r;
    sq[3] = m[3] - r;
  }
  else{
    sq[2] = m[3] + r;
    sq[3] = m[2] - r;
  }  
}

void swap(double *xp, double *yp) { 
    double temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void bubbleSort(double * arr, int n) { 
   for (int i = 0; i < n-1; i++){   
     for (int j = 0; j < n-i-1; j++){
       if(arr[j] > arr[j+1]){ 
              swap(&arr[j], &arr[j+1]);
       }
     }
   }
} 

void det_mvalues(double * m1, double * m2, double r1, double r2, double * output){
  double sq1[4];
  double sq2[4];
  
  build_sq(m1, r1, sq1);
  build_sq(m2, r2, sq2);
 
  double temp1[4] = {sq1[0], sq1[1], sq2[0], sq2[1]};
  double temp2[4] = {sq1[2], sq1[3], sq2[2], sq2[3]};
  bubbleSort(temp1, 4);
  bubbleSort(temp2, 4);

  //push together if no intersection
  if(intersect(sq1[0], sq1[1], sq2[0], sq2[1]) == 0){
    temp1[1] = (temp1[1] + temp1[2])/2;
    temp1[2] = temp1[1];
  }
  if(intersect(sq1[2], sq1[3], sq2[2], sq2[3]) == 0){
    temp2[1] = (temp2[1] + temp2[2])/2;
    temp2[2] = temp2[1];
  }
  
  output[0] = temp1[1];
  output[1] = temp1[2];
  output[2] = temp2[1];
  output[3] = temp2[2];
}

void merge_arcs(Node * n, double r, double c){
  double x = 0;
  double L = det_len((n -> left) -> m_array, (n -> right) -> m_array);
  if(L == 0){
    if((n -> left) -> t < (n -> right) -> t){
      x = -1;
    }else{
      x = 2;
    }
  }
  else{ //left = b, right = a
    x = ((n -> left) -> t - (n -> right) -> t + r*L*((n->left)->c + c * L / 2.0)) /(r*L*(c*L + (n->left)->c + (n -> right)->c));
  }
  n -> wire_l = (1-x)*L;
  n -> wire_r = x*L;
  if(x < 0){ // doulbe backing ta > tb
    n -> wire_r = 0;
    double ctb = (n -> left) -> c;
    double tb = (n -> left) -> t;
    double ta = (n -> right) -> t;
    n-> wire_l = (-ctb*r+sqrt(pow(ctb*r,2)-2*c*r*(tb-ta)))/(c*r); 
  }
  else if(x > 1){ // rlc > ta
    n -> wire_l = 0;
    double ctb = (n -> right) -> c;
    double tb = (n -> right) -> t;
    double ta = (n -> left) -> t;
    n-> wire_r = (-ctb*r+sqrt(pow(ctb*r,2)-2*c*r*(tb-ta)))/(c*r);
  }
  det_mvalues((n -> left) -> m_array, (n -> right) -> m_array, n -> wire_l, n -> wire_r, n -> m_array);
  n -> c  = (n->left) -> c + (n -> right) -> c + (n->wire_l + n->wire_r)*c;
  n -> t = (n-> right) -> t + n -> wire_r * r * ((n->right)->c + c*n->wire_r/2.0);
}

void determine_shortest(double * point, double * m){
  double dis1[4] = {m[0],m[0],m[2],m[2]};
  double dis2[4] = {m[1],m[1],m[3],m[3]};
  double len1 = det_len(point, dis1);
  double len2 = det_len(point, dis2);
  if(len1 < len2){
    m[1] = m[0];
    m[3] = m[2];
  }else{
    m[0] = m[1];
    m[2] = m[3];
  }
}
 
void assign_xy(Node * child, Node * parent, double len, int * count){
  if(child -> label != -1){
    return;
  }
  if(len == 0){
     child -> x = parent -> x;
     child -> y = parent -> y;
     for(int i = 0; i < 4; i++){
       child -> m_array[i] = parent -> m_array[i];
     }
  }
  else{
    det_mvalues(child -> m_array, parent -> m_array, 0, len, child -> m_array);
    determine_shortest(parent -> m_array, child -> m_array);
    child -> x = ((child -> m_array)[0]-(child -> m_array)[2])/2.0;
    child -> y = ((child ->m_array)[0]+(child -> m_array)[2])/2.0;
  }
  *count += 1;
  parent -> label = *count;
  assign_xy(child -> left, child, child -> wire_l, count);
  assign_xy(child -> right, child, child -> wire_r, count);
}

void get_xy(Node * head, int * count){
  double p1[4] = {head -> m_array[0], head -> m_array[0], head -> m_array[2], head -> m_array[2]};
  double p2[4] = {head -> m_array[1], head -> m_array[1], head -> m_array[3], head -> m_array[3]};
  double source[4] = {0,0,0,0};
  double dist1 = det_len(p1, source);
  double dist2 = det_len(p2, source);
  *count +=1;
  head -> label = *count;
  if(dist1 > dist2){
  head -> m_array[0] = head -> m_array[1];
  head -> m_array[2] = head -> m_array[3];
  }else{
  head -> m_array[1] = head -> m_array[0];
  head -> m_array[3] = head -> m_array[2];
  }
  head -> x = ((head -> m_array)[1] - (head -> m_array)[3])/2.0;
  head -> y = ((head -> m_array)[1] + (head -> m_array)[3])/2.0;

  assign_xy(head -> left, head, head -> wire_l, count);
  assign_xy(head -> right, head, head -> wire_r, count);
}

void insert_source(Node ** head){
  Node * bnode = create_node(-1, 0, 0, -1); //MUST MAKE THIS have the CAP for a buffer at some point 
  Node* snode = create_node(-1, 0, 0, -1); //source node
  snode->left = bnode;
  bnode->left = *head;
  bnode->parallel = 1;
  snode->parallel = 1;
  *head = snode;
}

double absolute(double x){
  if(x <0){
    x *= -1;
  }
  return x;
}

int in_middle(double test, double num1, double num2){
  if((test >= num1 && test <= num2) || (test <= num1 && test >= num2)){
    return 1;
  }
  return 0;
}

int intersect(double m1_0, double m1_1, double m2_0, double m2_1){
  if(in_middle(m2_0, m1_0, m1_1) || in_middle(m2_1, m1_0, m1_1) ||
     in_middle(m1_0, m2_0, m2_1) || in_middle(m1_1, m2_0, m2_1)){
    return 1;
  }
  return 0;
}

double min_distance(double point, double m1, double m2){
  double dis1 = absolute(m1 - point);
  double dis2 = absolute(m2 - point);
  if(dis1 < dis2){
    return dis1;
  }
  return dis2;
}

double det_len(double * m1, double * m2){
  double len = 0;
  double temp1, temp2;
  if(intersect(m1[0], m1[1], m2[0], m2[1]) != 1){
    len = min_distance(m2[0], m1[0], m1[1]);
    temp1 = min_distance(m2[1], m1[0], m1[1]);
    if(temp1 < len){
      len = temp1;
    }
  }
  if(intersect(m1[2], m1[3], m2[2], m2[3]) != 1){
    temp1 = min_distance(m2[2], m1[2], m1[3]);
    temp2 = min_distance(m2[3], m1[2], m1[3]);
    if(temp2 < temp1){
      temp1 = temp2;
    }
    if(temp1 > len){
      len = temp1;
    }
  }
  return len;
}


//****Printing Functions****
void print_pre_order(Node * head){
  if(head == NULL){
    return;
  }
  if(head -> label == -1){
    printf("()");
  }else{
    printf("(%d)", head -> label);
  }
  print_pre_order(head -> left);
  print_pre_order(head -> right);
}



//PA 4 WELCOME TO THE JUNGLE ///

void crazy_loop(Node * head, Bounds * bounds){
  //find length for zero skew
  double max_tau = 0.8 * 100 * pow(10, -12);
  double tau_L = calc_tau(head->left, head->wire_l, bounds);
  double tau_R = calc_tau(head->right, head->wire_r, bounds);
  while(need_i(head, bounds)){
    //left tau
    if(tau_L >= max_tau){
      insert_i(&head, head->left, bounds);
    }
    //right tau 
    if(tau_R >= max_tau){
      insert_i(&head, head->right, bounds);
    }
    //merge
    if(tau_R < max_tau && tau_L < max_tau){
      if(tau_R > tau_L){
	insert_i(&head, head->right, bounds);
      }
      else{
	insert_i(&head, head->left, bounds);
      }
      merge_arcs(head, bounds-> r, bounds->c);
    }
    tau_L = calc_tau(head->left, head->wire_l, bounds);
    tau_R = calc_tau(head->right, head->wire_r, bounds);
  }

  //FINAL POLARITY CHECK 
  if(head->right->polarity != head->left->polarity){
    if(tau_R > tau_L){
	insert_i(&head, head->right, bounds);
      }
      else{
	insert_i(&head, head->left, bounds);
      }
    merge_arcs(head, bounds->r, bounds->c); 
  }
						 
  
}

bool need_i(Node* head, Bounds * bounds){
  double tau = calc_tau(head, 0, bounds);
  if(tau > 0.8 * 100 * pow(10, -12)){
      return true;
    }
  if(head->left->polarity != head->right->polarity){
    return true;
  }
  return false;
}

double calc_tau(Node * n, double wire_len, Bounds * bounds){
  //tau = nrc
  double wire_r = bounds->r * wire_len;
  double wire_c = bounds->c * wire_len / 2;
  double total_r = wire_r; //+ n->r;
  double total_c = wire_c + n->c;
  double tau = bounds->tau_const * total_r * total_c;
  return tau;
}

void insert_i(Node ** head, Node * child, Bounds * bounds){
  
  Node * i = create_node(-1, 0, 0, bounds->inv_output_cap);
  i->left = child;
  if(i->left == (*head)->left){
    (*head)->left = i;
    i->wire_l = i_wire(*head, bounds);
    
  }
  else{
    (*head)->right = i;
    i->wire_r = i_wire(*head, bounds);
  }
  
  //UPDATE HEADS RESISTANCE AND CAPACITANCE
}

double i_wire(Node * head, Bounds * bounds){
  double r = bounds->r;
  double C = bounds->c;
  double cn = head->c;
  double rn = 1; //head->r; //FIXXXX
  double a = r * C / 2;
  double b = cn * r + C * rn /2;
  double c = rn * cn - 100 * pow(10, -12) / bounds->tau_const;
  double L = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
  return L;
}
