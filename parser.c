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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include "parser.h"

/**
 * Production rules for the top level rule, 'command'.
 * @params: N/A (void)
 * @return: result (int) The resulting token value.
**/
void command()
{
  // printf("command(): Begin.\n");
  int result = 0;
  // printf("command(): Check for expr().\n");
  result = expr();
  // printf("command(): Check for EOL.\n");
  if (match(EOL))
  {
    printf("\n\tResult: %d\n\n", result);
    // printf("command(): End. Result: %d\n", result);
  }
  else
  {
    // printf("command(): End.\n");
    error("\'\\n\' expected", "command");
  }
}

/**
 * Production rules for the second level rule, 'expr'.
 * @params: N/A (void)
 * @return: result (int) The resulting token value.
**/
int expr()
{
  // printf("expr(): Begin.\n");
  int result = 0;
  // printf("expr(): Check for term().\n");
  result = term();
  // printf("expr(): Check for expr_end().\n");
  result = expr_end(result);
  // printf("expr(): End. Result: %d\n", result);

  return result;
}

/**
 * Alternate production rules for the second level rule, 'expr'.
 * @params: operand (int) The first operand to carry forward.
 * @return: result (int) The resulting token value.
**/
int expr_end(int operand)
{
  // printf("expr_end(): Begin.\n");
  int result = 0;
  // printf("expr_end(): Check for expr_infix_operator().\n");
  result += expr_infix_operator(operand);
  // printf("expr_end(): End. Result: %d\n", result);

  return result;
}

/**
 * Production rules for the third level rule, 'term'.
 * @params: N/A (void)
 * @return: value (int) The resulting token value.
**/
int term()
{
  // printf("term(): Begin.\n");
  int result = 0;
  // printf("term(): Check for power().\n");
  result = power();
  // printf("term(): Check for term_end().\n");
  result = term_end(result);
  // printf("term(): End. Result: %d\n", result);

  return result;
}

/**
 * Alternate production rules for the third level rule, 'term'.
 * @params: operand (int) The first operand to carry forward.
 * @return: result (int) The resulting token value.
**/
int term_end(int operand)
{
  // printf("term_end(): Begin.\n");
  int result = 0;
  // printf("term_end(): Check for term_infix_operator.\n");
  result += term_infix_operator(operand);
  // printf("term_end(): End. Result: %d\n", result);

  return result;
}

/**
 * Production rules for the fourth level rule, 'power'.
 * @params: N/A (void)
 * @return: value (int) The resulting token value.
**/
int power()
{
  // printf("power(): Begin.\n");
  int result = 0;
  // printf("power(): Check for factor().\n");
  result = factor();
  // printf("power(): Check for power_end().\n");
  result = power_end(result);
  // printf("power(): End. Result: %d\n", result);

  return result;
}

/**
 * Production rules for the fourth level rule, 'power'.
 * @params: operand (int) The first operand to carry forward.
 * @return: result (int) The resulting token value.
**/
int power_end(int operand1)
{
  // printf("power_end(): Begin.\n");
  int result = 0;
  // printf("power_end(): Check for power_infix_operator().\n");
  result = power_infix_operator(operand1);
  // printf("power_end(): End. Result: %d\n", result);

  return result;
}

/**
 * Production rules for the fifth level rule, 'factor'.
 * @params: N/A (void)
 * @return: value (int) The resulting token value.
**/
int factor()
{
  int result = 0;
  // printf("factor(): Begin.\n");

  if (match(MINUS))
  {
    // printf("factor(): Found \'-\'.\n");
    getToken();
    // printf("factor(): Check for factor1().\n");
    result = (-1 * factor1());
    // printf("factor(): End. Result: %d\n", result);

    return result;
  }
  else
  {
    // printf("factor(): Check for factor1().\n");
    result = factor1();
    // printf("factor(): End. Result: %d\n", result);

    return result;
  }
}

