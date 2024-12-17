/* CSE 310: Recitation 3
   Kaytlyn Daffern
   1221263225
   06/05/2023
   It has warnings but it runs fine. Code uses a priority queue to create a plane landing system. Each plane has a time in queue and the plane with the lowest time will be popped first until all planes are gone.
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// struct to represent each plane
struct Plane {
    string flightNumber; // The flight number of the plane
    int flightWaitTime; // The wait time of the plane in minutes

    // Constructor to initialize the flightNumber and flightWaitTime of the plane
    Plane(string fNumber, int wTime) : flightNumber(fNumber), flightWaitTime(wTime) {}
};

// class for the priority queue
class PriorityQueue {
private:
    vector<Plane> heap; // The heap to store the planes

    // Helper function to heapify the heap while adding an element starting from the bottom
    void HeapifyGoingUp(int index) {
        int parent = (index - 1) / 2; // parent node
        while (index > 0 && heap[parent].flightWaitTime > heap[index].flightWaitTime) {
            // Swap the parent and current node if the parent has a larger wait time
            swap(heap[parent], heap[index]);
            index = parent; // Move to the parent node
            parent = (index - 1) / 2; // new parent node
        }
    }

    // Helper function to heapify the heap while adding an element starting from the top
    void HeapifyGoingDown(int index) {
        int leftChild = (2 * index) + 1; // left child node
        int rightChild = (2 * index) + 2; // right child node
        int smallest = index; // assumes the current node has the smallest wait time

        // Find the smallest child among the current node and its children
        if (leftChild < heap.size() && heap[leftChild].flightWaitTime < heap[smallest].flightWaitTime) {
            smallest = leftChild;
        }

        if (rightChild < heap.size() && heap[rightChild].flightWaitTime < heap[smallest].flightWaitTime) {
            smallest = rightChild;
        }

        // Swap elements if necessary and continue heapifying
        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            HeapifyGoingDown(smallest);
        }
    }

public:
    // Method to insert a plane into the priority queue
    void push(const Plane& plane) {
        heap.push_back(plane); // Add to the end
        HeapifyGoingUp(heap.size() - 1); // Heapify the heap starting from the bottom
    }

    // removes the plane with the minimum wait time
    void pop() {
        if (empty()) {
            return;
        }

        // swaps the min/root with the last element, removes the last element, and then heapify from the root
        swap(heap[0], heap.back());
        heap.pop_back();
        HeapifyGoingDown(0); // Heapify the heap starting from the top
    }

    // gets the plane with the minimum wait time from the priority queue without removing it
    const Plane& top() const {
        return heap[0]; // Return the root of the heap or min
    }

    // checks if the priority queue is empty
    bool empty() const {
        return heap.empty(); // Returns true if the heap is empty
    }
};

int main() {
    // Creates an instance of the PriorityQueue class to store landing requests
    PriorityQueue landingRequests;
    int option = 0;

    // Main menu loop until user asks to exit
    while (option != 4) {
        cout << "Welcome to the Plane Landing System" << endl;
        cout << "1. Make a landing request" << endl;
        cout << "2. Land a Plane" << endl;
        cout << "3. List all the landing requests" << endl;
        cout << "4. Exit" << endl;
        cout << "Select your option: ";
        cin >> option;
        cout << endl;

        switch (option) {
            case 1: {
                string flightNumber;
                int flightWaitTime;

                // Gets flight details from the user to add to queue
                cout << "Enter the flight number: ";
                cin >> flightNumber;
                cout << endl;
                cout << "Enter the flight wait time (minutes): ";
                cin >> flightWaitTime;
                cout << endl;

                // Adds the plane to the landing requests priority queue
                landingRequests.push(Plane(flightNumber, flightWaitTime));
                cout << "Landing request was added to the queue." << endl;
                break;
            }
            case 2: {
                // lands the next plane with the smallest time if there is any
                if (!landingRequests.empty()) {
                    Plane nextPlane = landingRequests.top(); // Make a copy of the plane with the minimum wait time
                    landingRequests.pop();
                    cout << "Landed plane: " << nextPlane.flightNumber << endl;
                } else {
                    cout << "There are no planes in the landing queue." << endl;
                }
                break;
            }
            case 3: {
                // Lists all the landing requests in the queue
                if (!landingRequests.empty()) {
                    cout << "List of landing requests:" << endl;
                    PriorityQueue tempQueue = landingRequests;  // Creates a copy of landingRequests
                    while (!tempQueue.empty()) {
                        const Plane& p = tempQueue.top();
                        cout << "Flight number: " << p.flightNumber << ", Wait time: " << p.flightWaitTime << endl;
                        tempQueue.pop(); // Removes the plane with the min wait time from the temporary queue
                    }
                } else {
                    cout << "There are no landing requests." << endl;
                }
                break;
            }
            case 4: {
                cout << "Goodbye. " << endl;
                break;
            }
            default: {
                cout << "That is not one of the options. Please choose a number between 1-4." << endl;
            }
        }
    }
}
