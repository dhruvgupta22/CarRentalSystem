#include <bits/stdc++.h>
#include <fstream>
#include "../include/car.hpp"
#include "../include/user.hpp"
#include <ctime>
#include <chrono>

using namespace std;

// Static variables of Classes. Used as an default objects.
Customer* CustomerDB::dummy = new Customer();
Employee* EmployeeDB::dummy = new Employee();
Manager* ManagerDB::dummy = new Manager();
Car* CarDB::dummy = new Car();

// Auxiliary functions used (defined in functions.cpp) 
vector<string> Read_From_File(const string &filename);
void Write_CarDB_To_File(CarDB &car_db, const string& filename);
void Write_CustomerDB_To_File(CustomerDB &customer_db, const string& filename);
void Write_EmployeeDB_To_File(EmployeeDB &employee_db, const string& filename);
void Write_ManagerDB_To_File(ManagerDB &manager_db, const string& filename);
vector<string> createCarList(string s);
void Display1();
void Display_Customer_Func();
void Display_Worker_Home();
void Display_Personal_Acc();
void Display_Emp_Work_Acc();
void Display_Mgr_Work_Acc();
void Display_CarDB_func();
void Display_CustomerDB_func();
void Display_EmployeeDB_func();
void Wait_For_Enter();
bool isInteger(string &s);

