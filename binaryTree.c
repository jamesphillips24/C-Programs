#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* COP 3502C Assignment 5
This program is written by: James Phillips */
/*I want to be considered for the bonus and I agree that any of my submissions
after July 22 will not be considered for grading regardless of my score on the most recent
submission before July 22*/
// 7/22/24
// Program to create a binary tree and implement useful
// functions like adding and deleting.

// NOTE, PLEASE READ: For the deleting, I made a shell function that calls seperate functions
// for the 3 delete cases (leaf node, 1 child, 2 children) instead of doing it all in the same
// delete function. It is doing exactly the same thing as the class recording code, except all 
// the code that would be in the if statements are just called to seperate functions. I wrote
// them from scratch before I went back and saw point 3 in the implementation requirements
// which is why I did it differently.

typedef struct Node{
  char name[26];
  int points;
  struct Node* left;
  struct Node* right;
} Node;

// Used the sorted nature of the binary tree to quickly
// find an element.
Node* search(Node* root, char* name, int* depth){
  if(root == NULL)
    return NULL;
  if(strcmp(root->name, name) == 0)
    return root;
  else if(strcmp(root->name, name) > 0){
    (*depth)++;
    return search(root->left, name, depth);
  }
  else{
    (*depth)++;
    return search(root->right, name, depth);
  }
}

// Same as the search function except look ahead to the
// names of child elements instead.
Node* searchParent(Node* root, Node* node){
  if(root == NULL || root == node)
    return NULL;
  if(root->left == node || root->right == node)
    return root;
  if(strcmp(root->name, node->name) > 0)
    return searchParent(root->left, node);
  else if(strcmp(root->name, node->name) < 0)
    return searchParent(root->right, node);
  return NULL;
}

// Check if node has no children
int isLeaf(Node* node){
  if(node->left == NULL && node->right == NULL)
    return 1;
  return 0;
}

// Check if node only has a left child
int hasOnlyLeftChild(Node* node){
  if(node->left != NULL && node->right == NULL)
    return 1;
  return 0;
}

// Check if node only has a right child
int hasOnlyRightChild(Node* node){
  if(node->left == NULL && node->right != NULL)
    return 1;
  return 0;
}

// Traverse down the right side of the subtree since the
// tree is sorted and therefore the right child must be
// greater than the parent
Node* maxVal(Node* root){
  if(root->right == NULL)
    return root;
  else
    return maxVal(root->right);
}

// Traverse down the left side of the subtree since the
// tree is sorted and therefore the left child must be
// smaller than the parent
Node* minVal(Node* root){
  if(root->left == NULL)
    return root;
  else
    return minVal(root->left);
}

// Similar to the search function, except insert new node
// as a child at the end
Node* insert(Node* root, Node* temp){
  if(root == NULL)
    return temp;

  if(strcmp(root->name, temp->name) == 0){
    root->points += temp->points;
    }
  else if(strcmp(root->name, temp->name) > 0){
    if(root->left != NULL)
      root->left = insert(root->left, temp);
    else
      root->left = temp;
  }
  else{
    if(root->right != NULL)
      root->right = insert(root->right, temp);
    else
      root->right = temp;
  }
  
  return root;
}

// Simply call the insert function, except it must
// check if the name is already in the tree using the
// search function. It also prints.
Node* add(Node* root){
  int depth = 0;
  Node* temp = malloc(sizeof(Node));
  scanf("%s %d", temp->name, &temp->points);

  if(search(root, temp->name, &depth) != NULL){
    depth = 0;
    Node* node = search(root, temp->name, &depth);
    node->points += temp->points;
    free(temp);
    printf("%s %d %d\n", node->name, node->points, depth);
    return root;
  }
  
  root = insert(root, temp);

  depth = 0;
  search(root, temp->name, &depth);
  printf("%s %d %d\n", temp->name, temp->points, depth);
  return root;
}

// Find the name with the search function and then print
// info, unless it's not found then print not found.
void sub(Node* root){
  char name[26];
  int points;
  int depth = 0;
  
  scanf("%s %d", name, &points);
  Node* temp = search(root, name, &depth);
  if(temp == NULL){
    
    printf("%s not found\n", name);
    return;
  }
  
  if(temp->points < points)
    temp->points = 0;
  else
    temp->points -= points;
  
  printf("%s %d %d\n", temp->name, temp->points, depth);
}

