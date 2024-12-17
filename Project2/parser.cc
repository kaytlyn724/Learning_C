//Kaytlyn Daffern
//CSE 340 SUMMER
//PROJECT 2
//6-19-2024
//1221263225

/*
* Copyright (C) Rida Bazzi, 2016
*
* Do not share this file with anyone
*/
#include <iostream>
#include <istream>
#include <cstdio>
#include <string>
#include <vector>
#include "inputbuf.h"
#include "lexer.h"
#include "parser.h"

using namespace std;
//syntax error when incorrect
void Parser::syntax_error() {
    std::cout << "Syntax Error\n";
    exit(1);
}
//expect to grab a token and parse it when needed
Token Parser::parse_token(TokenType type) {
    Token t;
    t = lexer.GetToken();
    //use syntax error incase wrong
    if (type != t.token_type) {
        syntax_error();
    }
    return t;
}
//push pop and get for tracking the scope
void Parser::pushScope(TrackScope* scope) {
    stackScope.push(scope);
}
void Parser::popScope() {
    stackScope.pop();
}
TrackScope* Parser::getScope() {
    return stackScope.top();
}
//printing for testing
void Parser::outputAll() {
    //for every lefthand side completed output it and the righthand side connected
    for (int i=0; i<lhs.size(); i++) {
        cout << lhs[i] + " = " + rhs[i] << endl;
    }
}

//check the scope to see if the variables were made here or somewhere else
string Parser::CheckScope(string varName, TrackScope* scope) {
    //checking if the string is made somewhere else in a separate scope
    TrackScope* newScope = scope;
    TrackScope* privateCheck = scope;
    // public check
    while (newScope->nested != nullptr) {
        for (auto var: newScope->publicVariables) {
            if (varName == var) {
                return newScope->name + "." + varName;
            }
        }
        //move up the scope
        newScope = newScope->nested;
    }

    for (auto var: privateCheck->privateVariables) {
        if (varName == var) {
            return privateCheck->name + "." + varName;
        }
    }

    // Check in the global scope for public variables
    for (auto var: newScope->publicVariables) {
        if (varName == var) {
            return newScope->name + varName;
        }
    }
    // Reset to the original scope
    //newScope = scope;
    // Check in the original scope for private variables

    // Return default value if variable is not found in any scope
    return ("?." + varName);
}

