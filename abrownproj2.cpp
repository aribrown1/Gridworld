#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 301
#define true 1 //for bool isEmpty()
#define false 0


typedef struct _Stack{
    int max; //max size of array
    char *dynArray; //pointer to an array of struct variables
    int top; //top of array
    
}_Stack;

void init(_Stack *array) {
    array->max = 2; //starting array is size 2
    array->top = -1; //initialize top to -1
    array->dynArray = (char*)malloc(sizeof(char) * array->max);
}

int isEmpty(_Stack *array){
    if (array->top == -1){ //if stack is empty return true, else return false
        return true;
    }
    else{
        return false;
    }
    return false;
}

int push(_Stack *array, char input, bool debug){
    int i = 0;
    if (input == '\0'){ //if there is no input, do not push anything onto stack
        return 1;
    }

    if (array->top == array->max-1){ //if array is at max size, allocate more memory
        if (debug == true){ //print when we are growing array if debug mode is selected
            printf("Doubling array. \n Old size: %d \n", array->max);
        }
        char *oldArray = array->dynArray;
        array->dynArray = (char*)malloc(sizeof(array->max) * sizeof(char)); //add 2 to size of dynamic array
        for (i=0; i<=array->top; i++){
            array->dynArray[i] = oldArray[i];
        }
        free(oldArray);
        array->max++; //increment max variable size by 2 since we are adding two more spaces for values
        array->max++;
        if (debug == true){ //print when we are growing array if debug mode is selected
            printf("Values copied: %d \n New size: %d \n", i, array->max);
        }
     }
    //increment top and assign last pushed char value to top
    array->top++;
    array->dynArray[array->top] = input;

    return 1;
}

char top(_Stack *array){
    int i;
    for (i= array->max-1; i<-1; i--){ //loop backwards through array by starting at max index
        while (array->dynArray[i] != '\0'){ //loop until array ends
            array->top = array->dynArray[i]; //assign top to last assigned value in array
        }
    }
    
    return array->dynArray[array->top]; //return top char of array
}

void pop(_Stack *array, bool debug){
    if (array->dynArray[array->top] == '\0'){ //if array is empty, print message and do not pop
        printf("Error: Array is empty. No element to pop \n");
        return;
    }
    else { //pop just one element of array
        if (debug == true){ //print when we are popping array if debug mode is selected
            printf("Popping value: %c \n", array->dynArray[array->top]);
        }
        array->dynArray[array->top]='\0';
        array->top--;
        return;
    }
}

void printStack(_Stack *array){ 
    int i;
    if (array->dynArray[array->top] == '\0'){ //if array is empty, just return
        return;
    }
    for (i=0; i<array->max; i++){ //while loop does not go over max val in stack, print array
        printf(" %c ", array->dynArray[i]);
    }
}

void clearArr(_Stack *array){ //function to clear array and return it to starting size
    
    free(array->dynArray);
    printStack(array);
    init(array); //reinitialize array to original size
    
}

char compare(char array, char b){//compare opening symbol 'a' and closing symbol 'b' to see if they are balanced
    char a = array;
    
    switch(a){
        case '{':
            if (b=='}'){ return b; }
            else if (b=='\0'){ return 'm'; }
            else{ return 'c'; }
            break;
        case '[':
            if (b==']'){ return b; }
            else if (b=='\0'){ return 'm'; }
            else{ return 'c'; }
            break;
        case '(':
            if (b==')'){ return b; }
            else if (b=='\0'){ return 'm'; }
            else{ return 'c'; }
            break;
        case '<':
            if (b=='>'){ return b; }
            else if (b=='\0'){ return 'm'; }
            else{ return 'c'; }
            break;
        case '\0':
            if (b == '}' || b == '>' || b == ')' || b== ']'){ return '1'; } //return 1 if missing opening symbol and closing symbol is present
    }
    return '\0';
}


