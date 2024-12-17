//Kaytlyn Daffern
//CSE 340 SUMMER
//PROJECT 3
//7-3-2024
//1221263225

/*
 * Created by Joshua Elkins
 * Date: June 24th, 2023
 */

#ifndef __PARSER__H__
#define __PARSER__H__

#include <vector>
#include <string>

#include "inputbuf.h"
#include "lexer.h"

// using struct to keep track of variable info
struct varInfo {
    std::string type;
    std::string name;
    bool checked;

    // Constructor to initialize the members
    varInfo(std::string type, std::string name, bool checked)
        : type(type), name(name), checked(checked) {}
    // Default constructor (if needed)
    varInfo() : type("?"), name(""), checked(false) {}
};

class Parser {
	public:
	    bool check;
		LexicalAnalyzer lexer;
		Token token;
		TokenType tempTokenType;
		int parse_program();
		std::vector<varInfo> holdVarsInProg;
		void printTMismatch(Token, std::string);
		//T type is the general type i have to figure out
		std::string getTType();
        int TType;
        void updateTType();
        // overloading for specific cases
        varInfo findVar(std::string lexeme);
        varInfo findVar(std::string lexeme, int code);
        varInfo findType(std::string type);
	private:
	    // changed some of these to be better suited for what i needed
        std::vector<varInfo> parse_varlist();
        // int parse_varlist();
		Token parse_unaryOperator();
		// int parse_unaryOperator();
		Token parse_binaryOperator();
		// int parse_binaryOperator();
		std::string parse_primary();
		// int parse_primary();
		std::string parse_expression();
		// int parse_expression();
		int parse_assstmt();
		int parse_case();
		int parse_caselist();
		int parse_switchstmt();
		int parse_whilestmt();
		int parse_ifstmt();
		int parse_stmt();
		int parse_stmtlist();
		int parse_body();
		int parse_typename(std::vector<varInfo> var);
		int parse_vardecl();
		int parse_vardecllist();
		int parse_globalVars();
};

#endif  //__PARSER__H__
