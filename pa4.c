#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pa4.h"

Bucket * pop_bucket(Bucket ** head){
  if(head == NULL){
    return NULL;
  }
  Bucket * dummy = *head;
  *head = (*head) -> next;
  dummy -> next = NULL;
  return dummy; 
}

void validate_index(int * value, Bounds * bounds){
  *value = *value >= bounds -> dim ? bounds -> dim -1 : *value;
  *value = *value < 0 ? 0 : *value;  
}

Node * create_array(FILE * in_fp, Bounds * bounds){
  fscanf(in_fp, "%le %le %le\n", &(bounds -> rd), &(bounds -> r), &(bounds -> c)); //get first line
  Node * dummy = NULL;
  Node ** head_ptr = &dummy;
  int ch, label;
  double x, y, cap;
  Node * n;
  while((ch = getc(in_fp)) != EOF){
    fseek(in_fp, -1, SEEK_CUR);
    fscanf(in_fp, "%d(%le %le %le)\n", &label, &cap, &x, &y);
    n = create_node(label, x, y, cap);
    n -> m_array[0] = n -> m_array[1] = (n -> y + n -> x);
    n -> m_array[2] = n -> m_array[3] = (n -> y - n -> x);
    push(head_ptr, n);
    (bounds -> dim) += 1;
   }
  return *head_ptr;
}

void det_bucket_dim(Node * head, Bounds * bounds){
  while(head != NULL){
    if(head -> m_array[0] > (bounds -> m1_max)){
      (bounds -> m1_max) = head -> m_array[0];
    }else if(head -> m_array[0] < (bounds -> m1_min)){
      (bounds -> m1_min) = head -> m_array[0];
    }
    if(head -> m_array[1] > (bounds -> m1_max)){
      (bounds -> m1_max) = head -> m_array[1];
    }else if(head -> m_array[1] < (bounds -> m1_min)){
      (bounds -> m1_min) = head -> m_array[1];
    }
    if(head -> m_array[2] > (bounds -> m2_max)){
      (bounds -> m2_max) = head -> m_array[2];
    }else if(head -> m_array[2] < (bounds -> m2_min)){
      (bounds -> m2_min) = head -> m_array[2];
    }
    if(head -> m_array[3] > (bounds -> m2_max)){
      (bounds -> m2_max) = head -> m_array[3];
    }else if(head -> m_array[3] < (bounds -> m2_min)){
      (bounds -> m2_min) = head -> m_array[3];
    }
    head = head -> next;
  }
  bounds -> dim = (int)sqrt(bounds -> dim);
  bounds -> m1_interval = ((bounds -> m1_max)-(bounds -> m1_min))/(bounds -> dim);
  bounds -> m2_interval =  ((bounds -> m2_max)-(bounds -> m2_min))/(bounds -> dim);
}

void create_bucket_list(Bucket *** list, int dim){
  *list = malloc(dim * sizeof(Bucket *));
  Bucket * new_bucket;
  for(int i = 0; i < dim; i++){
    new_bucket = malloc(dim * sizeof(Bucket));
    for(int k = 0; k < dim; k++){
      new_bucket[k].next = NULL;
      new_bucket[k].node = NULL;
    }
    (*list)[i] = new_bucket;
  }
}

void insert_sinks(Bucket ** bucket_list, Node * head, Bounds * bounds){
  Node * temp;
  while(head != NULL){
    Bucket * new_bucket = malloc(sizeof(Bucket));
    new_bucket -> next = NULL;
    new_bucket -> node = head;
    int x = (int)((head -> m_array[2] - (bounds -> m2_min))/ (bounds -> m2_interval));
    int y = (int)((head -> m_array[0] - (bounds -> m1_min))/ (bounds -> m1_interval));
    validate_index(&x, bounds); //make sure not out of the 2D array
    validate_index(&y, bounds);    
    temp = head -> next;
    new_bucket -> next = bucket_list[x][y].next;
    bucket_list[x][y].next = new_bucket;
    head = temp;
  }
}

void calc_span(Node * node, Bounds * bounds, int * x1, int * x2, int *y1, int * y2){
  *y1 = (int)((node -> m_array[0] - (bounds -> m1_min))/(bounds -> m1_interval));
  *y2 = (int)((node -> m_array[1] - (bounds -> m1_min))/(bounds -> m1_interval));
  *x1 = (int)((node -> m_array[2] - (bounds -> m2_min))/(bounds -> m2_interval));
  *x2 = (int)((node -> m_array[3] - (bounds -> m2_min))/(bounds -> m2_interval));
  validate_index(y1, bounds);
  validate_index(y2, bounds);
  validate_index(x1, bounds);
  validate_index(x2, bounds);
  int temp;
  if(*x2 < *x1){
      temp = *x1;
      *x1 = *x2;
      *x2 = temp;
  }
  if(*y2 < *y1){
      temp = *y1;
      *y1 = *y2;
      *y2 = temp;
  }
}


