/**
 * Name: Suraj Jena
 * Last Modified: April 1st, 2019 @ 12:55 PM
 * Purpose: Implement a recursive descent parser for a given
 *          for a given context-free grammar.
 * Rules (Right Recursive, in Chomsky Normal Form):
 *        command → expr ‘\n’
 *        expr → term expr-end
 *        expr_end → + term expr_end
 *                  | - term expr_end
 *                  | empty
 *        term → power term_end
 *        term_end → ∗ power term_end
 *                  | / power term_end
 *                  | % power term_end
 *                  | empty
 *        power → factor power_end
 *        power_end → ^ power
 *                   | empty
 *        factor → [-] factor1
 *        factor1 → ( expr )
 *                 | NUMBER
 *        NUMBER = [0 - 9]+
**/

#ifndef PARSER_H
  #define PARSER_H

#define MAX_LENGTH 33

typedef enum
{
  PLUS = '+', MINUS = '-', DIVIDE = '/', MULT = '*',
  REMAINDER = '%', POWER = '^', LPAREN = '(', RPAREN = ')',
  NUMBER = '9', ERROR = '\0', EOL = '\n'
} TokenType;

struct Token
{
  TokenType type;
  int value;
};

char* input = NULL;
struct Token token;

int expr();
int expr_infix_operator(int operand1);
int expr_end(int);
int term();
int term_infix_operator(int operand1);
int term_end(int);
int power();
int power_infix_operator(int operand1);
int power_end(int);
int factor();
int factor1();
void error();
void show_token(char* tokenType);
void lexer();
void match(TokenType tkType);
struct Token getToken();
void parse();

#endif
