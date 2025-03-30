#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

// Node class for Seat Linked List
class SeatNode {
public:
    int seatNumber;
    SeatNode* next;

    SeatNode(int num) {
        seatNumber = num;
        next = nullptr;
    }
};

// Flight class
class Flight {
public:
    int flightID;
    string destination;
    string departureTime;
    float price;
    SeatNode* seatHead; // Linked List for seats
    queue<string> waitingList; // Queue for waiting passengers

    Flight(int id, string dest, string time, float price, int totalSeats) {
        this->flightID = id;
        this->destination = dest;
        this->departureTime = time;
        this->price = price;

        // Create linked list for seats
        seatHead = nullptr;
        for (int i = totalSeats; i > 0; i--) {
            SeatNode* newSeat = new SeatNode(i);
            newSeat->next = seatHead;
            seatHead = newSeat;
        }
    }

    void displayFlight() {
        cout << "Flight ID: " << flightID << " | Destination: " << destination
             << " | Time: " << departureTime << " | Price: $" << price << endl;
        cout << "Available Seats: ";
        SeatNode* temp = seatHead;
        while (temp) {
            cout << temp->seatNumber << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    bool bookSeat(string passengerName, unordered_map<string, pair<int, int>>& passengerBookings) {
        if (seatHead != nullptr) {  
            int seatNum = seatHead->seatNumber;
            SeatNode* temp = seatHead;
            seatHead = seatHead->next;  // Remove booked seat from linked list
            delete temp;

            passengerBookings[passengerName] = {flightID, seatNum};
            cout << "Booking Successful! Seat Number: " << seatNum << endl;
            return true;
        } else {
            cout << "Flight is Full! Adding to Waiting List...\n";
            waitingList.push(passengerName);
            return false;
        }
    }

    void cancelSeat(string passengerName, unordered_map<string, pair<int, int>>& passengerBookings) {
        if (passengerBookings.find(passengerName) == passengerBookings.end()) {
            cout << "No Booking Found!\n";
            return;
        }

        int seatNum = passengerBookings[passengerName].second;
        passengerBookings.erase(passengerName);

        if (!waitingList.empty()) {
            string nextPassenger = waitingList.front();
            waitingList.pop();
            cout << "Seat allocated to waiting passenger: " << nextPassenger << endl;
            passengerBookings[nextPassenger] = {flightID, seatNum};
        } else {
            SeatNode* newSeat = new SeatNode(seatNum);
            newSeat->next = seatHead;
            seatHead = newSeat;
            cout << "Seat is now available for booking.\n";
        }
    }
};

// Global list of flights (Vector)
vector<Flight> flights;
unordered_map<string, pair<int, int>> passengerBookings; // Passenger -> (FlightID, SeatNo)
int flightCounter = 1;

// Admin Functions
class Admin {
public:
    static void addFlight() {
        string destination, time;
        float price;
        int totalSeats;
        cout << "Enter Destination: ";
        cin >> destination;
        cout << "Enter Departure Time: ";
        cin >> time;
        cout << "Enter Ticket Price: ";
        cin >> price;
        cout << "Enter Total Seats: ";
        cin >> totalSeats;

        flights.push_back(Flight(flightCounter++, destination, time, price, totalSeats));
        cout << "Flight Added Successfully!\n";
    }

    static void removeFlight() {
        int id;
        cout << "Enter Flight ID to Remove: ";
        cin >> id;

        for (auto it = flights.begin(); it != flights.end(); it++) {
            if (it->flightID == id) {
                flights.erase(it);
                cout << "Flight Removed Successfully!\n";
                return;
            }
        }
        cout << "Flight Not Found!\n";
    }

    static void viewFlights() {
        if (flights.empty()) {
            cout << "No flights available!\n";
            return;
        }

        for (Flight &flight : flights) {
            flight.displayFlight();
        }
    }
};

// Passenger Functions
class Passenger {
public:
    static void bookTicket() {
        if (flights.empty()) {
            cout << "No flights available!\n";
            return;
        }

        Admin::viewFlights();

        int flightID;
        string passengerName;
        cout << "Enter Flight ID to Book: ";
        cin >> flightID;
        cout << "Enter Your Name: ";
        cin >> passengerName;

        for (Flight &flight : flights) {
            if (flight.flightID == flightID) {
                flight.bookSeat(passengerName, passengerBookings);
                return;
            }
        }
        cout << "Invalid Flight ID!\n";
    }

    static void cancelTicket() {
        string passengerName;
        cout << "Enter Your Name: ";
        cin >> passengerName;

        if (passengerBookings.find(passengerName) == passengerBookings.end()) {
            cout << "No Booking Found!\n";
            return;
        }

        int flightID = passengerBookings[passengerName].first;
        for (Flight &flight : flights) {
            if (flight.flightID == flightID) {
                flight.cancelSeat(passengerName, passengerBookings);
                return;
            }
        }
    }

    static void viewBooking() {
        string passengerName;
        cout << "Enter Your Name: ";
        cin >> passengerName;

        if (passengerBookings.find(passengerName) == passengerBookings.end()) {
            cout << "No Booking Found!\n";
            return;
        }

        int flightID = passengerBookings[passengerName].first;
        int seatNum = passengerBookings[passengerName].second;
        cout << "Booking Details: Flight ID: " << flightID << " | Seat No: " << seatNum << endl;
    }
};

// Login System
void loginSystem() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        cout << "Admin Login Successful!\n";
        while (true) {
            int choice;
            cout << "\n1. Add Flight\n2. Remove Flight\n3. View Flights\n4. Logout\nEnter Choice: ";
            cin >> choice;
            if (choice == 1)
                Admin::addFlight();
            else if (choice == 2)
                Admin::removeFlight();
            else if (choice == 3)
                Admin::viewFlights();
            else
                break;
        }
    } else {
        cout << "Passenger Login Successful!\n";
        while (true) {
            int choice;
            cout << "\n1. Book Ticket\n2. Cancel Ticket\n3. View Booking\n4. Logout\nEnter Choice: ";
            cin >> choice;
            if (choice == 1)
                Passenger::bookTicket();
            else if (choice == 2)
                Passenger::cancelTicket();
            else if (choice == 3)
                Passenger::viewBooking();
            else
                break;
        }
    }
}

int main() {
    while (true) {
        cout << "\n===== Airline Reservation System =====\n";
        cout << "1. Login\n2. Exit\nEnter Choice: ";
        int choice;
        cin >> choice;
        if (choice == 1)
            loginSystem();
        else
            break;
    }
    cout << "Thank you for using the Airline Reservation System!\n";
    return 0;
}
