#include <bits/stdc++.h>
#include <ctime>
#include <chrono>
#include "../include/car.hpp"
#include "../include/user.hpp"
using namespace std;

// This file contains all the auxiliary functions which are not the members of any particular class.

// Reads data from the text file 'filename' and returns it as a vector of string
vector<string> Read_From_File(const string &filename){ 
    vector<string> lines;
    ifstream data(filename);
    // data.open(filename);
    if(!data.is_open()){
        cerr<<"File \'"<<filename<<"\' could not be read.\n";
    }
    string temp_line;
    while(data){
        getline(data,temp_line);
        lines.push_back(temp_line);
    }
    data.close();
    return lines;
}

// Writes data from the CarDB object to the car database text file
void Write_CarDB_To_File(CarDB &car_db, const string& filename) {
    // Open file stream for writing
    std::ofstream outFile(filename);

    if (outFile.is_open()){
        // Iterate through objects
        Car car;
        for(int i=0; i<car_db.list.size(); i++){
            // Write object data to file in desired format
            car = car_db.list[i];
            string avai = car.Available ? "Yes" : "No";
            outFile << car.Model << "\n" 
                    << car.Vehicle_Number << "\n" 
                    << car.condition << "\n"
                    << avai << "\n"
                    << car.rented_to << "\n"
                    << car.rented_on << "\n"
                    << car.return_date << "\n"
                    << car.price;
            if(i < car_db.list.size()-1) outFile << "\n\n";
        }

        // Close file stream
        outFile.close();
        cout << "Data has been written to " << filename << endl;
    }else{
        cerr << "Unable to open file : " << filename << endl;
    }
}

// Writes data from the CustomerDB object to the customer database text file
void Write_CustomerDB_To_File(CustomerDB &customer_db, const string& filename){
    // Open file stream for writing
    std::ofstream outFile(filename);

    if (outFile.is_open()){
        // Iterate through objects
        Customer customer;
        for(int i=0; i<customer_db.list.size(); i++){
            // Write object data to file in desired format
            customer = customer_db.list[i];
            string rent_cars = "";
            if(customer.RentedCarsList.size() == 0){
                rent_cars = "***";
            }
            else{
                for(auto u : customer.RentedCarsList){
                    rent_cars += u;
                    rent_cars += ",";
                }
                rent_cars.pop_back();
            }
            outFile << customer.Name << "\n" 
                    << customer.UserID << "\n" 
                    << customer.password << "\n"
                    << rent_cars << "\n"
                    << customer.Dues << "\n"
                    << customer.record;
            if(i < customer_db.list.size()-1) outFile << "\n\n";
        }

        // Close file stream
        outFile.close();
        cout << "Data has been written to " << filename << endl;
    }else{
        cerr << "Unable to open file : " << filename << endl;
    }
}

// Writes data from the EmployeeDB object to the employee database text file
void Write_EmployeeDB_To_File(EmployeeDB &employee_db, const string& filename){
    // Open file stream for writing
    std::ofstream outFile(filename);

    if (outFile.is_open()){
        // Iterate through objects
        Employee employee;
        for(int i=0; i<employee_db.list.size(); i++){
            // Write object data to file in desired format
            employee = employee_db.list[i];
            string rent_cars = "";
            if(employee.RentedCarsList.size() == 0){
                rent_cars = "***";
            }
            else{
                for(auto u : employee.RentedCarsList){
                    rent_cars += u;
                    rent_cars += ",";
                }
                rent_cars.pop_back();
            }
            outFile << employee.Name << "\n" 
                    << employee.UserID << "\n" 
                    << employee.password << "\n"
                    << rent_cars << "\n"
                    << employee.Dues << "\n"
                    << employee.record << "\n"
                    << employee.verification_code;
            if(i < employee_db.list.size()-1) outFile << "\n\n";
        }

        // Close file stream
        outFile.close();
        cout << "Data has been written to " << filename << endl;
    } else {
        cerr << "Unable to open file : " << filename << endl;
    }
}

// Writes data from the ManagerDB object to the manager database text file
void Write_ManagerDB_To_File(ManagerDB &manager_db, const string& filename){
    // Open file stream for writing
    std::ofstream outFile(filename);

    if (outFile.is_open()){
        // Iterate through objects
        Manager manager;
        for(int i=0; i<manager_db.list.size(); i++){
            // Write object data to file in desired format
            manager = manager_db.list[i];
            string rent_cars = "";
            if(manager.RentedCarsList.size() == 0){
                rent_cars = "***";
            }
            else{
                for(auto u : manager.RentedCarsList){
                    rent_cars += u;
                    rent_cars += ",";
                }
                rent_cars.pop_back();
            }
            outFile << manager.Name << "\n" 
                    << manager.UserID << "\n" 
                    << manager.password << "\n"
                    << rent_cars << "\n"
                    << manager.Dues << "\n"
                    << manager.record << "\n"
                    << manager.verification_code;
            if(i < manager_db.list.size()-1) outFile << "\n\n";
        }

        // Close file stream
        outFile.close();
        cout << "Data has been written to " << filename << endl;
    } else {
        cerr << "Unable to open file : " << filename << endl;
    }
}

// Takes a comma separated list of words as input and gives each word as a separate element in a vector.
//Used to Create RentedCarsList vector. 
vector<string> createCarList(string s){
    vector<string> list;
    stringstream f(s);
    while(f.good()){
        string str;
        getline(f,str,',');
        if(str != "***"){
            list.push_back(str);
        }
        else{
            list.clear();
            return list;
        }
    }
    return list;
}