//program -> global_vars scope
void Parser::parse_program() {
    parse_global_vars();
    parse_scope();
    //for printing the correct output
    outputAll();
}
//global_vars -> ε
//global_vars -> var_list SEMICOLON
void Parser::parse_global_vars() {
    //parse the global vars and add them to the scope vector
    TrackScope* globalVar = new TrackScope();
    //output for the global variables is this
    globalVar->name = "::";
    globalVar->nested = nullptr;
    //grab 2 tokens and check if this is epsilon or var_list SEMICOLON
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    // global_vars -> ε
    if((t1.token_type == ID)&&(t2.token_type == LBRACE)) {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        //in scope
        //push to the track scope
        pushScope(globalVar);
    }
    //global_vars -> var_list SEMICOLON
    else if((t1.token_type == ID)&&((t2.token_type == COMMA) || (t2.token_type == SEMICOLON))){
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        //in global so we parse var list
        //make sure we have the global var on the top of the scope
        parse_var_list(globalVar, GLOBALVT); //global type
        pushScope(globalVar);
        parse_token(SEMICOLON);
    }
    else {
        syntax_error();
    }
}
//var_list -> ID
//var_list -> ID COMMA var_list
void Parser::parse_var_list(TrackScope* scope, varListType vt) {
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();

    if((vt == GLOBALVT) || (vt == PUBLICVT)) {
        //add this token to the public variables
        scope->publicVariables.push_back(t1.lexeme);
    }else if(vt == PRIVATEVT) {
        scope->privateVariables.push_back(t1.lexeme);
    }else {
        syntax_error();
    }

    // var_list -> ID
    if((t1.token_type == ID)&&(t2.token_type == SEMICOLON)) {
        lexer.UngetToken(t2);
    }
    // var_list -> ID COMMA var_list
    else if((t1.token_type == ID)&&(t2.token_type == COMMA)) {
        parse_var_list(scope, vt);
    }
    else {
        syntax_error();
    }
}
//scope -> ID LBRACE public_vars private_vars stmt_list RBRACE
void Parser::parse_scope() {
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    //make a new scope and add to the scope
    TrackScope* scope = new TrackScope();
    scope->name = t1.lexeme;

    //new scope thats nested in the first scope
    TrackScope* nestIn = new TrackScope();
    nestIn = getScope();
    scope->nested = nestIn;

    // scope -> ID LBRACE public_vars private_vars stmt_list RBRACE
    if((t1.token_type == ID)&&(t2.token_type == LBRACE)) {
        parse_public_vars(scope);
        parse_private_vars(scope);
        pushScope(scope);
        //add to the list form the public and private being parse
        listScope.push_back(scope);
        parse_stmt_list(scope);
        parse_token(RBRACE);
        //out of scope now pop
        popScope();
    }
    else {
        syntax_error();
    }
}
//public_vars -> ε
//public_vars -> PUBLIC COLON var_list SEMICOLON
void Parser::parse_public_vars(TrackScope* scope) {
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    // public_vars -> PUBLIC COLON var_list SEMICOLON
    if((t1.token_type == PUBLIC)&&(t2.token_type == COLON)) {
        parse_var_list(scope, PUBLICVT);
        parse_token(SEMICOLON);
    }
    // public_vars -> ε -> private_vars -> PRIVATE COLON var_list SEMICOLON
    else if((t1.token_type == PRIVATE)&&(t2.token_type == COLON)) {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
    }
    // public_vars -> ε -> private_vars -> ε -> stmt_list -> stmt or -> scope
    else if((t1.token_type == ID)&&((t2.token_type == EQUAL)||(t2.token_type == LBRACE))) {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
    }
    else {
        syntax_error();
    }
}
//private_vars -> ε
//private_vars -> PRIVATE COLON var_list SEMICOLON
void Parser::parse_private_vars(TrackScope* scope) {
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    // public_vars -> ε -> private_vars -> PRIVATE COLON var_list SEMICOLON
    if((t1.token_type == PRIVATE)&&(t2.token_type == COLON)) {
        parse_var_list(scope, PRIVATEVT);
        parse_token(SEMICOLON);
    }
    // public_vars -> ε -> private_vars -> ε -> stmt_list -> stmt or -> scope
    else if((t1.token_type == ID)&&((t2.token_type == EQUAL)||(t2.token_type == LBRACE))) {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
    }
    else {
        syntax_error();
    }
}
//stmt_list -> stmt
//stmt_list -> stmt stmt_list
void Parser::parse_stmt_list(TrackScope* scope) {
    //doing this way so I dont have to write so many lines of code
    //basically going to parse stmt_list until I hit the right brace
    //and if i get it in here then ill just undo and move on
    Token t1 = lexer.GetToken();
    if(t1.token_type == RBRACE) {
        lexer.UngetToken(t1);
    }
    //so here because i dont want to write this a bunch ill just parse both
    //even if it doesnt have the stmt list and then itll come back and ill undo
    else {
        lexer.UngetToken(t1);
        parse_stmt(scope);
        parse_stmt_list(scope);
    }
}
//stmt -> ID EQUAL ID SEMICOLON
//stmt -> scope
void Parser::parse_stmt(TrackScope* scope) {
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    //want the first token on the left side of the string
    string left = t1.lexeme;
    // stmt -> ID EQUAL ID SEMICOLON
    if((t1.token_type == ID)&&(t2.token_type == EQUAL)) {
        Token t3 = parse_token(ID);
        string right = t3.lexeme;
        parse_token(SEMICOLON);
        //push both of the left and right sides to the lhs and rhs respectively
        //using check scope to verify where they were actually instantiated and where we are calling from
        lhs.push_back(CheckScope(left, scope));
        rhs.push_back(CheckScope(right, scope));
    }
    // stmt -> scope
    else if((t1.token_type == ID)&&(t2.token_type == LBRACE)) {
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        parse_scope();
    }
    else {
        syntax_error();
    }
}
int main()
{
    //use parser now instead of lexer
    Parser parser;
    parser.parse_program();
}