int main(){
    
    // Creating objects of databases
    CarDB car_db;
    CustomerDB customer_db;
    EmployeeDB employee_db;
    ManagerDB manager_db;

    cout<<"Fetching data from the database...\n";
    
    // Read data from the text files
    vector<string> car_lines = Read_From_File("../database/car.txt");
    vector<string> customer_lines = Read_From_File("../database/customer.txt");
    vector<string> employee_lines = Read_From_File("../database/employee.txt");
    vector<string> manager_lines = Read_From_File("../database/manager.txt");

    // Populate the Database objects from the text files
    car_db.Populate(car_lines);
    customer_db.Populate(customer_lines);
    employee_db.Populate(employee_lines);
    manager_db.Populate(manager_lines);
    
    cout<<"Data fetched successfully!\n\n";

    // Welcome message. Program starts.
    cout<<"Welcome to Car Rental Service !\n";
    cout<<"\nPress Enter to continue...";
    cin.get();
    
    bool end_prog = false; // program ends when end_prog becomes true.
    while(!end_prog){
        bool logged_in = false; // user is logged out when logged_in is false, and logged in when it is true.
        cout<<endl;

        // Ask the user whether he/she wants to register or login
        Display1();
        int mode = 0; // mode : 1 -> register, 2-> login, 3-> exit
        do{
            cin>>mode;
            if(mode != 1 && mode != 2 && mode != 3){
                cout<<"Please enter valid input : 1 for Register, 2 for Login, 3 for exit\n";
            }
        }while(mode != 1 && mode != 2 && mode != 3);

        string user_name, user_id, user_pass, conf_pass, verify_code; // Variables used for taking inputs from the user
        int type = 0; // Type of the user - Customer, Employee, Manager

        switch(mode){
            case 1:
                // Register

                // Selecting User Type
                cout<<"Select the type of account you want to create: \n[1] Customer\n[2] Employee\n[3] Manager\n";
                do{
                    cin>>type;
                    if(type != 1 && type != 2 && type != 3){
                        cout<<"Please enter valid input : 1 for Customer, 2 for Employee, 3 for Manager\n";
                    }
                }while(type != 1 && type != 2 && type != 3);

                // Enter Details
                cout<<"Enter Your Name : ";
                cin.ignore();
                getline(cin,user_name);
                cout<<"Enter User ID : ";
                cin>>user_id;
                while(customer_db.Find(user_id) != customer_db.dummy || employee_db.Find(user_id) != employee_db.dummy || manager_db.Find(user_id) != manager_db.dummy){
                    cerr<<"This User ID is already taken."<<endl;
                    Wait_For_Enter();
                    cout<<"Enter User ID : ";
                    cin>>user_id;
                }

                // Create Password
                do{
                    cout<<"Create a password : ";
                    cin>>user_pass;
                    cout<<"Confirm password : ";
                    cin>>conf_pass;
                    if(user_pass != conf_pass){
                        cout<<"Passwords do not match ! Please try again.\n";
                        Wait_For_Enter();
                    }
                }while(user_pass != conf_pass);
                
                // Based on type, add user to respective database
                switch(type){
                    case 1:
                        // Customer Registration
                        customer_db.Add(user_name,user_id,user_pass);
                        cout<<"You were successfully registered !\n";
                        Wait_For_Enter();
                    break;

                    case 2:
                        // Employee Registration
                        employee_db.Add(user_name,user_id,user_pass);
                        cout<<"You were successfully registered !\n";
                        Wait_For_Enter();
                    break;

                    case 3:
                        // Manager Registration
                        manager_db.Add(user_name,user_id,user_pass);
                        cout<<"You were successfully registered !\n";
                        Wait_For_Enter();
                    break;
                }
            break;

            case 2:
                // Login

                // Select type of user
                cout<<"Select the type of account you want to login with :\n[1] Customer\n[2] Employee\n[3] Manager\n";
                do{
                    cin>>type;
                    if(type != 1 && type != 2 && type != 3){
                        cout<<"Please enter valid input : 1 for Customer, 2 for Employee, 3 for Manager\n";
                    }
                }while(type != 1 && type != 2 && type != 3);
                
                // Based on type login the user
                // All users must have unique User ID.
                switch(type){
                    case 1:
                        //Customer Login : Customer only needs User ID and password to login.
                        do{
                            cout<<"Enter User ID : ";
                            cin>>user_id;
                            cout<<"Enter password : ";
                            cin>>user_pass;
                            Customer* customer = customer_db.Find(user_id);
                            if(customer == customer_db.dummy){
                                cerr<<"User not found !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }
                            if(customer->password != user_pass){
                                cerr<<"Incorrect Password !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }

                        }while(customer_db.Find(user_id) == customer_db.dummy || customer_db.Find(user_id)->password != user_pass);
                        logged_in = true; // If all data is correct, log in
                    break;

                    case 2:
                        // Employee Login : Employee needs User ID, password, employee verification code (provided by the company) to login
                        // For simplicity Verification code of all employees in database is kept "EMP001" (w/o quotes).
                        do{
                            cout<<"Enter User ID : ";
                            cin>>user_id;
                            cout<<"Enter password : ";
                            cin>>user_pass;
                            cout<<"Enter employee verification code : ";
                            cin>>verify_code;
                            Employee* employee = employee_db.Find(user_id);
                            if(employee == employee_db.dummy){
                                cerr<<"User not found !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }
                            if(employee->password != user_pass){
                                cerr<<"Incorrect Password !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }
                            if(employee->verification_code != verify_code){
                                cerr<<"Incorrect Verification Code !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }

                        }while(employee_db.Find(user_id) == employee_db.dummy || employee_db.Find(user_id)->password != user_pass  || employee_db.Find(user_id)->verification_code != verify_code);
                        logged_in = true; // If all data is correct, log in
                    break;

                    case 3:
                        // Manager Login : Manager needs User ID, password, Manager verification code (provided by the company) to login
                        // For simplicity Verification code of all managers in database is kept "MGR001" (w/o quotes).
                        do{
                            cout<<"Enter User ID : ";
                            cin>>user_id;
                            cout<<"Enter password : ";
                            cin>>user_pass;
                            cout<<"Enter manager verification code : ";
                            cin>>verify_code;
                            Employee* manager = manager_db.Find(user_id);
                            if(manager == manager_db.dummy){
                                cerr<<"User not found !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }
                            if(manager->password != user_pass){
                                cerr<<"Incorrect Password !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }
                            if(manager->verification_code != verify_code){
                                cerr<<"Incorrect Verification Code !"<<endl;
                                Wait_For_Enter();
                                continue;
                            }

                        }while(manager_db.Find(user_id) == manager_db.dummy || manager_db.Find(user_id)->password != user_pass  || manager_db.Find(user_id)->verification_code != verify_code);
                        logged_in = true; // If all data is correct, log in
                    break;
                }
            break;

            case 3:
                // End the program if mode = 3
                end_prog = 1;
                continue;
        }
        // Creating pointers to objects of different users. Used for referencing objects later
        Customer* C;
        Employee* E;
        Manager* M;
        int func = 0; // Denotes the function chosen by the user
        string aux; // temporary variable later used to check invalid input from user
        int acc_type = 0; // Denotes the type of account (personal/work) in case of employees and managers
        int access_type = 0; // Denotes the type of database (car/customer/employee) accessed by the manager 

        while(logged_in){
            switch(type){
                case 1:
                    // Customer functions
                    C = customer_db.Find(user_id);
                    cout<<endl;
                    cout<<"Hello "<<C->Name<<" (^_^)"<<endl;
                    Display_Customer_Func();
                    cin>>aux;
                    func = isInteger(aux) ? stoi(aux) : -1;
                    switch(func){
                        case 1:
                            // View Available Cars
                            car_db.Show_Available_Cars();
                        break;

                        case 2:
                            // Rent a new car
                            C->Rent_Car(car_db, type);
                        break;

                        case 3:
                            // View already rented cars and dues
                            /* NOTE : Since we are not implementing any payment method, it is assumed for simplicity that the customer pays the full rent at the time of renting. Hence no dues are kept. */
                            C->View_Rented_Cars_and_Dues(car_db, type);
                        break;

                        case 4:
                            // Change password
                            C->Change_Password();
                        break;

                        case -1:
                            // Log out
                            logged_in = false;
                        break;
                    }
                    Wait_For_Enter();
                break;

                case 2:
                    // Employee Functions
                    E = employee_db.Find(user_id);
                    cout<<endl;
                    cout<<"Hello "<<E->Name<<" (^_^)"<<endl;
                    cout<<"Which account do you want to access?\n";

                    Display_Worker_Home(); // Display home page
                    cin>>aux;
                    acc_type = isInteger(aux) ? stoi(aux) : -1;
                    // Any input other than 1 or 2 will become -1, leading the user to logout.
                    switch(acc_type){
                        case 1:
                        while(acc_type == 1){
                            cout<<"What do you want to do?\n";

                            Display_Personal_Acc(); // Display personal account funtions
                            cin>>aux;
                            func = isInteger(aux) ? stoi(aux) : -1;
                            // Any input other than 1,2,3,4 will become -1 and take back the user to home page.

                            // All the personal account functions of the employee is same as the Customer functions
                            switch(func){
                                case 1:
                                    car_db.Show_Available_Cars();
                                break;

                                case 2:
                                    E->Rent_Car(car_db,type);
                                break;

                                case 3:
                                    E->View_Rented_Cars_and_Dues(car_db,type);
                                break;

                                case 4:
                                    E->Change_Password();
                                break;

                                case -1:
                                    acc_type = 0;
                                break;
                            }
                            Wait_For_Enter();
                        }
                        break;

                        case 2:
                        while(acc_type == 2){
                            cout<<"What do you want to do?\n";
                            Display_Emp_Work_Acc();
                            cin>>aux;
                            func = isInteger(aux) ? stoi(aux) : -1;
                            
                            // Employee Work Account functions
                            switch(func){
                                case 1:
                                    /* When a customer wants to return the car, employee has to use this option to take the car back after evaluating the condition of the car. */
                                    E->Return_Customer_Car(customer_db,car_db);
                                break;

                                case 2:
                                    /*Employee (unlike customer) can return his own car using this option. Although the amount he has to pay will be added as dues in his account, which has to be cleared by the Manager after payment.*/
                                    E->Return_Own_Car(car_db);
                                break;

                                case -1:
                                    // Go back to Home Page
                                    acc_type = 0;
                                break;
                            }
                            Wait_For_Enter();
                        }
                        break;

                        case -1:
                            //Logout
                            logged_in = false;
                        break;
                    }

                break;
                case 3:
                    // Manager Functions
                    M = manager_db.Find(user_id);
                    cout<<endl;
                    cout<<"Hello "<<M->Name<<" (^_^)"<<endl;
                    cout<<"Which account do you want to access?\n";

                    Display_Worker_Home(); // Display home page
                    cin>>aux;
                    acc_type = isInteger(aux) ? stoi(aux) : -1;

                    switch(acc_type){
                        case 1:
                        while(acc_type == 1){
                            cout<<"What do you want to do?\n";

                            Display_Personal_Acc(); // Display personal account funtions
                            cin>>aux;
                            func = isInteger(aux) ? stoi(aux) : -1;

                            // All the personal account functions of the manager is same as the Customer functions
                            switch(func){
                                case 1:
                                    car_db.Show_Available_Cars();
                                break;

                                case 2:
                                    M->Rent_Car(car_db, type);
                                break;

                                case 3:
                                    M->View_Rented_Cars_and_Dues(car_db, type);
                                break;

                                case 4:
                                    M->Change_Password();
                                break;

                                case -1:
                                    acc_type = 0;
                                break;
                            }
                            Wait_For_Enter();
                        }
                        break;

                        case 2:
                        while(acc_type == 2){
                            cout<<"What do you want to do?\n";

                            Display_Mgr_Work_Acc();
                            cin>>aux;
                            func = isInteger(aux) ? stoi(aux) : -1;

                            // Manager Work Account Functions
                            /* Manager can also return his own and customer's rented cars like employee.
                             He can also clear employees dues. and access all databases.
                             P.S. We have left out Accessing Manager Database to the company owners. This cannot be done by our software. */
                            switch(func){
                                case 1:
                                    M->Return_Customer_Car(customer_db,car_db);
                                break;

                                case 2:
                                    M->Return_Own_Car(car_db);
                                break;

                                case 3:
                                    M->Clear_Employee_Dues(employee_db);
                                break;

                                // Cases 4-6 Accessing Car, Customer, Employee databases respectively.
                                // Accessing refers to four functions - View, Add, Delete, Edit
                                case 4:
                                while(func == 4){
                                    Display_CarDB_func();
                                    cin>>aux;
                                    access_type = isInteger(aux) ? stoi(aux) : -1;
                                    switch(access_type){
                                        case 1:
                                            M->View_All_Cars(car_db);
                                        break;

                                        case 2:
                                            M->Add_Car(car_db);
                                        break;

                                        case 3:
                                            M->Delete_Car(car_db);
                                        break;

                                        case 4:
                                            M->Edit_Car_Details(car_db);
                                        break;

                                        case -1:
                                            func = 0;
                                        break;
                                    }
                                    if(access_type != -1) Wait_For_Enter();
                                }
                                break;

                                case 5:
                                while(func == 5){
                                    Display_CustomerDB_func();
                                    cin>>aux;
                                    access_type = isInteger(aux) ? stoi(aux) : -1;
                                    switch(access_type){
                                        case 1:
                                            M->View_All_Customers(customer_db);
                                        break;

                                        case 2:
                                            M->Add_Customer(customer_db);
                                        break;

                                        case 3:
                                            M->Delete_Customer_Acc(customer_db,car_db);
                                        break;

                                        case 4:
                                            M->Edit_Customer_Details(customer_db, car_db);
                                        break;

                                        case -1:
                                            func = 0;
                                        break;
                                    }
                                    if(access_type != -1) Wait_For_Enter();
                                }
                                break;

                                case 6:
                                while(func == 6){
                                    Display_EmployeeDB_func();
                                    cin>>aux;
                                    access_type = isInteger(aux) ? stoi(aux) : -1;
                                    switch(access_type){
                                        case 1:
                                            M->View_All_Employees(employee_db);
                                        break;

                                        case 2:
                                            M->Add_Employee(employee_db);
                                        break;

                                        case 3:
                                            M->Delete_Employee_Acc(employee_db, car_db);
                                        break;

                                        case 4:
                                            M->Edit_Employee_Details(employee_db, car_db);
                                        break;

                                        case -1:
                                            func = 0;
                                        break;
                                    }
                                    if(access_type != -1) Wait_For_Enter();
                                }
                                break;

                                case -1:
                                    // Go Back to Home Page
                                    acc_type = 0;
                                break;
                            }
                            Wait_For_Enter();
                        }
                        break;

                        case -1:
                            //Logout
                            logged_in = false;
                        break;
                    }
            }
            
        }
        // Save : Each time a user logs out, all the changes made to the database are stored permanently in text files.
        Write_CarDB_To_File(car_db, "../database/car.txt");
        Write_CustomerDB_To_File(customer_db, "../database/customer.txt");
        Write_EmployeeDB_To_File(employee_db, "../database/employee.txt");
        Write_ManagerDB_To_File(manager_db, "../database/manager.txt");
    }
    return 0;
}