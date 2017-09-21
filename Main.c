/*
 * Main.c
 *
 *  Created on: 26/04/2017
 *      Author: is708596
 */

#include "Main.h"


int main(){
	setbuf(stdout, NULL);
	Bool wf = TRUE;					//WELL-FORMED FORMULA
	printf("*** ANALIZADOR DE FÓRMULAS BIEN FORMADAS ***\n");
	printf("Introduce una fórmula: ");
	char buffer[100];
	gets(buffer);
	pause();
	Queue q1 = stringToQueue(buffer);
	if(q1 == NULL){
		printf("SYNTAX ERROR - CARACTERES INVÁLIDOS\n");
		return 0;
	}

	//// Create one queue to hold the array
	Stack A_Stack = stack_create();
	Stack B_Stack = stack_create();

	TreeNode temp, temp2, temp3, temp4, root;

	//HERE IT BEGINS
	while ((!queue_isEmpty(q1) || !stack_isEmpty(A_Stack)) && wf){

	//PUSH MODE
		temp = (struct TreeNode*) queue_poll(q1);
		while (!queue_isEmpty(q1) && temp->type != CLOSE_PA){
			stack_push(A_Stack, temp);
			temp = (struct TreeNode*) queue_poll(q1);
		}
		if(!(temp == NULL))
			stack_push(A_Stack,temp);

	//POP MODE
	    do{
	    	temp2 = (struct TreeNode*) stack_pop(A_Stack);
	    	if(temp2 != NULL){
				//All of this conditions are pushing the top data from A stack to the B stack
				//When is an atomic proposition by itself
				if (temp2->type == CLOSE_PA || temp2->isComplete == TRUE)
					stack_push(B_Stack, temp2);

				//When enter negation condition
				else if(temp2->type == UNARY){
					if(stack_isEmpty(B_Stack)) wf = FALSE;
					else{
						temp3 = (struct TreeNode*) stack_pop(B_Stack);
						if(temp3->isComplete){
							temp2->right = temp3;
							temp2->isComplete = TRUE;
							stack_push(B_Stack,temp2);
						}
						else wf = FALSE;
					}
				}

				//When evaluating a binary expression
				else if (temp2->type == BINARY){
					if(stack_isEmpty(B_Stack) || stack_isEmpty(A_Stack))wf = FALSE;
					else{
						temp3 = (struct TreeNode*) stack_pop(B_Stack);
						temp4 = (struct TreeNode*) stack_pop(A_Stack);
						if(temp3->isComplete && temp4->isComplete){
							temp2->left  = temp4;
							temp2->right = temp3;
							temp2->isComplete = TRUE;
							stack_push(B_Stack,temp2);
						}
						else wf = FALSE;
					}
				}

				//When evaluating the node the open parentheses
				else if(temp2->type == OPEN_PA){
					if(stack_isEmpty(B_Stack))
						wf = FALSE;
					else{
						temp3  = (struct TreeNode*) stack_pop(B_Stack);
						if(temp3->isComplete){
							if(stack_isEmpty(B_Stack))
								wf = FALSE;
							else{
								temp4 = (struct TreeNode*) stack_pop(B_Stack);
								if(temp4->type == CLOSE_PA)
									stack_push(A_Stack,temp3);
								else
									wf = FALSE;
							}
						}else
							wf = FALSE;
					}
				}
	    	}
	    	else{
	    		wf = FALSE;
	    	}
			if(stack_isEmpty(A_Stack))
				root = (struct TreeNode*) stack_pop(B_Stack);
	    }while(!stack_isEmpty(B_Stack) && wf);
	}

	if(!wf){
		printf("SYNTAX ERROR - FÓRMULA MAL FORMADA\n");
		return EXIT_FAILURE;
	}
	printf("\n\n* FÓRMULA BIEN FORMADA *\n");
	printf("Nomenclatura polaca inversa: ");
	printPostfix(root);
	printf("\n");
	printf("Árbol binario generado:\n");
	printTree(root, 2);
	return 0;
}