void add_to_heap(Heaper ** heap, Node * node1, Node * node2, Bounds * bounds, int * heap_size){
  double dis = get_dist(node1, node2, bounds -> rd, bounds -> r, bounds -> c); //calcualte total distance
  Heaper * new_heaper = malloc(sizeof(Heaper)); //create item to be put in main heap
  new_heaper -> tot_len = dis; 
  new_heaper -> node1 = node1; //give heaper the two nodes that were used
  new_heaper -> node2 = node2;
  HeapPoint * h_node1 = malloc(sizeof(HeapPoint)); //create items for nodes personal list
  HeapPoint * h_node2 = malloc(sizeof(HeapPoint));
  h_node1 -> heaper = new_heaper; //give address of location in heap
  h_node2 -> heaper = new_heaper;
  h_node1 -> next = node1 -> len_list;   //push items into node's personal lists
  node1 -> len_list = h_node1;
  h_node2 -> next = node2 -> len_list;
  node2 -> len_list = h_node2;
  heap[*heap_size] = new_heaper;
  new_heaper -> loc = *heap_size;//store and increment heap
  upward_hepify(heap, *heap_size);
  *heap_size += 1;
}

void calc_heaper(Heaper ** heap, int * heap_size,  Node * node, Bucket ** bucket_list, Bounds * bounds){
  //used for initial creation of heap
  int x1, x2, y1, y2;
  calc_span(node, bounds, &x1, &x2, &y1, &y2);
  x1 -= 1;
  x2 += 1;
  y1 -= 1;
  y2 += 1;
  validate_index(&y1, bounds);
  validate_index(&y2, bounds);
  validate_index(&x1, bounds);
  validate_index(&x2, bounds);
  for(int i = x1 ; i <= x2; i++){
    for(int k = y1; k <= y2; k++){
      Bucket * temp = bucket_list[i][k].next;
      while(temp != NULL){
	if((temp -> node) -> merged != 1 && temp -> node != node){
	  add_to_heap(heap, node, temp -> node,  bounds, heap_size);
	}
	temp = temp -> next;
      }
    }
  }
}

void calc_heaper2(Heaper ** heap, int * heap_size,  Node * node, Bucket ** bucket_list, Bounds * bounds){
  //used for adding to heap after already created
  int x1, x2, y1, y2;
  calc_span(node, bounds, &x1, &x2, &y1, &y2);
  x1 -= 1;
  x2 += 1;
  y1 -= 1;
  y2 += 1;
  validate_index(&y1, bounds);
  validate_index(&y2, bounds);
  validate_index(&x1, bounds);
  validate_index(&x2, bounds);
  for(int i = x1 ; i <= x2; i++){
    for(int k = y1; k <= y2; k++){
      Bucket * temp = bucket_list[i][k].next;
      while(temp != NULL){
	if(temp -> node != node) {
	  add_to_heap(heap, node, temp -> node,  bounds, heap_size);
	}
	temp = temp -> next;
      }
    }
  }
}

void init_heap(Node * node, Heaper ** heap, int * heap_size, Bounds * bounds, Bucket ** bucket_list){
  while(node != NULL){
    calc_heaper(heap, heap_size, node, bucket_list, bounds);
    node -> merged = 1;
    node = node -> next;
  }
  for(int i = 0; i < *heap_size; i++){
    heap[i] -> loc = i;
  }
  for(int i = *heap_size / 2 - 1; i >=0; i--){
    downward_hepify(heap, heap_size, i);
  }
}

void despan_Node(Bucket ** bucket_list, Node * node, Bounds * bounds){
  int x1,x2,y1,y2;
  calc_span(node, bounds, &x1, &x2, &y1, &y2);
  for(int i = y1; i<=y2;i++){
    for(int k = x1;k<=x2;k++){
      Bucket * curr = bucket_list[k][i].next;
      Bucket dummy;
      Bucket * prev = &dummy;
      prev -> next = curr;
      Bucket * head = curr;
      while(curr != NULL){
	if(curr -> node == node){
	  if(head == curr){
	    bucket_list[k][i].next = curr -> next;
	    free(head);
	  }else{
	    prev -> next = curr -> next;
	    free(curr);
	  }
	  break;
	}
	prev = curr;
	curr = curr -> next;
      }
    }
  }
}

