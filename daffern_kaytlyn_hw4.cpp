/* 
Name: Kaytlyn Daffern
Date: 11/7/22
Descritpion: Zombie game that uses 2d arrays to instantiate a board and have zombies grow and infect others by being either a bitey zombie or a berserker zombie and attacking humans. Game ends when it either runs for too long, there are only humans left, or only zombies. enjoy ;)
Usage: ./<exe> follow user prompt (integer between 4 and 40) (integer between 4 and 40) (float between 0 and 1) (float between 0 and 1)
*/

#include <stdio.h>
#include <iostream>
#include <random>
#include <time.h>

//instantiates the functions I will be using to help run the game including a few that check for a true or false or simply print something while run iteration is used to actually implement all of the changes in the game

int run_iteration(int**, char**, int, int, float, float);

void print_map(char**, int, int);

bool valid_bite(int**, char**, int, int, int, int);

int bite(int, float);

bool is_berserker(float);

float randFunc(int, int);

int main(int argc, char** argv) {
    int width;
    int height;
    float p_of_bite;
    float p_of_berserk;
    int random_x;
    int random_y;
    
    //used for for loops throughout main
    int i;
    int j;
    int k;
    int l;
    int p;
    int iterate;
    int closeGame;
    
    //seed the rand function with the time
    srand(time(NULL));
    
    //basic user interface by offering the bounds to follow and if not then it will prompt the same question until it is followed by the user
    std::cout << "Welcome to ze zombie apocalypse!" << std::endl;
    
    std::cout << "Enter map width (between 4 and 40): ";
    std::cin >> width;
    
    while(!((width >= 4) && (width <= 40))) {
        std::cout << "Not a valid number. Try again." << std::endl;
        std::cout << "Enter map width (between 4 and 40): ";
        std::cin >> width;
    }
    
    std::cout << "Enter map height (between 4 and 40): ";
    std::cin >> height;
    
    while(!((height >= 4) && (height <= 40))) {
        std::cout << "Not a valid number. Try again." << std::endl;
        std::cout << "Enter map height (between 4 and 40): ";
        std::cin >> height;
    }
    
    std::cout << "Enter zombie bite accuracy (a float between 0 and 1): ";
    std::cin >> p_of_bite;
    
    while(!((p_of_bite > 0) && (p_of_bite < 1))) {
        std::cout << "Not a valid number. Try again." << std::endl;
        std::cout << "Enter zombie bite accuracy (a float between 0 and 1): ";
        std::cin >> p_of_bite;
        
    }
    
    std::cout << "Enter berserker probability (a float between 0 and 1): ";
    std::cin >> p_of_berserk;
    
    while(!((p_of_berserk > 0) && (p_of_berserk < 1))) {
        std::cout << "Not a valid number. Try again." << std::endl;
        std::cout << "Enter berserker probability (a float between 0 and 1): ";
        std::cin >> p_of_berserk;
    }
    
    //instantiates my 2d arrays used to hold type of people and their strengths
    int** array_zombie;
    
    array_zombie = new int*[height];
    
    for(k = 0; k < height; k++) {
        array_zombie[k] = new int[width];
    }
    
    char** array_states;
    
    array_states = new char*[height];
    
    for(l = 0; l < height; l++) {
        array_states[l] = new char[width];
    }
    
    //makes everyone on the map a human with no strength
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            array_zombie[i][j] = 0;
            array_states[i][j] = '.';
        }
    }
    
    //selects a random location on the board and gives it a strength of 5 and makes it a bitey zombie
    random_x = (int)(randFunc(0, width));
    random_y = (int)(randFunc(0, height));
    
    array_zombie[random_x][random_y] = 5;
    array_states[random_x][random_y] = 'Z';
    
    std::cout << "Strain dropped at coordinate (" << random_x << ", " << random_y << "). Zombie of strength 5 spawned." << std::endl;
    std:: cout << std::endl;
    
    std::cout << "Iteration: 0" << std::endl;
    std::cout << "Number of Zombies: 1" << std::endl;
    print_map(array_states, width, height);
    std:: cout << std::endl;
    
    //calls my run iteration function over and over as well as the print map to play the game and show what iteration it is at as well as the current number of zombies on each board
    for(p = 1; p <= 1000; p++) {
        iterate = run_iteration(array_zombie, array_states, width, height, p_of_bite, p_of_berserk);
        std::cout << "Iteration: " << p << std::endl;
        std::cout << "Number of Zombies: " << iterate << std::endl;
        print_map(array_states, width, height);
        std:: cout << std::endl;
        
        //if there are no zombies then game stops and humans win
        if(iterate == 0) {
            std::cout << "The Humans slaughtered all zombies and are declared victorious." << std::endl;
            break;
        }
        //if there are only zombies then the game stops and zombies win
        else if(iterate == (width * height)) {
            std::cout << "The Zombies killed of all humans and are declared victorious." << std::endl;
            break;
        }
        //the game reached the 1000th iteration and ended for its extent
        else if(p == 1000) {
            std::cout << "Humans and Zombies have learned to live in harmony. Tie game. " << std::endl;
        }
    }
    //dealocated my arrays before ran again for a new game
    for(closeGame = 0; closeGame < height; closeGame++) {
        delete[] array_zombie[closeGame];
        delete[] array_states[closeGame];
    }
    
    delete[] array_zombie;
    delete[] array_states;
    
}

