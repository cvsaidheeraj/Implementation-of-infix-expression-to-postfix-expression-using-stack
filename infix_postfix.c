// This C program converts an infix expression to its equivalent postfix expression
// It checks whether the input expression is a valid expression, whether it has valid operands, valid operators, and balanced parentheses
// It takes input from a file specified as a command-line argument and outputs the result to a file named "output.txt"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100 // maximum length of input expression

char stk[MAX]; // character array representing stack
int top = -1;

// function to check whether the stack is empty
int isEmpty()
{
  return top == -1;
}

// function to check whether the stack is full
int isFull()
{
  return top == MAX - 1;
}

// function to return the top element of the stack
char peek()
{
  return stk[top];
}

// function to pop an element from the stack
char pop()
{
  if (isEmpty())
    return -1;

  char ch = stk[top];
  top--;
  return (ch);
}

// function to push an element from the stack
void push(char oper)
{
  if (isFull())
    printf("Stack Full!!!!");
  else
  {
    top++;
    stk[top] = oper;
  }
}

// function to check whether a given character is an operand
int checkIfOperand(char ch)
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// function to return the precedence of an operator
int precedence(char ch)
{
  switch (ch)
  {
  case '+':
  case '-':
    return 1;

  case '*':
  case '/':
    return 2;

  case '^':
    return 3;
  }
  return -1;
}

// function to check whether the parentheses in the expression are balanced
int hasvalidParenthesis(char *expression)
{
  int i, count = 0;
  for (i = 0; expression[i]; ++i)
  {
    if (expression[i] == '(')
      count++;
    else if (expression[i] == ')')
      count--;
    if (count < 0)
      return 0;
  }
  return count == 0;
}

// function to check whether the operators in the expression are valid
int hasValidOperators(char *expression)
{
  int i;
  for (i = 0; expression[i]; ++i)
  {
    if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '^')
    {
      if (i == 0 || expression[i + 1] == '\n' || expression[i + 1] == '+' || expression[i + 1] == '-' || expression[i + 1] == '*' || expression[i + 1] == '/' || expression[i + 1] == '^')
        return 0;
    }
  }
  return 1;
}

// function to check whether the operands in the expression are valid
int hasValidOperands(char *expression)
{
  int i;
  for (i = 0; expression[i]; ++i)
  {
    if (expression[i] >= 'a' && expression[i] <= 'z')
    {
      if (expression[i + 1] >= 'a' && expression[i + 1] <= 'z')
        return 0;
    }
  }
  return 1;
}

// function to check whether the input expression is valid
int isValidExpression(char *expression)
{

  return hasValidOperators(expression) && hasValidOperands(expression) && hasvalidParenthesis(expression);
}

// This function converts an infix expression to postfix expression.
// It returns -1 if the expression is invalid, otherwise it returns 0.
int covertInfixToPostfix(char *expression, char *result, FILE *f_ptr)
{

  // Check if the expression is valid, if not return -1.
  if (!isValidExpression(expression))
    return -1;

  int i, j;
  char temp;

  // Traverse the expression from left to right
  for (i = 0, j = -1; expression[i]; ++i)
  {

    // If the character is an operand, add it to the result array or if it is an operator or a left parentheses, add it to the stack
    if (isalnum(expression[i]) || precedence(expression[i]) > 0 || expression[i] == '(' || expression[i] == ')')
    {
      if (checkIfOperand(expression[i]))
        result[++j] = expression[i];
      else if (expression[i] == '(')
      {
        push(expression[i]);
        fprintf(f_ptr, "%c is pushed into stack\n", expression[i]);

        // If the character is a right parentheses, pop all the operators from the stack until we reach the matching left parentheses.
      }
      else if (expression[i] == ')')
      {
        while (!isEmpty() && peek() != '(')
        {
          result[++j] = pop();
          fprintf(f_ptr, "%c is popped from stack\n", result[j]);
        }

        // Pop the left parentheses from the stack
        temp = pop();
        fprintf(f_ptr, "%c is popped from stack\n", temp);
      }

      // If the character is an operator, pop all the operators from the stack with higher or equal precedence and add them to the result array. Then push the current operator to the stack.
      else
      {
        while (!isEmpty() && precedence(expression[i]) <= precedence(peek()))
        {
          result[++j] = pop();
          fprintf(f_ptr, "%c is popped from stack\n", result[j]);
        }
        push(expression[i]);
        fprintf(f_ptr, "%c is pushed into stack\n", expression[i]);
      }
    }
  }

  // Pop all the remaining operators from the stack and add them to the result array.
  while (!isEmpty())
  {
    result[++j] = pop();
    fprintf(f_ptr, "%c is popped from stack\n", result[j]);
  }

  // Add the null character at the end of the result array.
  result[++j] = '\0';
  return 0;
}

// The main function takes one command line argument - the input file.
int main(int argc, char *argv[])
{
  // Check if the input file is provided.
  if (argc != 2)
  {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  // Open the input file for reading.
  FILE *input_file = fopen(argv[1], "r");
  if (!input_file)
  {
    printf("Failed to open input file %s\n", argv[1]);
    return 1;
  }

  // Open the output file for writing.
  FILE *output_file = fopen("output.txt", "w");
  if (!output_file)
  {
    printf("Failed to open output file\n");
    fclose(input_file);
    return 1;
  }

  // Define a char array called expression to store input expressions
  char expression[MAX];
  // Read expressions from input_file line by line and store them in expression array
  while (fgets(expression, MAX, input_file))
  {
    // Define a char array called result to store the postfix expression
    char result[MAX];
    // Check if the input expression is valid, and if not, handle the error
    if (covertInfixToPostfix(expression, result, output_file) == -1)
    {
      // Check if the expression has valid operands and operators, and if not, print "Invalid expression"
      if (!hasValidOperands(expression) || !hasValidOperators(expression))
      {
        fprintf(output_file, "%s", "Invalid expression");
      }
      else
      {
        // If the expression has unequal parentheses, print "Unequal Parentheses"
        fprintf(output_file, "%s", "Unequal Parentheses");
      }
      continue;
    }

    // If the input expression is valid, print the postfix expression to the output file
    fprintf(output_file, "Postfix expression is:%s\n", result);
    // Break out of the while loop after processing the first expression
    break;
  }
  
//   for(int i=0;expression[i];i++){
//       printf("%c",expression[i]);
//   }

  // Close the input and output files
  fclose(input_file);
  fclose(output_file);

  // Return 0 to indicate successful execution
  return 0;
}
