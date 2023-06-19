#include "centralqueue.h"



// Insert a node or multiple nodes as a root into the double
// linked list of roots.
// Returns the new most prior node.
CentralQueueNode *CentralQueueNode::insert(CentralQueueNode *node) {
  //          o---o---o
  //          |   |\  |
  // insert   |   o o o
  // new node |     |
  //          |     o
  //          v
  // ...o---o---o...
  //        ^
  //        |
  //  prior_node
  this->left->right = node->right;
  node->right->left = this->left;
  this->left = node;
  node->right = this;

  // Return the prior node.
  if (this->is_prior(node)) {
    return this;
  } else {
    return node;
  }

  // If there exists roots with the same degree.
}

// Remove a node from the Fibonacci heap.
// Notice that this function only removes the node but
// not its neighbors or its children.
// Returns:
//    1. The new prior node if the node to be removed is 'this';
//    3. Otherwise return 'this'.
CentralQueueNode *CentralQueueNode::remove_node(CentralQueueNode *node) {

  CentralQueueNode *new_handler = this;
  if (this == node) {
    if (this->right != this) {
      new_handler = this->right;
    } else {
      new_handler = this->child;
    }
  }

  // If the node to be removed has child / children:
  if (node->child != NULL) {
    this->insert(node->child);
    node->child = NULL;
    node->degree = 0;
  }

  // If the node to be removed has parents:
  if (node->parent != NULL) {
    if (node->parent->child == node) {
      node->parent->child = node->right == node ? NULL : node->right;
    }
  }

  // If the node to be removed has neighbors:
  if (node->right != node) {
    node->left->right = node->right;
    node->right->left = node->left;
    node->left = node->right = node;
  }

  // Consolidate.
  return new_handler->consolidate();
}

// Cut the subtree rooted at 'root' from 'this'.
// Returns:
//    1. NULL if 'this' is the node to be removed and 'this' is the only node on
//       the linked list;
//    2. The node to the right of 'this' if 'this' is the node to be removed
//       and 'this' is not the only node on the linked list;
//    3. Otherwise, returns 'this'.
CentralQueueNode *CentralQueueNode::remove_tree(CentralQueueNode *root) {
  if (root == root->right && root == this) {
    return NULL;
  }

  CentralQueueNode *new_handler = root == this ? this->right : this;
  root->left->right = root->right;
  root->right->left = root->left;
  root->left = root->right = root;
  return new_handler;
}

// Consolidate the heap, return the pointer to the new consolidated heap.
CentralQueueNode *CentralQueueNode::consolidate() {

  // Use a hashmap to store the degrees of the Fibonacci heap.
  hashmap<int, CentralQueueNode *> degree_map;

  // Traverse and remove the trees one by one along the linked list.
  CentralQueueNode *old_heap = this;
  CentralQueueNode *new_heap = NULL;
  CentralQueueNode *removed_root = NULL;
  // While the old heap is not empty.
  while (old_heap != NULL) {
    // Remove the root node with the highest priority from the old heap.
    removed_root = old_heap;
    old_heap = old_heap->remove_tree(old_heap); // NOTICE: old_heap is not the
                                             // most prior node in the old
                                             // heap.
    // While there exists a root in the new heap with
    // the same degree as the removed tree root.
    while (degree_map.member(removed_root->degree)) {
      CentralQueueNode *root_same_deg =
          degree_map.retrieve(removed_root->degree);
      // Remove that node with the same degree from the new heap then
      // merge it with the removed node.
      new_heap = new_heap->remove_tree(root_same_deg); // NOTICE: new_heap is not
                                                    // the most prior node
                                                    // in the new heap.
      degree_map.remove(root_same_deg->degree);
      if (root_same_deg->is_prior(removed_root)) {
        root_same_deg->merge(removed_root);
        removed_root = root_same_deg; // Ensure 'removed_root'
                                      // is the most prior node
                                      // in the merging heap.
      } else {
        removed_root->merge(root_same_deg);
      }
    }
    // Merge complete, insert the merged node into the new heap.
    if (new_heap == NULL) {
      new_heap = removed_root;
    } else {
      new_heap = new_heap->insert(removed_root);
    }
    degree_map.add(removed_root->degree, removed_root);
  }

  return new_heap;
}