/**
 * Production rules for the sixth level rule, 'factor1'.
 * @params: N/A (void)
 * @return: value (int) The resulting token value.
**/
int factor1()
{
  // printf("factor1(): Begin.\n");
  int result = 0;

  if (match(LPAREN))
  {
    // printf("factor1(): Found \'(\'.\n");
    getToken();
    // printf("factor1(): Check for expr().\n");
    result = expr();
    if (match(RPAREN))
    {
      // printf("factor1(): Found \')\'.\n");
      getToken();
      // printf("factor1(): End. Result: %d\n", result);
      return result;
    }
    else
    {
      error("No \')\' found", "factor1");
    }
  }
  else if (match(NUMBER))
  {
    // printf("factor1(): Found \'[0-9]+\'.\n");
    result = token.value;
    getToken();
    // printf("factor1(): End. Result: %d\n", result);
    return result;
  }

  return result;
}

/**
 * Error out when called from any rule functions.
 * @params: tkType (TokenType) The type of the token from the enum.
 * @return: N/A (void)
**/
void error(char* error, char* source)
{
  printf("Error: %s at rule: %s.\n\n----- Program terminated. -----", error, source);
  exit(0);
}

/**
 * Display current token when called from the lexical analyzer.
 * @params: tokenType (char*) The token type string.
 * @return: N/A (void)
**/
void show_token(char* tokenType)
{
  if (strcmp(tokenType, "EOL") == 0)
    return;
  else if (strcmp(tokenType, "NUMBER") == 0)
    printf("\t %d \t %s \n", token.value, tokenType);
  else
    printf("\t %c \t %s \n", token.value, tokenType);
}

/**
 * Resolves token by checking with the expr operators, expects infix.
 * @params: operand1 (int) ASCII value of already matched operand1.
 * @return: result (int) The resulting token value.
**/
int expr_infix_operator(int operand1)
{
  // printf("expr_infix_operator(): Begin.\n");
  int result = 0;

  if (match(PLUS))
  {
    // printf("expr_infix_operator(): Found a \'+\'.\n");
    getToken();
    // printf("expr_infix_operator(): Check for term().\n");
    result = operand1 + term();
    // printf("expr_infix_operator(): Check for expr_end().\n");
    result = expr_end(result);
    // printf("expr_infix_operator(): End. Result: %d\n", result);

    return result;
  }
  else if (match(MINUS))
  {
    // printf("expr_infix_operator(): Found a \'-\'.\n");
    getToken();
    // printf("expr_infix_operator(): Check for term().\n");
    result = operand1 - term();
    // printf("expr_infix_operator(): Check for expr_end().\n");
    result = expr_end(result);
    // printf("expr_infix_operator(): End. Result: %d\n", result);

    return result;
  }
  // printf("expr_infix_operator(): End. Result: %d\n", operand1);

  return operand1;
}

/**
 * Resolves token by checking with the term operators, expects infix.
 * @params: operand1 (int) ASCII value of already matched operand1.
 * @return: result (int) The resulting token value.
**/
int term_infix_operator(int operand1)
{
  // printf("term_infix_operator(): Begin.\n");
  int result = 0;

  if (match(MULT))
  {
    // printf("term_infix_operator(): Found a \'*\'.\n");
    getToken();
    // printf("term_infix_operator(): Check for power().\n");
    result = operand1 * power();
    // printf("term_infix_operator(): Check for term_end().\n");
    result = term_end(result);
    // printf("term_infix_operator(): End. Result: %d\n", result);

    return result;
  }
  else if (match(DIVIDE))
  {
    // printf("term_infix_operator(): Found a \'\\\'.\n");
    getToken();
    // printf("term_infix_operator(): Check for power().\n");
    result = operand1 / power();
    // printf("term_infix_operator(): Check for term_end().\n");
    result = term_end(result);
    // printf("term_infix_operator(): End. Result: %d\n", result);

    return result;
  }
  else if (match(REMAINDER))
  {
    // printf("term_infix_operator(): Found a \'%%\'.\n");
    getToken();
    // printf("term_infix_operator(): Check for power().\n");
    result = operand1 % power();
    // printf("term_infix_operator(): Check for term_end().\n");
    result = term_end(result);
    // printf("term_infix_operator(): End. Result: %d\n", result);

    return result;
  }
  // printf("term_infix_operator(): End. Result: %d\n", operand1);

  return operand1;
}