int run_iteration(int** array_zombie, char** array_states, int width, int height, float p_of_bite, float p_of_berserk) {
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int newStrength;
    int zombieCount = 0;
    int randLocation;
    int randLocation1;
    int randLocation2;
    int randLocation3;

    //for loop used to increment the zombies strength and if it is 10 then turn them back into a human
    for(m = 0; m < height; m++) {
        for(n = 0; n < width; n++) {
            if((array_states[n][m] == 'Z') || (array_states[n][m] == 'B')) {
                array_zombie[n][m] += 1;
                if(array_zombie[n][m] >= 10) {
                    array_states[n][m] = '.';
                    array_zombie[n][m] = 0;
                }
            }
        }
    }
    
    //for loop used to turn all injured people on the board into zombies by giving them a chance to become a berserker or just a bitey zombie
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            if(array_zombie[i][j] > 0) {
                if(array_states[i][j] == '*') {
                    if(is_berserker(p_of_berserk)) {
                        array_states[i][j] = 'B';
                    }
                    else {
                        array_states[i][j] = 'Z';
                    }
                }
            
            }
            else {
                array_zombie[i][j] = 0;
                array_states[i][j] = '.';
            }
        }
    }
    
    //for loop to check for only zombies and use them to bite other people
    for(l = 0; l < height; l++) {
        for(k = 0; k < width; k++) {
            if((array_zombie[k][l] > 0) && (array_zombie[k][l] < 10)) {
                //if the person at this location is a bitey zombie it continues
                if(array_states[k][l] == 'Z') {
                    
                    //creates a random location from 0 to 3 to check around the current zombie
                    randLocation = int(randFunc(0,4));
                    //checks if the zombie would pass and bite someone otherwise it skips the next part to move onto the next zombie
                    newStrength = bite(array_zombie[k][l], p_of_bite);
                    if(newStrength > 0) {
                        //these if statements give the zombie all possible chance to find an available person and only bite one time before moving onto the next person
                        
                        //if the random location is 0 then this is the northern spot (located directly above this position) and is checked if it is in bounds and is a regular human to be bit
                        if(randLocation == 0) {
                            if(valid_bite(array_zombie, array_states, width, height, k, (l - 1))) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k][l-1] = newStrength;
                                array_states[k][l-1] = '*';
                            }
                            else {
                                randLocation += 1;
                            }
                        }
                        //if the random location is 1 then this is the eastern spot (located directly to the right of this position) and is checked if it is in bounds and is a regular human to be bit
                        if(randLocation == 1) {
                            if(valid_bite(array_zombie, array_states, width, height, (k + 1), l)) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k+1][l] = newStrength;
                                array_states[k+1][l] = '*';
                            }
                            else {
                                randLocation += 1;
                            }
                        }
                        //if the random location is 2 then this is the southern spot (located directly below this position) and is checked if it is in bounds and is a regular human to be bit
                        if(randLocation == 2) {
                            if(valid_bite(array_zombie, array_states, width, height, k, (l + 1))) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k][l+1] = newStrength;
                                array_states[k][l+1] = '*';
                            }
                            else {
                                randLocation += 1;
                            }
                        }
                        //if the random location is 3 then this is the western spot (located directly to the left this position) and is checked if it is in bounds and is a regular human to be bit
                        if(randLocation == 3) {
                            if(valid_bite(array_zombie, array_states, width, height, (k - 1), l)) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k-1][l] = newStrength;
                                array_states[k-1][l] = '*';
                            }
                        }
                    }
                }
                
                else if(array_states[k][l] == 'B') {
                    //if the person is a berserker at this location it continues
                    
                    //instantiates 3 random numbers to be checked for locations around the berserker from 0 to 3
                    randLocation1 = int(randFunc(0,4));
                    randLocation2 = int(randFunc(0,4));
                    randLocation3 = int(randFunc(0,4));
                    
                    //checks if the zombie would pass and bite someone otherwise it skips the next part to move onto the next zombie
                    newStrength = bite(array_zombie[k][l], p_of_bite);
                    if(newStrength > 0) {
                        //these if statements give the zombie 3 possible chance to find an available person and bite 3 at most before moving onto the next person
                        
                        //if the random location is 0 then this is the northern spot (located directly above this position) and is checked if it is in bounds and is a regular human to be bit
                        if((randLocation1 == 0) || (randLocation2 == 0) || (randLocation3 == 0)) {
                            if(valid_bite(array_zombie, array_states, width, height, k, (l - 1))) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k][l-1] = newStrength;
                                array_states[k][l-1] = '*';
                            }
                        }
                        //if the random location is 1 then this is the eastern spot (located directly to the right of this position) and is checked if it is in bounds and is a regular human to be bit
                        if((randLocation1 == 1) || (randLocation2 == 1) || (randLocation3 == 1)) {
                            if(valid_bite(array_zombie, array_states, width, height, (k + 1), l)) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k+1][l] = newStrength;
                                array_states[k+1][l] = '*';
                            }
                        }
                        //if the random location is 2 then this is the southern spot (located directly below this position) and is checked if it is in bounds and is a regular human to be bit
                        if((randLocation1 == 2) || (randLocation2 == 2) || (randLocation3 == 2)) {
                            if(valid_bite(array_zombie, array_states, width, height, k, (l + 1))) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k][l+1] = newStrength;
                                array_states[k][l+1] = '*';
                            }
                        }
                        //if the random location is 3 then this is the western spot (located directly to the left this position) and is checked if it is in bounds and is a regular human to be bit
                        if((randLocation1 == 3) || (randLocation2 == 3) || (randLocation3 == 3)) {
                            if(valid_bite(array_zombie, array_states, width, height, (k - 1), l)) {
                                //the bite was valid and this location becomes a zombie
                                array_zombie[k-1][l] = newStrength;
                                array_states[k-1][l] = '*';
                            }
                        }
                    }
                }
            }
        }
    }
    //sets the zombie counter to 0 before checking for zombies
    zombieCount = 0;
    
    //uses a for loop to run through the states and if there are any types of zombies (bitey or berserker) it adds one to the counter
    for(m = 0; m < height; m++) {
        for(n = 0; n < width; n++) {
            if((array_states[n][m] == 'Z') || (array_states[n][m] == 'B')) {
                zombieCount += 1;
            }
        }
    }
    return zombieCount;
}

