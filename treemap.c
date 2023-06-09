#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
  new->root=NULL;
  new->current=NULL;
  new->lower_than = lower_than;
  return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode *new= createTreeNode(key, value);
  TreeNode *aux2= tree->current;
  if(tree->root==NULL){
    tree->root=new;
    aux2=new;
  }
  else{
    aux2=tree->root;
    while(aux2!=NULL){
      if(is_equal(tree, key, aux2->pair->key)) return;
      if(tree->lower_than(key,aux2->pair->key)==1){
        if(aux2->left==NULL){
          aux2->left=new;
          new->parent=aux2;
        }
        aux2=aux2->left;
      }
      else{
        if(aux2->right==NULL){
          aux2->right=new;
          new->parent=aux2;
        }
        aux2=aux2->right;
      }
      tree->current=aux2;
    }
  }
}

TreeNode * minimum(TreeNode * x){ 
  while(x->left!=NULL){
    x=x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(node->left==NULL&&node->right==NULL){
    if (node->parent != NULL){
      if(node->parent->left == node){
        node->parent->left = NULL;
      }
      else{
        node->parent->right = NULL;    
      }
    }
    else{
      tree->root = NULL;
    }
  }
  else if(node->left==NULL||node->right==NULL){
    if(node->parent!=NULL){
      if(node->parent->left==node){
        node->parent->left=node->right;
      }
      else{
        node->parent->left=node->right;
      }
      if(node->parent->right==node){
        node->parent->right=node->left;
      }
      else{
        node->parent->right=node->left;
      }
    }
    else{
      if(node->left==NULL)tree->root=node->right;
      if(node->right==NULL)tree->root=node->left;
    }
    if(node->left==NULL)node->right->parent=node->parent;
    if(node->right==NULL)node->left->parent=node->parent;
    free(node->pair);
    free(node);
  }
  else{
    TreeNode *min=minimum(node->right);
    node->pair->key= min->pair->key;
    node->pair->value= min->pair->value;
    removeNode(tree,min);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree->root;
  tree->current=aux;
  if(tree->root==NULL)return NULL;
  while(aux!=NULL){
    if(is_equal(tree,aux->pair->key,key)){
      return (aux->pair);
    }
    if(tree->lower_than(aux->pair->key,key)==0){
      aux=aux->left;
    }
    else{
      aux=aux->right;
    }
    tree->current=aux;
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root==NULL)return NULL;
    TreeNode *current=tree->root;
    while (current->left!=NULL){
      current=current->left;
    }
    tree->current=current;
    return current->pair;
}

Pair * nextTreeMap(TreeMap * tree){
    if (tree->current==NULL) return NULL;
    if (tree->current->right!=NULL){
      tree->current = tree->current->right;
      while (tree->current->left != NULL){
        tree->current = tree->current->left;
      }
      return tree->current->pair;
    } 
    else{
      while (tree->current->parent!=NULL&&tree->current->parent->right == tree->current){
        tree->current = tree->current->parent;
      }
      tree->current = tree->current->parent;
      return tree->current ? tree->current->pair : NULL;
    }
}


