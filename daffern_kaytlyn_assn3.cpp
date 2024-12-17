/*
Author: Kaytlyn Daffern
Date: Oct 21 2022
Description: C++ file used to "baconize" strings of words/letters a\
nd also have the functionality to reverse it back to regular letter\
s. Does this through input files (one to read from and one to type \
into).
Usage: <exe> <input file> <-bc|-e> <output file>
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

const char* englishToBaconCode(char); //instantiates the englishToBaconCode function

char baconCodeToEnglish(const char*); //instantiates the baconCodeToEnglish function

int main(int argc, char** argv) {
    
    //char* exec = argv[0]; //creates a variable to represent the first input by the user
    
    char* userInFile = argv[1]; //creates a variable to represent the infile the user will have
    
    char* userOutFile = argv[3]; //creates a variable to represent the outfile the user will have
    
    char* userCommand = argv[2]; //creates a variable to represent the command the user will use
    
    char compare2p1[] = "-e"; //creates a variable to represent to english to compare with the 3 input by the user
    char compare2p2[] = "-bc"; //creates a variable to represent to bacon code to compare with the 3 input by the user
    
    if(argc == 4) { //checks if there are 4 inputs from the user
        
        if (strcmp(userCommand, compare2p1) == 0) { //if the input by the user for the command represents to english continue
            std::cout << "Translating " << userInFile << " to English" << std::endl;
            
            //instantiate in and out file to be used
            std::ifstream infile;
            std::ofstream outfile;
            
            //instantiate opening the files previously made
            infile.open(userInFile);
            outfile.open(userOutFile);
            
            //while the infile is not at the end
            while(!infile.eof()) {
                
                //create the line length (since its in bacon its extra long) and the token for breaking the line
                char line[1530];
                char * token;
                
                infile.getline(line, 1530); //gets the line from the file to tokenize
                
                token = strtok(line, "|");
                
                //while the token isnt null send it through baconcode to english
                while(token != NULL) {
                    outfile << baconCodeToEnglish(token);
                    token = strtok(NULL, "|");
                }
                
                outfile << std::endl;
                
            }
            
            //close files before ending
            infile.close();
            outfile.close();
            
            std::cout << userOutFile << " file completed." << std::endl;
        }
        else if (strcmp(userCommand, compare2p2) == 0) { //if the input by the user for the command represents to bacon code continue
            std::cout << "Translating " << userInFile << " to BACON code" << std::endl;
            
            //instantiate in and out file to be used
            std::ifstream infile;
            std::ofstream outfile;
            
            //instantiate opening the files previously made
            infile.open(userInFile);
            outfile.open(userOutFile);
            
            //while the infile is not at the end
            while(!infile.eof()) {
                
                //create the line length and token to tokenize the line
                char line[255];
                char * token;
                
                //get the line to be used from the file
                infile.getline(line, 255);
                
                token = strtok(line, "\n");
                
                //while the token isnt null send it through the english to baconcode
                while(token != NULL) {
                    //get each letter from the token with the for loop
                    for(int j = 0; j < strlen(token); j++) {
                        outfile << englishToBaconCode(token[j]);
                        if(token[j+1] != NULL) {
                            outfile << "|";
                        }
                    }
                    token = strtok(NULL, "\n");
                }
                
                outfile << std::endl;
            }
            
            //close files before ending
            infile.close();
            outfile.close();
            
            std::cout << userOutFile << " file completed." << std::endl;
        }
        else { //user did not use an available command
            std::cout << "Error -- usage: <exe> <input file> <-bc|-e> <output file>" << std::endl;
        }
    }
    else {
        std::cout << "Error -- usage: <exe> <input file> <-bc|-e> <output file>" << std::endl;
    }
    
}

char baconCodeToEnglish(const char* check) { //function baconCode to english takes in a const char pointer
    
    //function instantiates a const char * for each possible letter or value and sets equal to their baconcode
    const char *checkA = "aaaaa";
    const char *checkB = "aaaab";
    const char *checkC = "aaaba";
    const char *checkD = "aaabb";
    const char *checkE = "aabaa";
    const char *checkF = "aabab";
    const char *checkG = "aabba";
    const char *checkH = "aabbb";
    const char *checkI = "abaaa";
    const char *checkJ = "abaab";
    const char *checkK = "ababa";
    const char *checkL = "ababb";
    const char *checkM = "abbaa";
    const char *checkN = "abbab";
    const char *checkO = "abbba";
    const char *checkP = "abbbb";
    const char *checkQ = "baaaa";
    const char *checkR = "baaab";
    const char *checkS = "baaba";
    const char *checkT = "baabb";
    const char *checkU = "babaa";
    const char *checkV = "babab";
    const char *checkW = "babba";
    const char *checkX = "babbb";
    const char *checkY = "bbaaa";
    const char *checkZ = "bbaab";
    const char *checkSpc = "/";
    const char *checkNum = "!!!!!";
    
    //functionruns through every possible if by checking with string compare and returning the letter or value it should be
    if (strcmp(check, checkA) == 0) {
        return 'A';
    }
    else if (strcmp(check, checkB) == 0) {
        return 'B';
    }
    else if (strcmp(check, checkC) == 0) {
        return 'C';
    }
    else if (strcmp(check, checkD) == 0) {
        return 'D';
    }
    else if (strcmp(check, checkE) == 0) {
        return 'E';
    }
    else if (strcmp(check, checkF) == 0) {
        return 'F';
    }
    else if (strcmp(check, checkG) == 0) {
        return 'G';
    }
    else if (strcmp(check, checkH) == 0) {
        return 'H';
    }
    else if (strcmp(check, checkI) == 0) {
        return 'I';
    }
    else if (strcmp(check, checkJ) == 0) {
        return 'J';
    }
    else if (strcmp(check, checkK) == 0) {
        return 'K';
    }
    else if (strcmp(check, checkL) == 0) {
        return 'L';
    }
    else if (strcmp(check, checkM) == 0) {
        return 'M';
    }
    else if (strcmp(check, checkN) == 0) {
        return 'N';
    }
    else if (strcmp(check, checkO) == 0) {
        return 'O';
    }
    else if (strcmp(check, checkP) == 0) {
        return 'P';
    }
    else if (strcmp(check, checkQ) == 0) {
        return 'Q';
    }
    else if (strcmp(check, checkR) == 0) {
        return 'R';
    }
    else if (strcmp(check, checkS) == 0) {
        return 'S';
    }
    else if (strcmp(check, checkT) == 0) {
        return 'T';
    }
    else if (strcmp(check, checkU) == 0) {
        return 'U';
    }
    else if (strcmp(check, checkV) == 0) {
        return 'V';
    }
    else if (strcmp(check, checkW) == 0) {
        return 'W';
    }
    else if (strcmp(check, checkX) == 0) {
        return 'X';
    }
    else if (strcmp(check, checkY) == 0) {
        return 'Y';
    }
    else if (strcmp(check, checkZ) == 0) {
        return 'Z';
    }
    else if (strcmp(check, checkSpc) == 0) {
        return ' ';
    }
    else if (strcmp(check, checkNum) == 0) {
        return '#';
    }
    else {
        return '#';
    }
    return 0;
    
}

const char* englishToBaconCode(char letter) { //english to baconCode takes in a char
    
    //checks if its a letter with its integer value and then subtracts 32 to make them all uppercase
    if((int(letter) >= 97) && (int(letter) <= 122)) {
        letter = letter - 32;
    }
    
    //switch used to run through what letter should equal and returns the value of that letter or symbol in baconcode
    switch(letter) {
        case 'A':
            return ("aaaaa");
            break;
        case 'B':
            return ("aaaab");
            break;
        case 'C':
            return ("aaaba");
            break;
        case 'D':
            return ("aaabb");
            break;
        case 'E':
            return ("aabaa");
            break;
        case 'F':
            return ("aabab");
            break;
        case 'G':
            return ("aabba");
            break;
        case 'H':
            return ("aabbb");
            break;
        case 'I':
            return ("abaaa");
            break;
        case 'J':
            return ("abaab");
            break;
        case 'K':
            return ("ababa");
            break;
        case 'L':
            return ("ababb");
            break;
        case 'M':
            return ("abbaa");
            break;
        case 'N':
            return ("abbab");
            break;
        case 'O':
            return ("abbba");
            break;
        case 'P':
            return ("abbbb");
            break;
        case 'Q':
            return ("baaaa");
            break;
        case 'R':
            return ("baaab");
            break;
        case 'S':
            return ("baaba");
            break;
        case 'T':
            return ("baabb");
            break;
        case 'U':
            return ("babaa");
            break;
        case 'V':
            return ("babab");
            break;
        case 'W':
            return ("babba");
            break;
        case 'X':
            return ("babbb");
            break;
        case 'Y':
            return ("bbaaa");
            break;
        case 'Z':
            return ("bbaab");
        case ' ':
            return ("/");
        default:
            return ("!!!!!");
            break;
    }
}
            