void span_Node(Bucket ** bucket_list, Node * node, Bounds * bounds){
  int x1,x2,y1,y2;
  calc_span(node, bounds, &x1, &x2, &y1, &y2);
  for(int i = y1; i<=y2; i++){
    for(int k = x1;k<=x2;k++){
      Bucket * new_bucket = malloc(sizeof(Bucket));
      new_bucket -> node = node;
      new_bucket -> next = bucket_list[k][i].next;
      bucket_list[k][i].next = new_bucket;
    }
  }
}

int init_sinks(Bucket ** bucket_list, Node * head, Bounds * bounds){
  int total = 0;
  while(head != NULL){
    count_neighbors(bucket_list, head, bounds);
    total += head -> lens;
    head = head -> next;
  }
  return total;
}

void swap_heaper(Heaper ** num1, Heaper ** num2){
  Heaper * temp = *num1;
  *num1 = *num2;
  *num2 = temp;
  int num1_loc = (*num1) -> loc;
  (*num1) -> loc = (*num2) -> loc;
  (*num2) -> loc = num1_loc;
  
}

void downward_hepify(Heaper ** heap, int * heap_size, int i){
  int l = 2*i + 1;
  int r = 2*i + 2;
  int smallest = i;
  if(l < *heap_size && heap[l] -> tot_len < heap[smallest] -> tot_len){
    smallest = l;
  }
  if(r < *heap_size && heap[r] -> tot_len < heap[smallest] -> tot_len){
    smallest = r;
  }
  if(smallest != i){
    swap_heaper(&heap[i], &heap[smallest]);
    downward_hepify(heap, heap_size, smallest);
  }
}

void upward_hepify(Heaper ** heap, int i){
  int parent = (i-1)/2;
  if(parent >= 0 && heap[parent]-> tot_len > heap[i] -> tot_len){
    swap_heaper(&heap[parent], &heap[i]);
    upward_hepify(heap, parent);
  }
}

Heaper * extract_loc(Heaper ** heap, int * heap_size, int loc){
  swap_heaper(&heap[loc], &heap[*heap_size - 1]);
  *heap_size -= 1;
  downward_hepify(heap, heap_size, loc);
  return heap[*heap_size];
}

void count_neighbors(Bucket ** bucket_list, Node * node, Bounds * bounds){
  int x1, x2, y1, y2, y_min, x_min;
  calc_span(node, bounds, &x1, &x2, &y1, &y2);
  x1 -= 1;
  x2 += 1;
  y1 -= 1;
  y2 += 1;
  validate_index(&y1, bounds);
  validate_index(&y2, bounds);
  validate_index(&x1, bounds);
  validate_index(&x2, bounds);
  y_min = y1;
  x_min = x1;
  for(;y1 <= y2; y1 += 1){
    for(x1 = x_min;x1 <= x2; x1 += 1){
      if(y1 == y2 || y1 == y_min || x1 == x_min || x1 == x2){
	Bucket * temp = bucket_list[x1][y1].next;
	while(temp != NULL){
	  if(temp -> node != node){	    
	    node -> lens += 1;
	  }
	   temp = temp -> next;
	}
      }
    }
  }
}

int get_total_count(Bucket ** bucket_list, Node * head, Bounds * bounds){
  int total = 0;
  while(head != NULL){
    count_neighbors(bucket_list, head, bounds);
    total += head -> lens;
    head = head -> next;
  }
  return total;
}

void remove_heapers(Heaper ** heap, int * heap_size, HeapPoint * list, Heaper * shared, Node * node){
  HeapPoint * curr_hp = list;
  while(curr_hp != NULL){
    if(curr_hp -> heaper != shared && curr_hp -> heaper -> loc < *heap_size){// && curr_hp -> heaper -> loc < *heap_size
      Heaper * heaper = extract_loc(heap, heap_size, (curr_hp -> heaper) -> loc);
      Node * other_node = (heaper -> node1 ==  node) ? heaper -> node2 : heaper -> node1;
      HeapPoint * curr = other_node -> len_list;
      HeapPoint * prev = curr;
      while(curr != NULL){
	if(curr -> heaper == curr_hp -> heaper){
	  if(prev == curr){
	    other_node -> len_list = curr -> next;
	  }else{
	    prev -> next = curr -> next;
	  }
	  break;
	}
	prev = curr;
	curr = curr -> next;
      }
      free(curr);
      free(heaper);
      
    }
    curr_hp = curr_hp -> next;
  }
}


