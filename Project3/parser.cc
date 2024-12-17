//Kaytlyn Daffern
//CSE 340 SUMMER
//PROJECT 3
//7-3-2024
//1221263225

/*
 * Created by: Joshua Elkins
 * Date: June 24th, 2023
 */

#include <iostream>
#include <string>
#include "parser.h"
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

// Syntax Error Function.
void syntax_error(){
	cout << "Syntax Error\n";
	exit(1);
}

varInfo Parser::findVar(string lexeme) {
    // loop through holdVarsInProg to find the variable with the matching name
    for (int i = 0; i < holdVarsInProg.size(); i++) {
        if (holdVarsInProg[i].name == lexeme) {
            return holdVarsInProg[i]; // return the variable if found
        }
    }
    varInfo noType;
    // using noType set the type to getTType and the name to lexeme we came with
    noType.type = getTType();
    noType.name = lexeme;
    noType.checked = false; // set checked to false
    // we want to update after adding
    updateTType();
    // add the new var to be held
    holdVarsInProg.push_back(noType);
    return holdVarsInProg[holdVarsInProg.size() - 1];
}

varInfo Parser::findVar(string lexeme, int count) {
    if(count > 0) {
        // if count is greater than 0, loop through holdVarsInProg to find the variable with the matching name
        for (int i = 0; i < holdVarsInProg.size(); i++) {
            if (holdVarsInProg[i].name == lexeme) {
                // return we found the variable
                return holdVarsInProg[i];
            }
        }
        varInfo noType;
        // using noType set the type to getTType and the name to lexeme we came with
        noType.type = "bool";
        noType.name = lexeme;
        noType.checked = false; // set checked to false
        // add the new var to be helf
        holdVarsInProg.push_back(noType);
        return holdVarsInProg[holdVarsInProg.size() - 1];
    } else {
        syntax_error();
    }
}

varInfo Parser::findType(string type) {
    // loop through to find the variable with the matching type
    for (const auto& var : holdVarsInProg) {
        if (var.type == type) {
            return var; // return the variable if found
        }
    }
    varInfo mVar;
    // using mVar set the type to moot and the name to moot
    mVar.type = "moot";
    mVar.name = "moot";
    mVar.checked = true; // set checked to true because its checked right now
    return mVar;
}

// returns the type mismatch in the way expected in the output
void Parser::printTMismatch(Token token, string error) {
    // print the type mismatch error message with the line number and error string
    cout << "TYPE MISMATCH " << token.line_no << " " << error << endl;
    exit(1);
}

// increments the T Type
void Parser::updateTType() {
    TType++;
}
// uses the same T but with a new number next to it depending on which T type it is
string Parser::getTType() {
    return "T" + to_string(TType);
}

/*
 * Completed Function.
 * Entry point to the program.
 */
 int Parser::parse_program(){
     #ifdef DEBUG
         cout << "Entered Parse Program" << endl;
     #endif
     // initialize to generic start vals
     TType = 1;
     check = false;
     token = lexer.GetToken();
     if(token.token_type == ID){
         // if the token is an ID unget and parse globalvars then body
         lexer.UngetToken(token);
         parse_globalVars();
         parse_body();
     }
     else if(token.token_type == LBRACE){
         // if the token is a left brace, unget the token and parse the body
         lexer.UngetToken(token);
         parse_body();
     }
     else{
         syntax_error(); // call syntax_error if the token is not an ID or left brace
     }
     // loop through holdVarsInProg to print variables and their types
     for (auto& var : holdVarsInProg) {
         if (!var.checked && var.name != "moot") {
             if (var.type == "bool" || var.type == "real" || var.type == "int") {
                 // print the variable name and type if it is a basic type
                 cout << var.name << ": " << var.type << " #\n";
             } else {
                 string stringSameVars;
                 for (auto& innerVar : holdVarsInProg) {
                     if ((innerVar.type == var.type) && (innerVar.name != "moot")) {
                         // concatenate names of variables with the same type
                         stringSameVars += innerVar.name + ", ";
                         innerVar.checked = true;
                     }
                 }
                 // remove the last comma and space if its at the end
                 stringSameVars.pop_back();
                 stringSameVars.pop_back();
                  // print the names of variables with the same type followed by a question mark
                 cout << stringSameVars << ": ? #\n";
             }
         }
     }
     return 0;
 }