int main (int argc, char** argv)
{
    //create and initialize instance of struct
    _Stack myStack;
    init(&myStack);
    
    bool debug = false; //for debug mode, initialize to false
    char missingsymbol; //var to save missing symbol
    char checkingsymbol; //var to save checking symbol
    
    char input[MAX];
    /* set up an infinite loop */
    while (1){
        /* get line of input from standard input */
        printf ("\nEnter input to check or q to quit\n");
        fgets(input, 300, stdin); //gets 300 chars of input
    
        int i = 0; //counter
        char balance; //variable for results of 'balance' function
        while (input[i]!= '\0'){ //keep looping to push all inputs until user enters 'q' or 'Q'
            
            if (input[i] == 'q' | input[i] == 'Q'){ return 0; } //quit if user enter q or Q
            if (input[i] == 'c' | input[i] == 'C'){ break; } //if user wants to clear array
            if(input[i] == '-' && input[i+1] == 'd'){ debug= true; printf("Debugging Information \n"); } //turn debug mode on
       
            if(input[i]=='{'|| input[i]=='['|| input[i]=='(' || input[i]== '<' || input[i] == '\0'){ //if input[i] is opening, push
                if (debug == true){ printf("Pushing opening bracket: %c \n", input[i]); } //print pushing info if debug is true
                push(&myStack, input[i], debug); //push opening brackets onto stack
                checkingsymbol = input[i]; //save opening symbol as checkingsymbol to use later in conditional statement
            }
            else if(input[i]=='}'|| input[i]==']'|| input[i]==')' || input[i]=='>' || input[i]== '\0'){ //if input is closing symbol, compare
        
                balance = compare(myStack.dynArray[myStack.top],input[i]); //if stack is mismatching closing symbol, call compare function to find error
                   if (balance=='c'){ //if stack has different closing symbol
                       printf("Error 1: NOT balanced. Expecting different closing symbol. \n");
                       if (myStack.dynArray[myStack.top]== '{'){ missingsymbol = '}';} //assign missingsymbol to correct bracket to display in 'missing' statement
                       if (myStack.dynArray[myStack.top] == '['){ missingsymbol = ']';}
                       if (myStack.dynArray[myStack.top] == '('){ missingsymbol = ')';}
                       if (myStack.dynArray[myStack.top]  == '<'){ missingsymbol = '>';}
                       break;
                    }
                    else if ( balance =='1' ){ //if stack is missing opening symobl
                        printf("Error 2: NOT balanced. Expression is missing opening symbol. \n");
                        if (input[i] == '}'){ missingsymbol = '{';} //if there is no opening symbol, use input[i] instead of checksingymbol
                        if (input[i] == ']'){ missingsymbol = '[';} //missing symbol will be an opening bracket to display in 'missing' statement
                        if (input[i] == ')'){ missingsymbol = '(';}
                        if (input[i] == '>'){ missingsymbol = '<';}
                        break;
                    }
        
                    if (balance== '>' || balance == ')' || balance=='}' || balance==']'){ //if values are balanced, pop bracket 
                    pop(&myStack, debug); //need to pass in stack and pop off top

                }
            }
        i++; //increment to next value of input
            
        } //end of user input loop
        
        if (input[i] == 'c' || input[i] == 'C'){ //if user enters clear
            printf("Now clearing \n");
            clearArr(&myStack);
            continue; //allow user to enter more input
        }
        
        balance = compare(myStack.dynArray[myStack.top], input[i]); //call balance function again to check for error 3

        if (isEmpty(&myStack) == true && balance!='1'){ //if stack is empty after loop AND there are no closing symbols, expression is balanced
            printf("Expression is balanced. \n");
        }
        if (balance == 'm'){
             if (isEmpty(&myStack) == false && myStack.dynArray[1] == '\0'){ //if there is only one element left and there isnt a closing bracket
                checkingsymbol = myStack.dynArray[0];
            }
            printf("Error 3: NOT balanced. Expression is missing closing symbol. \n");
            if (myStack.dynArray[myStack.top] == '{'){ missingsymbol = '}';} //save missing symbol to use in 'missing' statement
            if (myStack.dynArray[myStack.top] == '['){ missingsymbol = ']';}
            if (myStack.dynArray[myStack.top] == '('){ missingsymbol = ')';}
            if (myStack.dynArray[myStack.top] == '<'){ missingsymbol = '>';}
        }
        
        int j = 0;  //use to loop through array instead of i since i has saved index of error bracket
        int k = 0;  //counter for loop 
        
        char missing[myStack.max]; //create an array to print out spaces for 'missing' statement
        while (input[j]!= '\0'){ //initialize 'missing' array values all to spaces. this will space out 'missing' statement properly
            missing[j] = ' ';
            j++;
        }

        j= 0; //reset j back to zero for next loop
        
        if (balance!= '>' && balance != ')' && balance!='}' && balance!=']'){ //if array is not balanced, show error
            while(input[j]!='\0'){ //print out brackets in stack
                printf("%c", ' '); //space out brackets for easier reading
                printf("%c", input[j]);
                j++;
            }
            
            //conditional statements to print out missing/expecting statements
            if ( balance == 'm'){
                for (k=0; k<= i; k++){ //print out missing[] (array of spaces) for correct ^ spacing
                    printf("%c ", missing[k]);
                }
                printf("^missing closing symbol %c \n", missingsymbol); //TO DO return correct missingsymbol
            }
            if ( balance == '1'){
                for (k=0; k<= i; k++){ //print out missing[] (array of spaces) for correct ^ spacing
                    printf("%c ", missing[k]);
                }
                printf("^missing opening symbol %c \n", checkingsymbol);
            }
            if ( balance == 'c'){
                for (k=0; k<= i; k++){ //print out missing[] for correct ^ spacing
                    printf("%c ", missing[k]);
                }
                printf("^expecting different closing symbol %c \n", missingsymbol);
            }
        }
        
        input[i] = '\0';

    }

    return 0;
}