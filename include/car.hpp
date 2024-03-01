#pragma once
#include <bits/stdc++.h>
#include <ctime>
#include <chrono>

using namespace std;

// Car
class Car{
    public:
        string Model;               // Model of the car
        string Vehicle_Number;      // Vehicle Number/License plate number of the car
        string condition;           // Condtion of the car
        bool Available;             // Availability of the car
        string rented_to;           // User ID of the person the car is rented to
        string rented_on;           // Date and time when the car was rented
        string return_date;         // Date and time when the car is supposed to be returned
        int price = 0;              // Per day rent of the car

        Car(string Model = "***", string Vehicle_Number = "***", string condition = "***", bool Available = false, string rented_to = "***", string return_date = "***", int price = -1);
        ~Car(){};

        int Calculate_Rent(string curr_date_time, string issue_date_time, int type);
        int Calculate_Fine(string curr_date_time, string return_date_time);

        int Evaluate_Condition_and_Update_Details();
};

// Car Database
class CarDB{
    public:
        static Car* dummy;  // pointer to a dummy Car object
        vector<Car> list;   // List of Car objects in the database

        void Populate(vector<string> car_lines);
        Car* Find(string vehicle_num);
        void Add(string model, string vehicle_num, string condition, int price);
        int available_car_count();
        void Show_Available_Cars();
        void makeAvailable(vector<string> cars);
};