/*
 * Completed Function.
 * Acts as basic entry into the global variable list.
 */
int Parser::parse_globalVars(){
	#ifdef DEBUG
		cout << "Entered Parse Global Variables" << endl;
	#endif
	parse_vardecllist();

	return 0;
}

/*
 * Completed
 * Loops our variable declaration list.
 */
int Parser::parse_vardecllist(){
	#ifdef DEBUG
		cout << "Entered Parse Variable Declaration List" << endl;
	#endif
	token = lexer.GetToken();
	while(token.token_type == ID){
		lexer.UngetToken(token);
		parse_vardecl();
		token = lexer.GetToken();
	}
	lexer.UngetToken(token);

	return 0;
}

/*
 * Completed Function
 * Acts as a method to handle the declaration statements.
 */
int Parser::parse_vardecl(){
	#ifdef DEBUG
		cout << "Entered Parse Variable Declaration" << endl;
	#endif
	token = lexer.GetToken();
	// making a vector var here to be used to save things
	vector<varInfo> var;
	if(token.token_type != ID){
		syntax_error();
	}
	lexer.UngetToken(token);
	// set the var to parse var list
	var = parse_varlist();
	token = lexer.GetToken();
	if(token.token_type != COLON){
		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type == INT || token.token_type == REAL || token.token_type == BOO){
		lexer.UngetToken(token);
		// sending the var into type name here
		parse_typename(var);
		token = lexer.GetToken();
		if(token.token_type != SEMICOLON){
			syntax_error();
		}
	}
	else{
		syntax_error();
	}
	return 0;
}

/*
 * Completed Function
 * Acts as the gathering function for our variables
 */
 vector<varInfo> Parser::parse_varlist(){
     #ifdef DEBUG
         cout << "Entered Parse Variable List" << endl;
     #endif
     token = lexer.GetToken();
     // made a vector again to track
     vector<varInfo> var;
     if(token.token_type != ID){
         syntax_error();
     }
     else{
         Token t2 = lexer.GetToken();
         if(t2.token_type == COMMA){
             // loop to handle a list of IDs separated by commas
             while(token.token_type == ID && t2.token_type == COMMA){
                 varInfo trackVar;
                 // setting the track var name to the lexeme and adding it to the vector
                 trackVar.name = token.lexeme;
                 trackVar.checked = false;
                 var.push_back(trackVar);
                 token = lexer.GetToken();
                 // check the next token
                 if(token.token_type != ID){
                     syntax_error();
                 }
                 t2 = lexer.GetToken();
             }
             lexer.UngetToken(t2);
             // new track var and set the name to the lexeme and push to the vector
             varInfo trackVar2;
             trackVar2.name = token.lexeme;
             trackVar2.checked = false;
             var.push_back(trackVar2);
         }
         else{

             lexer.UngetToken(t2);
             // same track var on the else case to grab the lexeme and push
             varInfo trackVar2;
             trackVar2.name = token.lexeme;
             trackVar2.checked = false;
             var.push_back(trackVar2);
         }
     }
     return var;
 }

/*
 * Completed Function
 * Just consumes the INT, REAL, or BOO tokens
 */
 int Parser::parse_typename(vector<varInfo> var){
     #ifdef DEBUG
     cout << "Entered Parse Type Name" << endl;
     #endif

     token = lexer.GetToken();
     if(token.token_type == INT){
         // handle gathering info for variables of type int
         for (int i = 0; i < var.size(); i++) {
             // set the type of the variable to int
             var[i].type = "int";
             bool found = false;
             // check if the variable already exists in holdVarsInProg
             for(int j = 0; j < holdVarsInProg.size(); j++) {
                 if(holdVarsInProg[j].name == var[i].name) {
                      // mark as found if the variable exists
                     found = true;
                 }
             }
             if(!found) {
                 // add the variable to holdVarsInProg if not found
                 holdVarsInProg.push_back(var[i]);
             }
         }
     }
     else if(token.token_type == REAL){
         // handle gathering info for variables of type real
         for (int i = 0; i < var.size(); i++) {
             // set the type of the variable to real
             var[i].type = "real";
             bool found = false;
             // check if the variable already exists in holdVarsInProg
             for(int j = 0; j < holdVarsInProg.size(); j++) {
                 if(holdVarsInProg[j].name == var[i].name) {
                     // mark as found if the variable exists
                     found = true;
                 }
             }
             if(!found) {
                 // add the variable to holdVarsInProg if not found
                 holdVarsInProg.push_back(var[i]);
             }
         }
     }
     else if(token.token_type == BOO){
         // handle gathering info for variables of type bool
         for (int i = 0; i < var.size(); i++) {
             // set the type of the variable to bool
             var[i].type = "bool";
             bool found = false;
             // check if the variable already exists in holdVarsInProg
             for(int j = 0; j < holdVarsInProg.size(); j++) {
                 if(holdVarsInProg[j].name == var[i].name) {
                     found = true;
                 }
             }
             if(!found) {
                 // add the variable if not found
                 holdVarsInProg.push_back(var[i]);
             }
         }
     }
     else{
         syntax_error();
     }

     return 0;
 }

