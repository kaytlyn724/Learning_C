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
#ifndef PARSERH
#define PARSERH
#include <string>
#include <vector>
#include <stack>
using namespace std;
#include "inputbuf.h"
#include "lexer.h"

//using this to know exactly which one im in to later check
//I wasnt sure how to do this initially but I was reminded that enums existed

typedef enum {
    GLOBALVT = 0,
    PUBLICVT,
    PRIVATEVT
} varListType;

//use to track the scope
struct TrackScope {
    //used to track the name
    string name;
    //, public and private in vectors
    vector<string> publicVariables;
    vector<string> privateVariables;
    //and where they are nested
    TrackScope* nested;
};

class Parser {
public:
    //parse grammar
    void parse_program();
    void parse_global_vars();
    void parse_var_list(TrackScope*, varListType); //from enum
    void parse_scope();
    void parse_public_vars(TrackScope*);
    void parse_private_vars(TrackScope*);
    void parse_stmt_list(TrackScope*);
    void parse_stmt(TrackScope*);
    void syntax_error();
    Token parse_token(TokenType t);
    //output grammar
    void outputAll();
    //confirm the scope we're in
    string CheckScope(string, TrackScope*);
    //push scope to the track scop
    void pushScope(TrackScope*);
    //pop the current scope
    void popScope();
    //grab current scope
    TrackScope* getScope();
private:
    LexicalAnalyzer lexer;
    //two sides of the output
    vector<string> lhs;
    vector<string> rhs;
    //use stack and list separately
    stack<TrackScope*> stackScope;
    vector<TrackScope*> listScope;
};

#endif //PARSERH