// Merges node_2 as a child of node_1.
// Change the child pointer of node_1 to node_2
// if node_2 is prior to any of the previous children of node_1.
void CentralQueueNode::merge(CentralQueueNode *node) {
  node->parent = this;
  if (this->child == NULL) {
    this->child = node;
  } else {
    this->child->left->right = node;
    node->left = this->child->left;
    this->child->left = node;
    node->right = this->child;
  }

  if (node->is_prior(this->child)) {
    this->child = node;
  }

  ++this->degree;
}

// Return the degree of the node.
int CentralQueueNode::get_degree() { return this->degree; }

// Check if 'this' has greater priority than 'node'.
bool CentralQueueNode::is_prior(CentralQueueNode* node) {
  if (this->registration->get_risk() == 3 && node->registration->get_risk() < 3) {
    return false;
  } else if (node->registration->get_risk() && this->registration->get_risk() < 3) {
    return true;
  }
  
  if (this->registration->get_profession() > node->registration->get_profession()) {
    return true;
  } else if (node->registration->get_profession() > this->registration->get_profession()) {
    return false;
  }

  if (this->registration->get_age() > node->registration->get_age()) {
    return true;
  } else if (node->registration->get_age() > this->registration->get_age()) {
    return false;
  }

  if (this->registration->get_timestamp()->earlier(node->registration->get_timestamp())) {
    return true;
  } else if (node->registration->get_timestamp()->earlier(this->registration->get_timestamp())) {
    return false;
  } else {
    return (this->registration->get_id() > node->registration->get_id() ? true : false);
  }
}

// Inspect a Fibonacci node.
void CentralQueueNode::inspect() {
  cout << "registration id: " << this->registration->get_id() << endl;
  cout << "neighborhood id:";
  CentralQueueNode *traverse = this->right;
  int count = 0;
  while (traverse != this && count < 10) {
    cout << " " << traverse->registration->get_id() << " ";
    traverse = traverse->right;
    ++count;
  }
  cout << endl;
  cout << "degree: " << this->degree << endl;
  if (this->child != NULL) {
    cout << "child id: " << this->child->registration->get_id() << endl;
  } else {
    cout << "child id: NULL" << endl;
  }
  cout << endl;
}





int node_cmp_age(node_t* m, node_t* n){
    if( m->reg->get_age() > n->reg->get_age()){
        return 1;
    }
    if( m->reg->get_age() < n->reg->get_age()){
        return -1;
    }
    return 0;
}

int node_cmp_prof(node_t* m, node_t* n){
    if( m->reg->get_profession() > n->reg->get_profession()){
        return 1;
    }
    if( m->reg->get_profession() < n->reg->get_profession()){
        return -1;
    }
    return 0;   
}

int node_cmp_name(node_t* m, node_t* n){
    string name_m  = m->reg->get_name();
    string name_n  = n->reg->get_name();
    if( name_m.compare(name_n) ==-1){
        return 1;
    }
    if( name_m.compare(name_n) == 1){
        return -1;
    }
    return 0;
}

/* Merge sort a double linked list.
 * Args:    (node_t**) head_ptr:      Pointer to 'node_t' head ptr.
 *          (func ptr) cmp(m, n):   Callback func that compares
 *                                  two nodes in the list,
 *                                  receives two pointers 'm' and 'n',
 *                                  return 1 if '*m' > '*n',
 *                                  -1 if '*m' < '*n',
 *                                  0 if '*m' == '*n'.
 * Return:  void.
 * Effects: Change the linked list and the head pointer in place
 */
void merge_sort(node_t **head_ptr, int (*cmp)(node_t *m, node_t *n)) {

  /* Stopping condition. */
  if (NULL == *head_ptr || NULL == (*head_ptr)->next || NULL == cmp) {
    return;
  }

  /* Split into two sublists, then sort recursively. */
  node_t *list_1 = NULL;
  node_t *list_2 = NULL;
  dl_split(*head_ptr, &list_1, &list_2);
  merge_sort(&list_1, cmp);
  merge_sort(&list_2, cmp);

  /* Merge two sorted sublists. */
  *head_ptr = dl_merge(list_1, list_2, cmp);
}