/*
 * Completed Function
 * Acts as the method to consume braces and enter statement list
 */
int Parser::parse_body(){
	#ifdef DEBUG
		cout << "Entered Parse Body" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == LBRACE){
		parse_stmtlist();
		token = lexer.GetToken();
		if(token.token_type != RBRACE){
			syntax_error();
		}
	}
	else{
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Acts as our looper to enter all our statements
 */
int Parser::parse_stmtlist(){
	#ifdef DEBUG
		cout << "Entered Parse Statement List" << endl;
	#endif
	token = lexer.GetToken();
	while(token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH){
		lexer.UngetToken(token);
		parse_stmt();
		token = lexer.GetToken();
	}
	lexer.UngetToken(token);

	return 0;
}

/*
 * Completed Function
 * Acts as our method to enter the specific statements
 */
int Parser::parse_stmt(){
	#ifdef DEBUG
		cout << "Entered Parse Statement" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == ID){
		lexer.UngetToken(token);
		parse_assstmt();
	}
	else if(token.token_type == IF){
		lexer.UngetToken(token);
		parse_ifstmt();
	}
	else if(token.token_type == WHILE){
		lexer.UngetToken(token);
		parse_whilestmt();
	}
	else if(token.token_type == SWITCH){
		lexer.UngetToken(token);
		parse_switchstmt();
	}
	else{
		syntax_error();
	}

	return 0;
}

/*
 * Function Completed
 * Acts as our assignment statement parser
 */
 int Parser::parse_assstmt(){
     #ifdef DEBUG
     cout << "Entered Parse Assignment Statement" << endl;
     #endif
     token = lexer.GetToken();
     if(token.token_type != ID){
         syntax_error();
     }
     // Do something with ID
     // find the variable information for the token lexeme
     varInfo var = findVar(token.lexeme);
     string next = token.lexeme;
     token = lexer.GetToken();

     if(token.token_type != EQUAL){
         syntax_error(); // call syntax_error if the token is not an equal sign
     }
    // parse the expression and store its return type
    string returnS = parse_expression();
    // find the variable information again for the token lexeme
     var = findVar(next);
     if((var.type == "int") || (var.type == "bool") || (var.type == "real")) {
         if(var.type != returnS) {
              // print type mismatch error if types do not match
             printTMismatch(token, "C1");
         }
     }
     else {
         // handle assigning new type to var of the same type as var.type
         for (auto& v : holdVarsInProg) {
             if (v.type == var.type) {
                 // assign new type to variables of the same type as var.type
                 v.type = returnS;
             }
         }
     }
     token = lexer.GetToken();
     if(token.token_type != SEMICOLON){
         syntax_error();
     }
     return 0;
 }


