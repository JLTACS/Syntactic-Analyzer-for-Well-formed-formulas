/*
 * Main.h
 *
 *  Created on: 27/04/2017
 *      Author: Cursos
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Queue.h>
#include <Stack.h>

typedef enum {
	OPEN_PA,
	CLOSE_PA,
	ATOMIC,
	UNARY,
	BINARY
}NodeTreeType_t;

struct TreeNode{
	char  data;
	struct TreeNode *left, *right;
	NodeTreeType_t type;
	Bool isComplete;
};

//FUNC PROTOTYPES
typedef struct TreeNode* TreeNode;
TreeNode treeNode_create(char);
Queue stringToQueue(char*);
void appPushPopModes();
void printPostfix( struct TreeNode* n);
void printTree(TreeNode, int);
void pause();

TreeNode treeNode_create(char data){
	TreeNode n = (TreeNode) malloc(sizeof(struct TreeNode));
	n->isComplete = FALSE;
	if(data >= 'a' && data <= 'z'){
		n->data = data;
		n->type = ATOMIC;
		n->isComplete = TRUE;
	}else{
		switch(data){
			case '¬':	n->data = data;	n->type = UNARY;	break;
			case '(':	n->data = data;	n->type = OPEN_PA;	break;
			case ')':	n->data = data;	n->type = CLOSE_PA;	break;
			case '&':	n->data = data;	n->type = BINARY;	break;
			case '|':	n->data = data;	n->type = BINARY;	break;
			case '#':	n->data = data;	n->type = BINARY;	break;
			case '$':	n->data = data;	n->type = BINARY;	break;
			default :	return NULL;
		}
	}
	n->left  = NULL;
	n->right = NULL;
	return n;
}

Queue stringToQueue(char* string){
	Queue q1 = queue_create();
	int i;
	for(i = 0; i < strlen(string); i++){
		if(string[i] == '-' && string[i+1] == '>'){
			i++;
			TreeNode n = treeNode_create('$');
				if(n != NULL){
					queue_offer(q1, n);
				} else{
					queue_destroy(q1);
					return NULL;
				}
		}
		else if(string[i] == '<' && string[i+1] == '-' && string[i+2] == '>'){
			i += 2;
			TreeNode n = treeNode_create('#');
			if(n != NULL){
				queue_offer(q1, n);
			} else{
				queue_destroy(q1);
				return NULL;
			}
		}
		else if(string[i] != ' '){
			TreeNode n = treeNode_create(string[i]);
			if(n != NULL){
				queue_offer(q1, n);
			} else{
				queue_destroy(q1);
				return NULL;
			}
		}
	}
	return q1;
}

void printPostfix(struct TreeNode* n) {
	if(n->left  != NULL) printPostfix(n->left);
	if(n->right != NULL) printPostfix(n->right);
	char c = n->data;
	printf("%c", c);
}

void printTree(TreeNode n, int spaces){
	int i;
	for(i = 0; i < spaces; i++)
		printf(" ");
	if(n->data == '#'){
		puts("<->");
		spaces += 2;
	} else if(n->data == '$'){
		puts("->");	 spaces++;
		spaces += 2;
	}else
		printf("%c\n", n->data);
	if(n->left  != NULL) printTree(n->left,  spaces + 1);
	if(n->right != NULL) printTree(n->right, spaces + 1);
}

void pause(){
	printf("Press enter to continue . . .");
	getchar();
}

#endif /* MAIN_H_ */