// Returns the index of the element if found, else returns -1
int Find(vector<string> v, string s){
    for(int i=0; i<v.size(); i++){
        if(v[i] == s){
            return i; 
        }
    }
    return -1;
}
//-----------------------------------------------------------------------------------------------------------------//

/* Time maipulation functions */

// Outputs current system clock time in dd-mm-yy hh-mm format
string Get_Current_Date_Time(){
    chrono::system_clock::time_point now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* timeinfo = localtime(&currentTime);

    ostringstream oss;
    oss << setfill('0') << setw(2) << timeinfo->tm_mday << "-"
        << setfill('0') << setw(2) << (timeinfo->tm_mon + 1) << "-"
        << (timeinfo->tm_year + 1900) << " "
        << setfill('0') << setw(2) << timeinfo->tm_hour << ":"
        << setfill('0') << setw(2) << timeinfo->tm_min;

    return oss.str();
}

// Calculates and returns the date a car has to be returned by the user by adding initial date and the number of days user rents the car for 
string Calculate_Return_Date(string initial_date, int num_days){
    tm timeinfo = {};
    istringstream iss(initial_date);
    iss >> get_time(&timeinfo, "%d-%m-%Y %H:%M");

    time_t initial_time = mktime(&timeinfo);
    time_t final_time = initial_time + num_days * 24 * 60 * 60;
    tm* final_timeinfo = localtime(&final_time);

    ostringstream oss;
    oss << put_time(final_timeinfo, "%d-%m-%Y %H:%M");
    return oss.str();
}

// Calculates number of days between two dates. Used to calculate rent and fine.
int Days_Between_Dates(string dateStr1, string dateStr2){
    // Parse date string 1
    tm timeinfo1 = {};
    istringstream iss1(dateStr1);
    iss1 >> get_time(&timeinfo1, "%d-%m-%Y %H:%M");

    // Parse date string 2
    tm timeinfo2 = {};
    istringstream iss2(dateStr2);
    iss2 >> get_time(&timeinfo2, "%d-%m-%Y %H:%M");

    // Convert tm structs to time_t objects
    time_t time1 = mktime(&timeinfo1);
    time_t time2 = mktime(&timeinfo2);

    // Calculate difference in seconds
    time_t diffSeconds = difftime(time2, time1);

    // Convert seconds to days and round to next integer
    int days = (int)(ceil((double)(diffSeconds) / (60 * 60 * 24)));

    return days;
}
//------------------------------------------------------------------------------------------------------//

// Waits for user to press enter before continuing the progam
void Wait_For_Enter(){
    cout<<"\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// Checks if the user has given an integer input
bool isInteger(string &s){
    try {
        std::size_t pos;
        std::stoi(s, &pos);
        return pos == s.size(); // If pos equals s.size(), the entire string was converted
    } catch (...) {
        return false; // Conversion failed or exception occurred
    }
}
// ----------------------------------------------------------------------------------------------------- //


// Display Functions : These functions show the options available to the user at each step
// The numbers given in Box Brackets are valid inputs.

// Login/Sign in Display
void Display1(){
    // cout<<endl;
    cout<<"If you are a new user, please register for an account :\t[1] Register\n";
    cout<<"If you are already a registered user, please login :\t[2] Login \n";
    cout<<"If you want to end the program : \t\t\t[3] Exit\n";
}

// Customer Functions Display
void Display_Customer_Func(){
    cout<<"What do you want to do?\n";
    cout<<"[1] View Available Cars\n"
        <<"[2] Rent a Car\n"
        <<"[3] View Rented Car details and Dues\n"
        <<"[4] Change Password\n"
        <<"[X] Logout\n";
}

// Employee/Manager Home Page
void Display_Worker_Home(){
    cout<<"[1] Personal account\n"
        <<"[2] Work account\n"
        <<"[X] Logout\n";
}

// Employee/Manager personal account
void Display_Personal_Acc(){
    cout<<"[1] View Available Cars\n"
        <<"[2] Rent a Car\n"
        <<"[3] View Rented Car details and Dues\n"
        <<"[4] Change Password\n"
        <<"[X] Back to Home\n";
}

// Employee work account
void Display_Emp_Work_Acc(){
    cout<<"[1] Return Customer's Rented Car\n"
        <<"[2] Return Your Rented Car\n"
        <<"[X] Back to Home\n";
}

// Manager work account
void Display_Mgr_Work_Acc(){
    cout<<"[1] Return Customer's Rented Car\n"
        <<"[2] Return Your Rented Car\n"
        <<"[3] Clear Employee Dues\n"
        <<"[4] Access Car Database\n"
        <<"[5] Access Customer Database\n"
        <<"[6] Access Employee Database\n"
        <<"[X] Back to Home\n";
}

// Following displays are only seen by manager - 

// Accessing Car Database
void Display_CarDB_func(){
    cout<<"[1] View All Cars\n"
        <<"[2] Add Car\n"
        <<"[3] Delete Car\n"
        <<"[4] Edit Car Details\n"
        <<"[X] Back to Work Account\n";
        
}

// Accessing Customer Database
void Display_CustomerDB_func(){
    cout<<"[1] View All Customers\n"
        <<"[2] Add Customer\n"
        <<"[3] Delete Customer Account\n"
        <<"[4] Edit Customer Details\n"
        <<"[X] Back to Work Account\n";
}

// Accessing Employee Database
void Display_EmployeeDB_func(){
    cout<<"[1] View All Employees\n"
        <<"[2] Add Employee\n"
        <<"[3] Delete Employee Account\n"
        <<"[4] Edit Employee Details\n"
        <<"[X] Back to Work Account\n";
}