/*
 * Completed Function
 * Acts as our expression handling.
 */
 string Parser::parse_expression(){
     #ifdef DEBUG
     cout << "Entered Parse Expression" << endl;
     #endif
     token = lexer.GetToken();
     if(token.token_type == NOT){
         // unget the NOT token and mark checked
         lexer.UngetToken(token);
         check = true;
          // parse unary operator and get token
          //  // recursively parse expression
         Token token = parse_unaryOperator();
         string returnS = parse_expression();
         if((returnS == "int") || (returnS == "real")) {
             // print type mismatch error for C3
             printTMismatch(token, "C3");
         }
         else {
             // reset check to false and return bool
             check = false;
             return "bool";
         }
     }
     else if(token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT ||  token.token_type == DIV){
         lexer.UngetToken(token);
          // store current check value
         bool keepCheck = check;
         check = false;
          // parse binary operator and get token
          // recursively parse expression
          // recursively parse expression
         Token token = parse_binaryOperator();
         string returnS = parse_expression();
         string return2S = parse_expression();
         if (returnS == "int") {
              if (return2S == "int") {
                   // restore original check value
                  check = keepCheck;
                  return "int";
              }
              else if(return2S == "real" || return2S == "bool") {
                // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "int";
              }
         }
         else if(returnS == "real") {
              if (return2S == "int" || return2S == "bool") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "real") {
                   // restore original check value
                  check = keepCheck;
                  return "real";
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "real";
              }
         }
         else if(returnS == "bool") {
              if (return2S == "int" || return2S == "real") {
                  // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "bool") {
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
         }
         else {
              if (return2S == "int") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                           // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "int";
              }
              else if(return2S == "real") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "real";
              }
              else if(return2S == "bool") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                           // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return returnS;
              }
         }
     }
     else if(token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL){
         lexer.UngetToken(token);
          // store current check value
         bool keepCheck = check;
         check = false;
          // parse binary operator and get token
          // recursively parse expression
          // find type name based on returnS
          // recursively parse expression
          // find variable information based on track
         Token token = parse_binaryOperator();
         string returnS = parse_expression();
         string track = findType(returnS).name;
         string return2S = parse_expression();
         varInfo trackInfo = findType(track);
         if((returnS != trackInfo.type) && (trackInfo.name != "moot")) {
           for(int i = 0; i < holdVarsInProg.size(); i++) {
                 if(holdVarsInProg[i].type == returnS) {
                      // update type in holdVarsInProg
                     holdVarsInProg[i].type = trackInfo.type;
                 }
             }
              // update returnS to trackInfo.type
             returnS = trackInfo.type;
         }
         if (returnS == "int") {
              if (return2S == "int") {
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else if(return2S == "real") {
                  // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "bool") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
         }
         else if(returnS == "real") {
              if (return2S == "int") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "real") {
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else if(return2S == "bool") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
         }
         else if(returnS == "bool") {
              if (return2S == "int") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "real") {
                   // print type mismatch error for C2
                  printTMismatch(token, "C2");
              }
              else if(return2S == "bool") {
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
         }
         else {
              if (return2S == "int") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                           // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                   // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else if(return2S == "real") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                          // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else if(return2S == "bool") {
                  for (int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == returnS) {
                           // update type in holdVarsInProg
                          holdVarsInProg[i].type = return2S;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
              else {
                  for(int i = 0; i < holdVarsInProg.size(); i++) {
                      if(holdVarsInProg[i].type == return2S) {
                           // update type in holdVarsInProg
                          holdVarsInProg[i].type = returnS;
                      }
                  }
                  // restore original check value
                  check = keepCheck;
                  return "bool";
              }
         }
     }
     else if(token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA){
         lexer.UngetToken(token);
          // parse primary and get return string and return
         string retString1 = parse_primary();
         return retString1;
     }
     else{
         syntax_error();
     }
}

/*
 * Completed Function
 * Gets our NOT token
 */
Token Parser::parse_unaryOperator(){
	#ifdef DEBUG
		cout << "Entered Parse Unary Operator" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type != NOT){
		syntax_error();
	}
	//Do something with the NOT
	return token;
}

/*
 * Completed Function
 * Acts as our binary handler
 */
Token Parser::parse_binaryOperator(){
	#ifdef DEBUG
		cout << "Entered Binary Operator" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT ||  token.token_type == DIV){
		return token;
	}
	else if(token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL){
		return token;
	}
	else{
		syntax_error();
	}
}

/*
 * Completed Function
 * Acts as our primary handler
 */
