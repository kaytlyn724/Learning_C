/* CSE 310: Recitation 1
   Kaytlyn Daffern
   1221263225
   05/22/2023
   Write a C or C++ program that creates a linked list of US cities. Each entry in the list is to have a city name, city population, maiden household income and a pointer to the next city record. Uses vectors, structs, and a linkedlist class
 */

#include <iostream>
#include <vector>

//struct for each city created
struct City {
    std::string name;
    int population;
    double income;
    City* next; //the pointer to the next city

    //constructor used to initialize the city object to avoid setting these values to random things
    City(const std::string& cityName, int cityPopulation, double cityIncome)
        : name(cityName), population(cityPopulation), income(cityIncome), next(nullptr) {}
};

//linkedlist of cities inputted
class CityLinkedList {
private:
    //pointers to the head and tail of the list
    City* head;
    City* tail;

public:
    //constructor to initialize the pointers
    CityLinkedList() : head(nullptr), tail(nullptr) {}

    //function for adding a city at the tail
    void addAtTail(const std::string& cityName, int cityPopulation, double cityIncome) {
        City* newCity = new City(cityName, cityPopulation, cityIncome);
    
        //if the list is empty when called the new city becomes the head and tail
        if (tail == nullptr) {
            head = newCity;
            tail = newCity;
        }
        //if the list is not empty when called the new city is put after the current tail and becomes the tail
        else {
            tail->next = newCity;
            tail = newCity;
        }

        std::cout << "City record was added at the tail." << std::endl;
    }
    
    //function for adding a city at the head
    void addAtHead(const std::string& cityName, int cityPopulation, double cityIncome) {
        City* newCity = new City(cityName, cityPopulation, cityIncome);
        
        //if the list is empty when called the new city becomes the head and tail
        if (head == nullptr) {
            head = newCity;
            tail = newCity;
        }
        //if the list is not empty when called the new city sets its next to the current head and becomes the head
        else {
            newCity->next = head;
            head = newCity;
        }

        std::cout << "City record was added at the head." << std::endl;
    }
    
    //function used to get multiple cities with the largest population if necessary or just one if there is only one
    std::vector<City*> getLargestPopulation() {
        std::vector<City*> maxPopulationCities;
        int maxPopulation = 0;

        City* current = head;

        //goes through the list to fine the max or multiple maxes
        while (current != nullptr) {
            if (current->population > maxPopulation) {
                maxPopulation = current->population;
                maxPopulationCities.clear();
            }
            if (current->population >= maxPopulation) {
                maxPopulationCities.push_back(current);
            }

        current = current->next;
        }

        return maxPopulationCities;
    }

    //this function references the getLargestPopulation and displays for the user
    void displayLargestPopulation() {
        std::vector<City*> maxPopulationCities = getLargestPopulation();

        if (maxPopulationCities.empty()) {
            std::cout << "No city records." << std::endl;
        } else {
            //goes through the max populated cities and outputs their information
            for (City* city : maxPopulationCities) {
                std::cout << city->name << ", " << city->population << ", " << city->income << ". " << std::endl;
            }
        }
    }
    //this function displays all cities that were added, if there was nothing added it says no records were found
    void displayCities() {
        if (head == nullptr) {
            std::cout << "No city records. " << std::endl;
            return;
        }

        City* current = head;
        while (current != nullptr) {
            std::cout << current->name << ", " << current->population << ", " << current->income << ". " << std::endl;
            current = current->next;
        }
    }
};

// Function to validate if a string represents a valid positive integer
bool CheckIfInteger(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}

// Function to validate if a string represents a valid positive floating-point number
bool CheckIfDouble(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    bool hasDecimal = false;
    bool hasDigit = false;

    for (char c : str) {
        // More than one decimal point found or no digit before the decimal point
        if (c == '.') {
            if (hasDecimal || !hasDigit) {
                return false;
            }
            hasDecimal = true;
        } else if (!std::isdigit(c)) {
            return false; // Non-digit character found
        } else {
            hasDigit = true;
        }
    }

    return hasDigit;
}

int main() {
    CityLinkedList cityList;
    int choice;
    std::string cityName;
    std::string populationStr;
    std::string incomeStr;
    int cityPopulation;
    double cityIncome;
    
    //continues to output the questions until the user chooses option 5
    do {
        std::cout << "1. Add city record at the tail" << std::endl;
        std::cout << "2. Add city record at the head" << std::endl;
        std::cout << "3. Display the city with the largest population" << std::endl;
        std::cout << "4. Display all city records" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << std::endl;
        
        //cases call from the citylist linked list's functons to complete the option the user chooses
        switch (choice) {
            case 1:
                std::cout << "Enter city name: ";
                std::cin >> cityName;
                //check that population input is valid
                do {
                    std::cout << "Enter city population: ";
                    std::cin >> populationStr;
                } while (!CheckIfInteger(populationStr));

                //check that median income input is valid
                do {
                    std::cout << "Enter city median household income: ";
                    std::cin >> incomeStr;
                } while (!CheckIfDouble(incomeStr));

                // Convert validated input strings to numbers
                cityPopulation = std::stoi(populationStr);
                cityIncome = std::stod(incomeStr);

                cityList.addAtTail(cityName, cityPopulation, cityIncome);
                break;
                
            case 2:
                std::cout << "Enter city name: ";
                std::cin >> cityName;

                //check that population input is valid
                do {
                    std::cout << "Enter city population: ";
                    std::cin >> populationStr;
                } while (!CheckIfInteger(populationStr));

                //check that median income input is valid
                do {
                    std::cout << "Enter city median household income: ";
                    std::cin >> incomeStr;
                } while (!CheckIfDouble(incomeStr));

                //Convert validated input strings to numbers
                cityPopulation = std::stoi(populationStr);
                cityIncome = std::stod(incomeStr);

                cityList.addAtHead(cityName, cityPopulation, cityIncome);
                break;
                
            case 3:
                cityList.displayLargestPopulation();
                break;
                
            case 4:
                cityList.displayCities();
                break;
                
            case 5:
                std::cout << "Goodbye. " << std::endl;
                break;
                
            default:
                std::cout << "Invalid choice. Please choose a number between 1-5." << std::endl;
        }

        std::cout << std::endl;

    } while (choice != 5);

    return 0;
}
