//The code is from previous labs and lab7 that I wrote
//Author: me - Yuan Wang
//Date accessed: March 24, 2022


#include "bintree.h"
#include "queue.h"
#include "stack.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


BinaryTree* bintree_initialize(int typeSize, char* typeName, 
		int (*compFunc)(void*, void*), void (*printFunc)(void*))
{
  //Allocate memory for the tree
  BinaryTree *btree = malloc(sizeof(BinaryTree));

  //Set appropriate values to its field
  btree -> top = NULL;
  btree -> itemSize = typeSize;

  btree -> type = malloc(sizeof(char) * sizeof(strlen(typeName)));
  strcpy(btree -> type, typeName);

  //Set the functions
  btree -> compare = compFunc;
  btree -> print = printFunc;
}


BinaryTreeNode* bintree_create_node(int typeSize, void* elem)
{
  //Allocate memory for the binary tree node
  BinaryTreeNode* btNode = malloc(sizeof(BinaryTreeNode));

  //Set left and right children as null
  btNode -> left = NULL;
  btNode -> right = NULL;

  //Copy the data value
  btNode -> data = malloc(sizeof(elem));
  memcpy(btNode -> data, elem, typeSize);

  return btNode;
}


bool bintree_insert(BinaryTree* btree, void* elem)
{
  //If tree or element is null, return false
  if(btree == NULL || elem == NULL) return false;

  //If the root node is null, set the node as the root
  if(btree -> top == NULL)
  {
    //Use the element to create a tree node
    btree -> top = bintree_create_node(btree->itemSize,elem);

    return btree -> top != NULL;
  }
  
  //Otherwise, insert the node to somewhere else
  else
    return _bintree_insert_recursive (btree, btree -> top, elem);

}


bool bintree_search(BinaryTree* btree, void* elem)
{
  //If tree or element is null, nothing to search
  if(btree == NULL || elem == NULL) return false;

  return _bintree_search_recursive(btree, btree -> top, elem);
}


void bintree_print_in_order(BinaryTree* btree)
{
  //If tree is null, nothing to print
  if(btree == NULL) return;

  _bintree_in_order_recursive(btree, btree -> top);
}


void bintree_print_pre_order(BinaryTree* btree)
{
  //If tree is null, nothing to print
  if(btree == NULL) return;

  _bintree_pre_order_recursive(btree, btree -> top);
}


void bintree_print_post_order(BinaryTree* btree)
{ 
  //If tree is null, nothing to print
  if(btree == NULL) return;

  _bintree_post_order_recursive(btree, btree -> top);
}


void bintree_print_reverse_order(BinaryTree* btree)
{
  //If tree is null, nothing to print
  if(btree == NULL) return;

  _bintree_reverse_order_recursive(btree, btree -> top);
}


bool _bintree_insert_recursive(BinaryTree* btree, BinaryTreeNode* btNode, void* elem)
{
  //Compute the comparing result between the node's data and the element
  int compare = btree -> compare(btNode -> data, elem);


  //If the element is bigger or equal, go to the right
  if(compare >= 0)
  {
    //If no child on the right, make the element as the child
    if(btNode -> right == NULL)
    {
      btNode -> right = bintree_create_node(btree -> itemSize, elem);
      return btNode -> right != NULL;
    }

    //Otherwise, keep going down the right
    _bintree_insert_recursive(btree, btNode -> right, elem);
  }
  

  //If the element is smaller, go to the left
  else
  {
    //If no child on the left, make the element as the child
    if(btNode -> left == NULL)
    {
      btNode -> left = bintree_create_node(btree -> itemSize, elem);
      return btNode -> left != NULL;
    }

    //Otherwise, keep going down the left
    _bintree_insert_recursive(btree, btNode -> left, elem);
  }
  

  //If the steps above did not succeed, return false
  return false;
}


bool _bintree_search_recursive(BinaryTree* btree, BinaryTreeNode* btNode, void* elem)
{
  //If reached null and still not found, the element is not on the tree
  if(btNode == NULL) return false;

  //Compute the comparing result
  int compare = btree -> compare(btNode -> data, elem);

  //If the element is bigger, go to the right
  if(compare > 0) 
  {
    _bintree_search_recursive(btree, btNode -> right, elem);
  }

  //If the element is smaller, go to the left
  else if (compare < 0) 
  {
    _bintree_search_recursive(btree, btNode -> left, elem);
  }

  //Otherwise, the element matches the data, element found
  else 
    return true;
}