string Parser::parse_primary(){
	#ifdef DEBUG
		cout << "Entered Parse Primary" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA){
	    if(token.token_type == ID) {
			// make a new var to track
            varInfo var;
            if(check) {
                // find the var with the lexeme
                var = findVar(token.lexeme, 1);
            } else {
                // find the var with the lexeme
                var = findVar(token.lexeme);
            }
            return var.type;
		} else if (token.token_type == NUM) {
            return "int";
		} else if (token.token_type == TR || token.token_type == FA) {
            return "bool";
		}else if (token.token_type == REALNUM) {
            return "real";
		} else {
            syntax_error();
		}
	}
	else{
		syntax_error();
	}
}
/*
 * Completed Function
 * Acts as our If Statement handler
 */
 int Parser::parse_ifstmt(){
     #ifdef DEBUG
     cout << "Entered Parse If Statement" << endl;
     #endif

     token = lexer.GetToken();
     if(token.token_type != IF){
         syntax_error();
     }
     token = lexer.GetToken();
     if(token.token_type != LPAREN){
         syntax_error();
     }
     // parse expression and get return string
     string returnS = parse_expression();
     if((returnS == "int") || (returnS == "real")) {
         // print type mismatch error for C4
         printTMismatch(token, "C4");
     }
     for (auto& v : holdVarsInProg) {
         if (v.type == returnS) {
              // update type in holdVarsInProg to bool
             v.type = "bool";
         }
     }
     token = lexer.GetToken();
     if(token.token_type != RPAREN){
         syntax_error();
     }
    // parse the body of the if statement
     parse_body();
     return 0;
 }

/*
 * Completed Function
 * Acts as our While Statement handler
 */
 int Parser::parse_whilestmt(){
     #ifdef DEBUG
     cout << "Entered Parse While Statement" << endl;
     #endif
     token = lexer.GetToken();
     if(token.token_type != WHILE){
         syntax_error();
     }
     token = lexer.GetToken();
     if(token.token_type != LPAREN){
         syntax_error();
     }
      // parse expression and get return string
     string returnS = parse_expression();
     if((returnS == "int") || (returnS == "real")) {
         // print type mismatch error for C4
         printTMismatch(token, "C4");
     }
     for (auto& v : holdVarsInProg) {
         if (v.type == returnS) {
             // update type in holdVarsInProg to bool
             v.type = "bool";
         }
     }
     token = lexer.GetToken();
     if(token.token_type != RPAREN){
         syntax_error();
     }
      // parse the body of the while statement
     parse_body();
     return 0;
 }

/*
 * Completed Function
 * Acts as out Switch Statement handler
 */
 int Parser::parse_switchstmt(){
     #ifdef DEBUG
     cout << "Entered Switch Statement" << endl;
     #endif
     token = lexer.GetToken();
     if(token.token_type != SWITCH){
         syntax_error();
     }
     token = lexer.GetToken();
     if(token.token_type != LPAREN){
         syntax_error();
     }
     // parse expression and get return string
     string returnS = parse_expression();
     if((returnS == "real") || (returnS == "bool")) {
         // print type mismatch error for C5
         printTMismatch(token, "C5");
     }
     for (auto& v : holdVarsInProg) {
         if (v.type == returnS) {
             // update type in holdVarsInProg to int
             v.type = "int";
         }
     }
     token = lexer.GetToken();
     if(token.token_type != RPAREN){
         syntax_error();
     }
     token = lexer.GetToken();
     if(token.token_type != LBRACE){
         syntax_error();
     }
     // parse the caselist of the switch statement
     parse_caselist();
     token = lexer.GetToken();
     if(token.token_type != RBRACE){
         syntax_error();
     }
     return 0;
 }

/*
 *
 */
int Parser::parse_caselist(){
	#ifdef DEBUG
		cout << "Entered Parse Case List" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == CASE){
		while(token.token_type == CASE){
			lexer.UngetToken(token);
			parse_case();
			token = lexer.GetToken();
		}
		lexer.UngetToken(token);
	}
	else{
		syntax_error();
	}

	return 0;
}

int Parser::parse_case(){
	#ifdef DEBUG
		cout << "Entered Parse Case" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type != CASE){
		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != NUM){
		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != COLON){
		syntax_error();
	}
	parse_body();

	return 0;
}

int main(){
	#ifdef DEBUG
		cout << "Entered Main" << endl;
	#endif

	int i;
    	Parser* parseProgram = new Parser();
    	i = parseProgram->parse_program();
	return 0;
}