void greedy_master(Bounds * bounds, Node ** head){
  Bucket ** bucket_list = NULL;
  int nodes_left = bounds -> dim;
  //free stuff when?
  while(nodes_left != 1){
    //init bounds
    (bounds -> m1_max) = (bounds -> m1_min) = (*head) -> m_array[0];
    (bounds -> m2_max) = (bounds -> m2_min) = (*head) -> m_array[2];
    det_bucket_dim(*head, bounds);
    //init bucket_list
    bucket_list = NULL;
    create_bucket_list(&bucket_list, bounds -> dim);
    insert_sinks(bucket_list, *head, bounds);
    //create heap
    Heaper ** heap = malloc((int)pow(nodes_left,2) * sizeof(Heaper *));
    int heap_size = 0;
    init_heap(*head, heap, &heap_size, bounds, bucket_list);
    //preform greedy merging
    greedy_merge(heap, &heap_size, bucket_list, bounds);
    *head = NULL;
    //make new node
    nodes_left = get_new_head(bucket_list, bounds, head);
    //free unused stuff here
    free_bucket_array(bucket_list, bounds);
    free_heap(heap, &heap_size);
  }
}

Node * greedy_merge(Heaper ** heap, int * heap_size, Bucket ** bucket_list, Bounds * bounds){
  Node * new_node = NULL;
  while(*heap_size > 0){ //condition here
    Heaper * min_dis = extract_loc(heap, heap_size, 0);
    new_node = create_node(-1, 0, 0, 0);
    new_node -> left = min_dis -> node1;
    new_node -> right = min_dis -> node2;
    merge_arcs(new_node, bounds->rd, bounds->r, bounds->c); //merge arcs together
    despan_Node(bucket_list, min_dis -> node1, bounds); //create new spanning in buckets
    despan_Node(bucket_list, min_dis -> node2, bounds);
    span_Node(bucket_list, new_node, bounds);
    remove_heapers(heap, heap_size, (min_dis -> node1) -> len_list, min_dis, min_dis -> node1); 
    remove_heapers(heap, heap_size,  (min_dis -> node2) -> len_list, min_dis, min_dis -> node2);
    free_hp(min_dis -> node1);
    free_hp(min_dis -> node2);
    free(min_dis);
    calc_heaper2(heap, heap_size,  new_node, bucket_list, bounds);
  }
  return new_node;
}

int get_new_head(Bucket ** bucket_list, Bounds * bounds, Node ** new_node){
  int total = 0;
  for(int i = 0; i < bounds -> dim; i++){
    for(int k = 0; k < bounds -> dim; k++){
      Bucket * head = bucket_list[i][k].next;
      Bucket * temp;
      while(head != NULL){
	(head -> node) -> merged = 0;
	push(new_node, head -> node);
	temp = head -> next;
	despan_Node(bucket_list, head-> node, bounds);
	head = temp;
	total++;
      }
    }
  }
  return total;
}

//******Free memory Functions**********
void free_hp(Node * node){
  HeapPoint * curr = node -> len_list;
  HeapPoint * temp;
  while(curr != NULL){
    temp = curr;
    curr = curr -> next;
    free(temp);
  }
  node -> len_list = NULL;
}

void free_bucket_array(Bucket ** bucket_list, Bounds * bounds){
  for(int i = bounds -> dim -1; i > -1; i--){
    for(int k = bounds -> dim-1; k > -1; k--){
      Bucket * bucket = bucket_list[i][k].next;
      Bucket * next;
      while(bucket != NULL){
	next = bucket -> next;
	free(bucket);
	bucket = next;
      }
    }
    free(bucket_list[i]);
  }
  free(bucket_list);
}

void free_heap(Heaper ** heap, int * heap_size){
  *heap_size = *heap_size >= 0? *heap_size - 1 : *heap_size;
  for(;*heap_size > -1; *heap_size -= 1){
    free(heap[*heap_size]);
  }
  *heap_size = 0;
  free(heap);
}

void destroy_tree(Node * head){
  if(head == NULL){
    return;
  }
  destroy_tree(head -> left);
  destroy_tree(head -> right);
  HeapPoint * hp;
  while(head -> len_list != NULL){
      hp = (head -> len_list) -> next;
      free(head -> len_list);
      (head -> len_list) = hp;
  }
  free(head);
}

