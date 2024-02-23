/*Lana Batnij __ 1200308*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 150
//--------------------------------------------------------------
/*TreeNode struct represents a node in the expression tree*/
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
//--------------------------------------------------------------
/*Stack struct for implementing a stack*/
typedef struct {
    int top;
    TreeNode* items[MAX_SIZE];
} Stack;
//--------------------------------------------------------------
/*- Function to initialize the stack
  - by makeing the top of the stack equales -1 which means it
   is emplty*/
void initialize(Stack* stack) {
    stack->top = -1;
}
//--------------------------------------------------------------
/*- Function to check if the stack is empty
  - it will return a 1 if it is empty else will return 0*/
int isEmpty(Stack* stack) {
    return stack->top == -1;
}
//--------------------------------------------------------------
/*- The Push function will add items to the stack
  - Firstly stack will be checked if it is full:
  - if yes then it will print that stack is FULL!
  - else it will add the item to the top of the stack*/
void push(Stack* stack, TreeNode* item) {
    if (stack->top == MAX_SIZE - 1) {
        printf("Stack Overflow\n");
    }
    stack->items[++stack->top] = item;
}
//--------------------------------------------------------------
/*- POP is the Function that will remove the items from the stack
  - First check if it is empty if yes then print it is empty
  - else the item from the top will be removed and function will return it */
TreeNode* pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow\n");
    }
    return stack->items[stack->top--];
}
//--------------------------------------------------------------
/*- This function create the Tree nodes */
TreeNode* createNode(int data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}
//--------------------------------------------------------------
/* - The below function will check if the given character is
     an Operator or not */
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}
//--------------------------------------------------------------
/*- This function will Sets a priority for the operators
  - if the operator was a + or - it will returns 1 This indicates
  that addition and subtraction have a lower precedence level
  compared to other operators
  - if it was *, /, % then it will return 2 which will have a
  higher precedence level*/
int getPrecedence(char operator) {
    if (operator == '+' || operator == '-')
        return 1;
    else if (operator == '*' || operator == '/' || operator == '%')
        return 2;
    else
        return 0;
}
//--------------------------------------------------------------
/* this function will take the given infix expression and return
    it as a expression tree*/
TreeNode* buildExpressionTree(char* expression) {

    Stack operatorStack;
    Stack operandStack;

    initialize(&operatorStack);
    initialize(&operandStack);

    // will go through each single character in the expression
    for (int i = 0; expression[i] != '\0'; i++) {
        if (isdigit(expression[i])) {
            int operandValue = 0;
            while (isdigit(expression[i])) {
                /*multiplies it by 10 (will move the digits to the left),
                and then adds the numeric value of the current digit. This
                will create a multi-digit integer from the individual digits*/
                operandValue = operandValue * 10 + (expression[i] - '0');
                i++;
            }
            i--;  // Move back one position after reading the operand

            push(&operandStack, createNode(operandValue));
            /*If the character is an operator, it processes the
            operators on the operator stack in the order of
            priority and associativity, producing new nodes and
            moving them onto the operand stack.*/
        } else if (isOperator(expression[i])) {
            while (!isEmpty(&operatorStack) && isOperator(operatorStack.items[operatorStack.top]->data) &&
                   getPrecedence(expression[i]) <= getPrecedence(operatorStack.items[operatorStack.top]->data)) {
                TreeNode* operator = pop(&operatorStack);
                TreeNode* rightOperand = pop(&operandStack);
                TreeNode* leftOperand = pop(&operandStack);

                operator->left = leftOperand;
                operator->right = rightOperand;

                push(&operandStack, operator);
            }

            push(&operatorStack, createNode(expression[i]));
            /*The character is pushed into the operator stack
            if it is an open parenthesis '('.If the character
            is a closing parenthesis ')',the operator stack
            is processed until an error occurs. */
        } else if (expression[i] == '(') {
            push(&operatorStack, createNode(expression[i]));
        } else if (expression[i] == ')') {
            while (!isEmpty(&operatorStack) && operatorStack.items[operatorStack.top]->data != '(') {
                TreeNode* operator = pop(&operatorStack);
                TreeNode* rightOperand = pop(&operandStack);
                TreeNode* leftOperand = pop(&operandStack);

                operator->left = leftOperand;
                operator->right = rightOperand;

                push(&operandStack, operator);
            }

            if (!isEmpty(&operatorStack) && operatorStack.items[operatorStack.top]->data == '(') {
                pop(&operatorStack); // Pop the '('
            } else {
                printf("Error: Mismatched parentheses\n");
            }
        }
    }

    while (!isEmpty(&operatorStack)) {
        TreeNode* operator = pop(&operatorStack);
        TreeNode* rightOperand = pop(&operandStack);
        TreeNode* leftOperand = pop(&operandStack);

        operator->left = leftOperand;
        operator->right = rightOperand;

        push(&operandStack, operator);
    }
    /*The root of the expression tree is the top
    element of the operand stack*/
    return operandStack.items[operandStack.top];
}
//--------------------------------------------------------------
/*- this function takes the root of the tree and return the result*/
int evaluateExpressionTree(TreeNode* root) {
    //if root is null means no expression
    if (root == NULL) {
        return 0;
    }
    // if the left and right is null then there is not operands
    // then it will returned the stored DATA in the root itself
    if (root->left == NULL && root->right == NULL) {
        return root->data;
    }

    int leftValue = evaluateExpressionTree(root->left);
    int rightValue = evaluateExpressionTree(root->right);
    /* evaluates the left and right subtrees*/
    switch (root->data) {
        case '+':
            return leftValue + rightValue;
        case '-':
            return leftValue - rightValue;
        case '*':
            return leftValue * rightValue;
        case '/':
            if (rightValue != 0) {
                return leftValue / rightValue;
            } else {
                printf("Error: Division by zero\n");
            }
        case '%':
            if (rightValue != 0) {
                return leftValue % rightValue;
            } else {
                printf("Error: Modulo by zero\n");
            }
        default:
        //does not match any of the recognized operators,
        //it returns 0
            return 0;
    }
}
//--------------------------------------------------------------
/*- This function is responsible for printing the
    equations stored in the equations*/
