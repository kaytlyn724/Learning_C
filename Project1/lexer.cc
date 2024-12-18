//Kaytlyn Daffern
//CSE 340 SUMMER
//PROJECT 1
//5-29-2024
//1221263225

/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;
    char myC;
    char hold;
    bool not08 = false;
    std::vector<char> holdBase16;

    input.GetChar(c);
    if (isdigit(c)) {
        //checks for all starting with 0
        if (c == '0') {
            tmp.lexeme = "0";
            input.GetChar(hold);
            if(hold == 'x') {
                char val1;
                char val2;
                input.GetChar(val1);
                input.GetChar(val2);
                if(val1 == '0' && val2 == '8') {
                    //base 08 num
                    tmp.lexeme += hold;
                    tmp.lexeme += val1;
                    tmp.lexeme += val2;
                    tmp.token_type = BASE08NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } else if(val1 == '1' && val2 == '6') {
                    //base 16 num
                    tmp.lexeme += hold;
                    tmp.lexeme += val1;
                    tmp.lexeme += val2;
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                } else {
                    //unget
                    input.UngetChar(val2);
                    input.UngetChar(val1);
                    input.UngetChar(hold);
                    tmp.token_type = NUM;
                    tmp.line_no = line_no;
                    return tmp;

                }
            } else if(hold == '.') {
                //real num
                input.GetChar(myC);
                if(isdigit(myC)) {
                    tmp.lexeme += hold;
                    while (!input.EndOfInput() && isdigit(myC)) {
                        //get to end of line
                        tmp.lexeme += myC;
                        input.GetChar(myC);
                    }
                    if (input.EndOfInput()) {
                        //its a realnum
                        tmp.token_type = REALNUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }else if (!input.EndOfInput()) {
                        //not at the end, unget last character
                        //still return realnum for the other stuff
                        input.UngetChar(myC);
                        tmp.token_type = REALNUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }else {
                    //following dot is not a digit return 0 as a num
                    //and then continue with rest as whatever it is
                    input.UngetChar(hold);
                    tmp.token_type = NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }

            } else {
                input.UngetChar(hold);
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                if(c == '8' || c == '9') {
                    //cant be 08
                    not08 = true;
                }
                input.GetChar(c);
            }
            if (input.EndOfInput()) {
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
            else if(!input.EndOfInput()) {
                if (c == 'x' && !(not08)) { //it can be 08
                    char val1;
                    char val2;
                    input.GetChar(val1);
                    input.GetChar(val2);
                    //it was an 08
                    if(val1 == '0' && val2 == '8') {
                        tmp.lexeme += c;
                        tmp.lexeme += val1;
                        tmp.lexeme += val2;
                        tmp.token_type = BASE08NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }else if(val1 == '1' && val2 == '6') {
                        tmp.lexeme += c;
                        tmp.lexeme += val1;
                        tmp.lexeme += val2;
                        tmp.token_type = BASE16NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                    //return num and forget about everything past x
                    else {
                        input.UngetChar(val2);
                        input.UngetChar(val1);
                        input.UngetChar(c);
                        tmp.token_type = NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }else if (c == 'x' && (not08)) { //not 08
                    char val1;
                    char val2;
                    input.GetChar(val1);
                    input.GetChar(val2);
                    //it was a 16
                    if(val1 == '1' && val2 == '6') {
                        tmp.lexeme += c;
                        tmp.lexeme += val1;
                        tmp.lexeme += val2;
                        tmp.token_type = BASE16NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                    //return num and forget about everything past x
                    else {
                        input.UngetChar(val2);
                        input.UngetChar(val1);
                        input.UngetChar(c);
                        tmp.token_type = NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }else if (c == '.') {
                    input.GetChar(hold);
                    if(isdigit(hold)) {
                        tmp.lexeme += c;
                        tmp.lexeme += hold;
                        input.GetChar(c);
                        while (!input.EndOfInput() && isdigit(c)) {
                            tmp.lexeme += c;
                            input.GetChar(c);
                        }
                        if (input.EndOfInput()) {
                            tmp.token_type = REALNUM;
                            tmp.line_no = line_no;
                            return tmp;
                        }else if (!input.EndOfInput()) {
                            input.UngetChar(c);
                            tmp.token_type = REALNUM;
                            tmp.line_no = line_no;
                            return tmp;
                        }
                    }else {
                        input.UngetChar(hold);
                        input.UngetChar(c);
                        tmp.token_type = NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }else if ((c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F')) {
                    holdBase16.push_back(c);
                    input.GetChar(c);
                    while (!input.EndOfInput() && ((c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F') || isdigit(c))) {
                        holdBase16.push_back(c);
                        input.GetChar(c);
                    }
                    if(c == 'x') {
                        char val1;
                        char val2;
                        input.GetChar(val1);
                        input.GetChar(val2);
                        if(val1 == '1' && val2 == '6') {
                            for(int i = 0; i < holdBase16.size(); i++) {
                                tmp.lexeme += holdBase16[i];
                            }
                            tmp.lexeme += c;
                            tmp.lexeme += val1;
                            tmp.lexeme += val2;
                            tmp.token_type = BASE16NUM;
                            tmp.line_no = line_no;
                            return tmp;
                        }else {
                            input.UngetChar(val2);
                            input.UngetChar(val1);
                            input.UngetChar(c);
                            for(int i = holdBase16.size() - 1; i >= 0; i--) {
                                input.UngetChar(holdBase16[i]);
                            }
                            tmp.token_type = NUM;
                            tmp.line_no = line_no;
                            return tmp;
                        }
                    } else if(!input.EndOfInput() || input.EndOfInput()) {
                        input.UngetChar(c);
                        for(int i = holdBase16.size() - 1; i >= 0; i--) {
                            input.UngetChar(holdBase16[i]);
                        }
                        tmp.token_type = NUM;
                        tmp.line_no = line_no;
                        return tmp;
                    }
                }else {
                    input.UngetChar(c);
                    tmp.token_type = NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }
            }
        }

    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
    return tmp;
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