void _bintree_in_order_recursive(BinaryTree* btree, BinaryTreeNode* btNode)
{
  //If reached null, exit
  if(btNode == NULL) return;


  //Otherwise, traverse the tree in inorder: left - root - right
  _bintree_in_order_recursive(btree, btNode -> left);
  btree -> print(btNode -> data);
  _bintree_in_order_recursive(btree, btNode -> right);
}


void _bintree_pre_order_recursive(BinaryTree* btree, BinaryTreeNode* btNode)
{
  //If reached null, exit
  if(btNode == NULL) return;


  //Otherwise, traverse the tree in preorder: root - left - right
  btree -> print(btNode -> data);
  _bintree_pre_order_recursive(btree, btNode -> left);
  _bintree_pre_order_recursive(btree, btNode -> right);
}


void _bintree_post_order_recursive(BinaryTree* btree, BinaryTreeNode* btNode)
{  
  //If reached null, exit
  if(btNode == NULL) return;


  //Otherwise, traverse the tree in post order: left - right - root
  _bintree_post_order_recursive(btree, btNode -> left);
  _bintree_post_order_recursive(btree, btNode -> right);
  btree -> print(btNode -> data);
}


void _bintree_reverse_order_recursive(BinaryTree* btree, BinaryTreeNode* btNode)
{ 
  //If reached null, exit
  if(btNode == NULL) return;


  //Otherwise, traverse the tree in reversed inorder: right - root - left
  _bintree_reverse_order_recursive(btree, btNode -> right);
  btree -> print(btNode -> data);
  _bintree_reverse_order_recursive(btree, btNode -> left);

}


void bintree_print_breadth_first(BinaryTree* btree)
{
  //If the tree is null, nothing to travese
  if(btree == NULL) return;

  //Initialize a queue that stores binary tree nodes
  Queue* q = queue_initialize(sizeof(BinaryTreeNode) ,"BinaryTreeNode");

  //Put the root node to the queue
  queue_enqueue(q, btree -> top);

  BinaryTreeNode* curNode = NULL;

  //If the queue has nodes, the travesal is not done
  while(queue_size(q))
  { 
    //Dequeue the queue, store the node in curNode
    curNode = queue_dequeue(q);

    //Put the node's children to the queue
    queue_enqueue(q, curNode -> left);
    queue_enqueue(q, curNode -> right);

    //Print out the node's data value
    btree -> print(curNode -> data);
  }
}


void bintree_print_depth_first(BinaryTree* btree)
{
  //If the tree is null, nothing to travese
  if(btree == NULL) return;

  //Initialize a stack to store binary tree nodes
  Stack* s = stack_initialize(sizeof(BinaryTreeNode), "BinaryTreeNode");

  //Push the tree's root node to the stack
  stack_push(s, btree -> top);

  BinaryTreeNode* curNode = NULL;

  //If the stack has nodes, the travesal is not done
  while(stack_size(s))
  {
    //Pop the stack, store the node in curNode
    curNode = stack_pop(s);

    //Push the node's children to the stack
    stack_push(s, curNode -> right);
    stack_push(s, curNode -> left);

    //Print out the node's data value
    btree -> print(curNode -> data);
  }
}


bool bintree_insert_replace(BinaryTree* btree, void* elem)
{
  //If the tree or element is null, nothing to insert
  if(btree == NULL || elem == NULL) return false;

  //If the root node is null, create a node as the root
  if(btree -> top == NULL)
  {
    btree -> top = bintree_create_node(btree -> itemSize, elem);

    return btree -> top != NULL;
  }

  return _bintree_insert_replace_recursive(btree, btree -> top, elem);
}


bool _bintree_insert_replace_recursive(BinaryTree* btree, BinaryTreeNode* btNode, void* elem)
{
  //Compute the comparing result between the node's data and the element
  int compare = btree -> compare(btNode -> data, elem);

  //If the element is greater, go to the right
  if(compare > 0)
  {
    //If no child on the right, make the element as the child
    if(btNode -> right == NULL)
    {
      btNode -> right = bintree_create_node(btree -> itemSize, elem);
      return btNode -> right != NULL;
    }


    //Otherweise, keep searching
    _bintree_insert_replace_recursive(btree, btNode -> right, elem);
  }

  //If the element is smaller, go to the left
  else if(compare < 0)
  {
    //If no child on the left, make the element as the child
    if(btNode -> left == NULL)
    {
      btNode -> left = bintree_create_node(btree -> itemSize, elem);
      return btNode -> left != NULL;
    }


    //Otherwise, keep searching
    _bintree_insert_replace_recursive(btree, btNode -> left, elem);
  }

  //If the element matches the data value
  else
  {
    //Replace the data with the element
    memcpy(btNode -> data, elem, btree -> itemSize);

    return !btree -> compare(btNode -> data, elem);
  }

}