void printEquations(char* equations) {
    if (equations[0] == '\0') {
        printf("No equations available. Please read equations first. AKA - Choose 1 -\n");
        return;
    }

    printf("Equations:\n%s\n", equations);
}
//--------------------------------------------------------------
/*-  function reads equations from a file and stores
     them in a array */
void readEquations(char* filename, char* equations) {
    FILE* file = fopen(filename, "r");
    // if null then there is an error
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
    }
    // initialized it with null
    equations[0] = '\0';
    // then get and read the expressions from the file
    char buffer[MAX_SIZE];
    while (fgets(buffer, MAX_SIZE, file) != NULL) {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        strcat(equations, buffer);
        strcat(equations, "\n");
    }

    fclose(file);
}
//--------------------------------------------------------------
/* -  Function saves an array of results to a file
   - if file is null the print an error
   - else it will writes the results at the file */
void saveResultsToFile(char *filename, int *results, int count) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error opening file %s for writing\n", filename);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Result %d: %d\n", i + 1, results[i]);
    }

    fclose(file);
}
//--------------------------------------------------------------
int main() {
    Stack stack;
    initialize(&stack);

    int choice;
    char equations[MAX_SIZE];
    int equationCount = 0;
    int results[MAX_SIZE];
    printf("                    ******************************************\n");
    printf("                    ---------- WELCOME TO PROJECT 2 ----------\n");
    printf("                    ******************************************\n");
    do {
        printf("\nExpression Tree Menu:\n");
        printf("1. Read equations\n");
        printf("2. Print equations\n");
        printf("3. Evaluate using Expression tree\n");
        printf("4. Print infix expressions -NOT-\n");
        printf("5. Save to output file \n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the file name: ");
                scanf("%s", equations);
                readEquations(equations, equations);
                printf("File was read successfully!!! \n");
                break;

            case 2:
                printEquations(equations);
                break;

            case 3:
                if (equations[0] == '\0') {
                    printf("No equations available. Please read equations first.\n");
                } else {
                    printf("Results:\n");
                    char* equation = strtok(equations, "\n");
                    while (equation != NULL) {
                        TreeNode* root = buildExpressionTree(equation);
                        results[equationCount] = evaluateExpressionTree(root);
                        printf("Current Equation: %s\n", equation);
                        printf("Result %d: %d\n", equationCount + 1, results[equationCount]);
                        equation = strtok(NULL, "\n");
                        equationCount++;
                    }
                }
                break;
            case 4:
                break;
            case 5:
                 if (equationCount == 0) {
                    printf("No equations evaluated. Please evaluate equations first.\n");
                } else {
                    char outputFileName[MAX_SIZE];
                    printf("Enter the output file name: ");
                    scanf("%s", outputFileName);
                    saveResultsToFile(outputFileName, results, equationCount);
                    printf("Results saved to %s\n", outputFileName);
                }
                break;

            case 6:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 6);

    return 0;
}