/**
 * Resolves token by checking with the power operators, expects infix.
 * @params: operand1 (int) ASCII value of already matched operand1.
 * @return: result (int) The resulting token value.
**/
int power_infix_operator(int operand1)
{
  // printf("power_infix_operator(): Begin.\n");
  int result = 0;

  if (match(POWER))
  {
    // printf("power_infix_operator(): Found a \'^\'.\n");
    getToken();
    // printf("power_infix_operator(): Check for power().\n");
    result = (int) pow((double) operand1, (double) power());
    // printf("power_infix_operator(): End. Result: %d\n", result);
    return result;
  }

  // printf("power_infix_operator(): End. Result: %d\n", operand1);
  return operand1;
}

/**
 * Resolve type of the latest ingested token - perform lexical analysis.
 * @params: N/A (void)
 * @return: N/A (void)
**/
void lexer()
{
  // printf("lexer(): Begin.\n");

  if (token.type == NUMBER) { token.type = NUMBER; show_token("NUMBER"); }
  else if (token.value == MINUS) { token.type = MINUS; show_token("MINUS"); }
  else if (token.value == MULT) { token.type = MULT; show_token("MULT"); }
  else if (token.value == DIVIDE) { token.type = DIVIDE; show_token("DIVIDE"); }
  else if (token.value == REMAINDER) { token.type = REMAINDER; show_token("REMAINDER"); }
  else if (token.value == POWER) { token.type = POWER; show_token("POWER"); }
  else if (token.value == LPAREN) { token.type = LPAREN; show_token("LPAREN"); }
  else if (token.value == RPAREN) { token.type = RPAREN; show_token("RPAREN"); }
  else if (token.value == EOL) { token.type = EOL; show_token("EOL"); }
  else if (token.value == PLUS) { token.type = PLUS; show_token("PLUS"); }
  else
  {
    // printf("Token entered: %c\n", token.value);
    error("Unrecognized token", "lexer()");
  }

  // printf("lexer(): End.\n");
  return;
}

/**
 * Matches the current token with the suggested TokenType.
 * @params: tkType (TokenType) The type of the token from the enum.
 * @return: result (bool) The match result - false if failed.
**/
bool match(TokenType tkType)
{
  // printf("match(): Begin.\n");

  if (token.type == tkType)
  {
    // printf("match(): Found %c.\n", tkType);
    return true;
  }

  // printf("match(): End.\n");

  return false;
}

/**
 * Parses a new token, and returns the latest token parsed.
 * @params: N/A (void)
 * @return: token (Token) The token structure with the value & type.
**/
struct Token getToken()
{
  // printf("getToken(): Begin.\n");
  int currentCharacter = 0;
  // Reset global token
  token.value = 0;
  token.type = ERROR;

  // Loop to skip blanks
  while ((currentCharacter = getchar()) == ' ');

  if (isdigit(currentCharacter))
  {
    // Loop when reading number
    while (isdigit(currentCharacter))
    {
      //Convert character in respective digit
      token.value = 10 * token.value + currentCharacter - '0';
      // read in next character
      currentCharacter = getchar();
      // Set read-in token type to 'NUMBER'
      token.type = NUMBER;
    }

    // done reading all digits, out of while loop, must have found a
    // non-digit, so push last character read in back to standard input
    ungetc(currentCharacter, stdin);
    lexer();
  }
  else
  {
    token.value = currentCharacter;
    lexer();
  }

  // printf("getToken(): End.\n");
  return token;
}

/**
 * Parses the next upcoming token in the token stream.
 * @params: N/A (void)
 * @return: N/A (void)
**/
void parse()
{
  // printf("parse(): Begin.\n");
  printf("\nInput: ");
  getToken();
  command();
  // printf("parse(): End.\n");
}

/**
 * Scans the next upcoming token, performs lexical analysis.
 * Just performs a test of tokenization, to verify everything works.
 * Expects a two-operand single-operator infix expression, checks for PLUS to verify input.
 * @params: N/A (void)
 * @return: N/A (void)
**/
void scan_test()
{
  // printf("scan(): Begin.\n");
  // printf("Input: ");
  getToken();
  getToken();
  // printf("scan(): Begin.\n");
}

/**
 * Control center for the program.
 * @params: N/A (void)
 * @return: 0 (int) Provisional return parameter.
**/
int main()
{
  // printf("***** Top-Down Recursive Descent Parser *****\n");
  // scan_test();
  parse();
  // printf("***** END *****\n");
}