//***********Functions to check correctness of output****************
void check_l(Node * head, int * count){
  if(head -> label != -1){
    return;
  }
  if((absolute(head -> x - (head -> left) -> x) + absolute(head -> y - (head -> left) -> y)) > head -> wire_l * 1.001){
    printf("parent: (%le, %le) child (%le, %le) len: %le opr: %le\n", head -> x, head -> y, (head -> left) -> x, (head -> left) -> y, head -> wire_l, head -> wire_r);
    *count += 1;
  }
  if((absolute(head -> x - (head -> right) -> x) + absolute(head -> y - (head -> right) -> y)) > head -> wire_r * 1.001){
    printf("parent: (%le, %le) child (%le, %le) len: %le opl: %le\n", head -> x, head -> y, (head -> right) -> x, (head -> right) -> y, head -> wire_r, head -> wire_l);
    *count += 1;
  }
  check_l(head -> left, count);
  check_l(head -> right, count);
}

void total_wire_len(Node * node, int * tot_len){
  if(node -> label != -1){
    return;
  }
  *tot_len += (node -> wire_l + node -> wire_r);
  total_wire_len(node -> left, tot_len);
  total_wire_len(node -> right, tot_len);
}

//**********Funtions for output printing and debugging********
void print_bucket(Bucket ** bucket_list, Bounds * bounds){
  for(int i = 0; i < bounds -> dim; i++){
    for(int k = 0; k < bounds -> dim; k++){
      Bucket * head = bucket_list[i][k].next;
      printf("[");
      while(head != NULL){
	printf("%d,", (head -> node) -> label);
	head = head -> next;
      }
      printf("]\t");
    }
    printf("\n");
  }
}
void print_test(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  print_test(fp, head -> left);
  print_test(fp, head -> right);
  if(head -> label == -1){
    fprintf(fp, "(%le %le)\n", head -> wire_l, head -> wire_r);
  }else{
    fprintf(fp, "%d(%le)\n", head -> label, head -> c);
  }
}

void print_heap(Heaper ** heap, int heap_size){
  int count = 1;
  for(int i = 0; i < heap_size; i++){
    printf("[%d %d %d]\t", (heap[i]->node1) -> label, (heap[i]->node2) -> label, (int)heap[i] -> tot_len);
    if(i+1 == count){
      printf("\n");
      count = 2*count+1;
    }
  }
  printf("\n\n");
}

void print_hp_list(Node * node){
  while(node != NULL){
    HeapPoint * hp = node -> len_list;
    if(hp != NULL){
    printf("%d: ", node -> label);
    while(hp != NULL){
      printf("[%d,%d, %d]\t", (hp -> heaper) -> node1 -> label, (hp -> heaper) -> node2 -> label, hp -> heaper -> loc);
      hp = hp -> next;
    }
    printf("\n");
    }
     node = node -> next;
  }
  printf("\n");
}

void print_binary(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  print_binary(fp, head->left);
  print_binary(fp, head-> right);
  if(head -> label == -1){
    int neg = -1;
    fwrite(&neg, sizeof(neg), 1, fp);
    fwrite(&(head -> wire_l), sizeof(head -> wire_l), 1, fp);
    fwrite(&(head -> wire_r), sizeof(head -> wire_r), 1, fp);
    fwrite(&(head -> x), sizeof(head -> x), 1, fp);
    fwrite(&(head -> y), sizeof(head -> y), 1, fp);	   
  }else{
    fwrite(&(head -> label), sizeof(head -> label), 1, fp);
    fwrite(&(head -> c), sizeof(head -> c), 1, fp);
    fwrite(&(head -> x), sizeof(head -> x), 1, fp);
    fwrite(&(head -> y), sizeof(head -> y), 1, fp);
  } 
}

void print_spicey(FILE * fp, Node * head){
  if(head == NULL){
    return;
  }
  print_spicey(fp, head->left);
  print_spicey(fp, head->right);
  if(head -> label == -1){
    fwrite(&(head->label), sizeof(head->label), 1, fp);
    fwrite(&(head -> wire_l), sizeof(head -> wire_l), 1, fp);
    fwrite(&(head -> wire_r), sizeof(head -> wire_r), 1, fp);
    fwrite(&(head -> x), sizeof(head -> x), 1, fp);
    fwrite(&(head -> y), sizeof(head -> y), 1, fp);    
    fwrite(&(head -> parallel), sizeof(head -> parallel), 1, fp);
  }else{
    fwrite(&(head -> label), sizeof(head -> label), 1, fp);
    fwrite(&(head -> c), sizeof(head -> c), 1, fp);
    fwrite(&(head -> x), sizeof(head -> x), 1, fp);
    fwrite(&(head -> y), sizeof(head -> y), 1, fp);

  } 
}
