#pragma once
#include <bits/stdc++.h>
#include "car.hpp"
#include <ctime>
#include <chrono>

using namespace std;
class Customer;
class Employee;
class Manager;
class CustomerDB;
class EmployeeDB;
class ManagerDB;

// User Classes

// Customer
class Customer{
    public:
        string Name;                    // Name of the customer
        string UserID;                  // User ID of the customer
        string password;                // Customer's password
        vector<string> RentedCarsList;  // List of cars rented
        int Dues;                       // Due amount to be payed
        int record;                     // customer record

        Customer(string name="***", string userid="***", string pass="***", vector<string> rcl={}, int dues = 0, int rec = 3);
        ~Customer(){}; // Desructor function

        void Rent_Car(CarDB &car_db, int type);
        void View_Rented_Cars_and_Dues(CarDB &car_db, int type);
        void Change_Password();

        int Calculate_Dues(CarDB car_db, int type);
        int renting_limit();

};

// Employee
class Employee : public Customer{
    public:
        string verification_code;       // Employee Verification Code

        Employee(string name="***", string userid="***", string pass="***", vector<string> rcl={}, int dues = 0, int rec = 3, string vc="***");
        ~Employee(){}; // Desructor function

        void Return_Customer_Car(CustomerDB &customer_db, CarDB &car_db);
        void Return_Own_Car(CarDB &cardb);
};


// Manager
class Manager : public Employee{
    public:
        Manager(string name="***", string userid="***", string pass="***", vector<string> rcl={}, int dues = 0, int rec = 3, string vc="***");
        ~Manager(){}; // Desructor function

        void Clear_Employee_Dues(EmployeeDB &employee_db);
        void View_All_Cars(CarDB &car_db);
        void Add_Car(CarDB &car_db);
        void Delete_Car(CarDB &car_db);
        void Edit_Car_Details(CarDB &car_db);
        void View_All_Customers(CustomerDB &customer_db);
        void Add_Customer(CustomerDB &customer_db);
        void Delete_Customer_Acc(CustomerDB &customer_db, CarDB &car_db);
        void Edit_Customer_Details(CustomerDB &customer_db, CarDB &car_db);
        void View_All_Employees(EmployeeDB &employee_db);
        void Add_Employee(EmployeeDB &employee_db);
        void Delete_Employee_Acc(EmployeeDB &employee_db, CarDB &car_db);
        void Edit_Employee_Details(EmployeeDB &employee_db, CarDB &car_db);
};


// Database classes

// Customer Database
class CustomerDB{
    public:
        static Customer* dummy; // pointer to a dummy Customer object
        vector<Customer> list;  // List of Customer objects in the database

        void Populate(vector<string> customer_lines);
        Customer* Find(string userid);
        void Add(string name, string userid, string pass);
        void Add(string name, string userid, string pass, int record);
};

// Employee Database
class EmployeeDB{
    public:
        static Employee* dummy; // pointer to a dummy Employee object
        vector<Employee> list;  // List of Employee objects in the database

        void Populate(vector<string> employee_lines);
        Employee* Find(string userid);
        void Add(string name, string userid, string pass);
        void Add(string name, string userid, string pass, string verify_code, int record);
};

// Manager Database
class ManagerDB{
    public:
        static Manager* dummy;  // pointer to a dummy Manager object
        vector<Manager> list;   // List of Manager objects in the database

        void Populate(vector<string> manager_lines);
        Manager* Find(string userid);
        void Add(string name, string userid, string pass);
        void Add(string name, string userid, string pass, string verify_code, int record);
};