/* Slipt a double linked list from the middle.
 * Args:    (node_t*) head:       Pointer to the head of the list.
 *          (node_t**) list_1:    Ptr to the head ptr of the first sublist.
 *          (node_t**) list_2:    Ptr to the head ptr of the second sublist.
 * Return:  void.
 * Effects: Directly split the linked list.
 *          Changes '*list_1' and '*list_2'.
 */
void dl_split(node_t *head, node_t **list_1, node_t **list_2) {

  node_t *fast = head;
  node_t *slow = head;

  while (NULL != fast) { // Use fast-slow ptrs to find middle.
    fast = fast->next;
    if (NULL != fast) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *list_1 = head;
  *list_2 = slow;
  slow->prev->next = NULL;
  slow->prev = NULL;
}

/* Merge two sorted linked lists in ascending order.
 * Args:    (node_t*) list_1:         Head ptr of the first list.
 *          (node_t*) list_2:         Head ptr of the second list.
 *          (func ptr) cmp(m, n):   Callback func,
 *                                  receives two ptrs 'm' and 'n'.
 *                                  return 1 if '*m' > '*n',
 *                                  -1 if '*m' < '*n',
 *                                  0 if '*m' == '*n'.
 * Return:  (node_t*) head:       Head ptr to the merged list.
 * Effects: Changes two sublists directly.
 */
node_t *dl_merge(node_t *list_1, node_t *list_2, int (*cmp)(node_t *m, node_t *n)) {

  if (NULL == list_1) {
    return list_2;
  }
  if (NULL == list_2) {
    return list_1;
  }

  node_t *head = NULL;      // Head of the merged list.
  node_t *tail = NULL;      // Tail of the merged list.
  node_t **modifier = NULL; // Pointer to list_1 or list_2.

  /* Determine the head. */
  modifier = (1 == cmp(list_1, list_2) ? &list_2 : &list_1);
  head = *modifier;
  tail = head;
  *modifier = (*modifier)->next;

  /* Merge the rest. */
  while (NULL != list_1 && NULL != list_2) {
    tail->next->prev = NULL;

    modifier = (1 == cmp(list_1, list_2) ? &list_2 : &list_1);
    tail->next = *modifier;
    if (NULL != (*modifier)->prev) {
      (*modifier)->prev->next = NULL;
    }
    (*modifier)->prev = tail;
    tail = *modifier;
    (*modifier) = (*modifier)->next;
  }

  /* list_1 ran out or list_2 ran out. */
  /* (tail must be list_1->prev or list_2->prev) */
  modifier = (NULL == list_1 ? &list_2 : &list_1);
  tail->next = *modifier;
  if (NULL != (*modifier)->prev) {
    (*modifier)->prev->next = NULL;
  }
  (*modifier)->prev = tail;

  return head;
}


/* Prints a linked list of 'node_t's from head to tail.
 * Args:    (node_t*) head: Head ptr of the linked list.
 * Return:  void.
 * Effects: Prints on the screen.
 */
void print_node_list(node_t *head) {
  if (NULL == head) {
    return;
  }
  for (; NULL != head->next; head = (node_t *)head->next) {
    cout << head->reg << endl;
  }
  cout << head->reg << endl;
}

/* Insert a new element to the head of a linked list.
 * Args:    (node_t**) head_ptr:  Pointer to the head ptr of the list.
 *          (node_t*) elt:        Pointer to the new element.
 * Return:  void.
 * Effects: Direcly change the head pointer.
 */
void dl_insert(node_t **head_ptr, node_t *list_elt) {
  if (NULL != *head_ptr) {
    (*head_ptr)->prev = list_elt;
  }
  list_elt->next = *head_ptr;
  list_elt->prev = NULL;
  *head_ptr = list_elt;
}

void free_node_list (node_t** head_ptr) {
    if (NULL == *head_ptr) { return; }
    while (NULL != *head_ptr) {
        node_t* next = (*head_ptr)->next;
        delete *head_ptr;
        *head_ptr = (node_t*) next;
    }
}