void print_map(char** array_states, int width, int height) {
    int i;
    int j;
    
    //uses a for loop to output all people or zombies in the array states with a space between each one
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            if(i < (width - 1)) {
                std::cout << array_states[i][j] << " ";
            }
            else {
                std::cout << array_states[i][j];
            }
        }
        std::cout << std::endl;
    }
    
}

bool valid_bite(int** array_zombie, char** array_states, int width, int height, int neighbor_x, int neighbor_y) {
    //checks if this location of a bite is valud by seeing if it is in range both with the width and height and then checking if it does not have a strength and if it is a human otherwise it is false
    if((neighbor_x >= 0) && (neighbor_x <= (width - 1))) {
        if((neighbor_y >= 0) && (neighbor_y <= (height - 1))) {
            if(array_zombie[neighbor_x][neighbor_y] == 0) {
                if(array_states[neighbor_x][neighbor_y] == '.') {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

int bite(int zombie_strength, float p_of_bite) {
    //instantiates a random value between 0 and 1
    float randVal;
    randVal = ((rand() % 11) / 10.0);
    //checks to see if the random value passes by being below the function given, if less than 0.1 the strength is set to 1 otherwise the strength is the random value * 10
    if(randVal <= ((p_of_bite * zombie_strength) / 10.0)) {
        if(randVal < 0.1) {
            return 1;
        }
        else {
            return ((int)(randVal * 10));
        }
    }
    else {
        return 0;
    }
}

bool is_berserker(float p_of_berserk) {
    //instantiates a random value between 0 and 1
    float randVal;
    randVal = ((rand() % 11) / 10.0);
    //checks to see if the random value passes by being below the chance of being a berserk and returns a bool value based on that
    if(randVal <= p_of_berserk) {
        return true;
    }
    else {
        return false;
    }
}

float randFunc(int min, int max) {
    //function i created to help get random numbers (int) between a min and a max + 1 and returns the random number
    float randNum;
    randNum = rand() % (max - min) + min;
    return randNum;
}