// This function is only called under the assumption that 
// node is a leaf, so it isn't checked.
// Look at the children of the parent and whichever one is
// node, delete and set to null.
void deleteLeaf(Node* parent, Node* node) {
  if (parent->left == node)
    parent->left = NULL;
  else if (parent->right == node)
    parent->right = NULL;

  free(node);
}

// Check whether the node has a left child or a right child,
// then check if the node is the root, in which case
// you can just set the child equal to the new root.
// If not, find which side of the parent the node is on
// and then set that child of the parent to node's child.
Node* deleteSingleParent(Node* root, Node* parent, Node* node){
  if(hasOnlyLeftChild(node)){
    if(root == node){
      Node* temp = root->left;
      free(root);
      return temp;
    }
    if(parent->left == node)
      parent->left = parent->left->left;
    else
      parent->right = parent->right->left;
  }
  else if(hasOnlyRightChild(node)){
    if(root == node){
      Node* temp = root->right;
      free(root);
      return temp;
    }
    if(parent->left == node){
      parent->left = parent->left->right;
    }
    else{
      parent->right = parent->right->right;
    }
  }

  free(node);

  return root;
}

// Find the max value on the left side and copy its info
// to the node. Then delete the max value based on if it's
// a leaf or a single parent.
Node* deleteDoubleParent(Node* root, Node* node) {
  Node* max = maxVal(node->left);
  Node* maxParent = searchParent(root, max);

  strcpy(node->name, max->name);
  node->points = max->points;

  if (isLeaf(max)) {
    deleteLeaf(maxParent, max);
  } else {
    root = deleteSingleParent(root, maxParent, max);
  }

  return root;
}

// Shell function to decide what kind of parent the node
// is and calls the respective delete function. Also takes
// care of if the node is the root or not found.
Node* deleteShell(Node* root){
  int depth = 0;
  char name[26];
  scanf("%s", name);
  
  Node* node = search(root, name, &depth);
  if(node == NULL){
    printf("%s not found\n", name);
    return root;
  }
  
  Node* parent = searchParent(root, node);

  printf("%s deleted\n", name);
  if(root == NULL)
    return NULL;
  else if(isLeaf(root)){
    free(root);
    return NULL;
  }
  else if(isLeaf(node))
    deleteLeaf(parent, node);
  else if(hasOnlyLeftChild(node) || hasOnlyRightChild(node)){
    root = deleteSingleParent(root, parent, node);
  }
  else
    root = deleteDoubleParent(root, node);
  return root;
}

// Same height code from the exercise. Traverse the tree
// finding the max height. Called twice for each side.
int height(Node* root) {
  int leftHeight, rightHeight;
  
  if(root == NULL)
    return -1;
  
  leftHeight = height(root->left);
  rightHeight = height(root->right);
  
  if(leftHeight > rightHeight)
    return leftHeight + 1;
  
  return rightHeight + 1;
}

// Go through every node in depth first postorder,
// freeing the memory afterwards.
void freeMem(Node* root){
  if(root != NULL){
    freeMem(root->left);
    freeMem(root->right);
    free(root);
  }
}

// Get input for number of commands and then run, checking
// the input and calling the appropriate function. Then
// free memory.
int main(void) {
  Node* root = NULL;
  Node* temp;
  char choice[26];
  int depth = 0, n;

  scanf("%d", &n);
  
  for(int i = 0; i < n; i++){
    scanf("%s", choice);
    
    if(strcmp(choice, "add") == 0){
      root = add(root);
    }
    else if(strcmp(choice, "sub") == 0){
      sub(root);
    }
    else if(strcmp(choice, "del") == 0){
      root = deleteShell(root);
    }
    else if(strcmp(choice, "search") == 0){
      depth = 0;
      
      scanf("%s", choice);
      temp = search(root, choice, &depth);

      if(temp == NULL)
        printf("%s not found\n", choice);
      else
        printf("%s %d %d\n", temp->name, temp->points, depth);

      strcpy(choice, "reset");
    }
    else if(strcmp(choice, "height_balance") == 0){
      int heightLeft = height(root->left);
      int heightRight = height(root->right);

      printf("left height = %d right height = %d ", heightLeft, heightRight);
      if(heightLeft == heightRight)
        printf("balanced\n");
      else
        printf("not balanced\n");
    }
  }
  freeMem(root);
}