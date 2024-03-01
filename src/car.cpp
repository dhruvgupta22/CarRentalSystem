#include <bits/stdc++.h>
#include "../include/car.hpp"
#include "../include/user.hpp"
#include "../include/macros.hpp"
#include <ctime>
#include <chrono>

using namespace std;

// Auxillary Functions used
string Get_Current_Date_Time();
string Calculate_Return_Date(string initial_date, int num_days);
int Days_Between_Dates(string dateStr1, string dateStr2);


// Car Functions

// Car Constructor
Car::Car(string model, string vehicle_num, string cond, bool avai, string r_to, string r_date, int _price){
    Model = model;
    Vehicle_Number = vehicle_num;
    condition = cond;
    Available = avai;
    rented_to = r_to;
    return_date = r_date;
    price = _price;
}

// Calculates rent for a rented car by calculating number of days between the two dates. 
// Type : type of user. Employee discount is applied for employees and managers only
int Car::Calculate_Rent(string curr_date_time, string issue_date_time, int type){
    int rent = 0;
    int days = Days_Between_Dates(curr_date_time, issue_date_time);
    rent = days * price;
    if(type == 2 || type == 3){
        rent = rent - (EMPLOYEE_DISCOUNT * rent /100);
    }
    return rent;
}

// Calculates fine for the rented car at the time of returning.
// No discount on fines
int Car::Calculate_Fine(string curr_date_time, string return_date_time){
    int fine = 0;
    int days = Days_Between_Dates(return_date_time, curr_date_time);
    if(days < 0){
        return 0;
    }
    else{
        fine = days * FINE_PER_DAY;
        return fine;
    }
}

// Used by Employee to evaluate condition of the car while returning and update the availability status of the car in the databse at the same time.
int Car::Evaluate_Condition_and_Update_Details(){
    cout<<"What is the condition of the Car?\n"
        <<"[1] Fair\n"
        <<"[2] Good\n"
        <<"[3] Excellent\n";
    int curr_cond = 0;
    cin>>curr_cond;
    int prev_cond = 0;
    if(condition == "Fair") prev_cond = 1;
    if(condition == "Good") prev_cond = 2;
    if(condition == "Excellent") prev_cond = 3;

    //Update
    if(curr_cond == 1) condition = "Fair";
    if(curr_cond == 2) condition = "Good";
    if(curr_cond == 3) condition = "Excellent";

    Available = true;
    rented_to = "***";
    rented_on = "dd-mm-yyyy hh:mm";
    return_date = "dd-mm-yyyy hh:mm";

    //Evaluate
    if(curr_cond < prev_cond) return -1;
    else if(curr_cond > prev_cond) return +1;
    else return 0;
}

//-------------------------------------------------------------------------------------------------------//

// Car Database Functions

// Populates the CarDB object with the data read from the text database file
void CarDB::Populate(vector<string> car_lines){
    for(int i = 0; i < car_lines.size(); i += 9){
            Car temp_car;

            temp_car.Model = car_lines[i];
            temp_car.Vehicle_Number = car_lines[i+1];
            temp_car.condition = car_lines[i+2];
            temp_car.Available = (car_lines[i+3] == "Yes") ? true : false;
            temp_car.rented_to = car_lines[i+4];
            temp_car.rented_on = car_lines[i+5];
            temp_car.return_date = car_lines[i+6];
            temp_car.price = stoi(car_lines[i+7]);
            
            list.push_back(temp_car);
        }
}

// Finds a particular car in the car database using its unique vehicle number
Car* CarDB::Find(string vehicle_num){
    for(auto &entry : list){
        if(entry.Vehicle_Number == vehicle_num){
            Car* temp = &entry;
            return temp;
        }
    }
    return dummy;
}

// Adds a new car to the database
void CarDB::Add(string model, string vehicle_num, string condition, int price){
    Car new_car;
    new_car.Model = model;
    new_car.Vehicle_Number = vehicle_num;
    new_car.condition = condition;
    new_car.Available = true;
    new_car.rented_on = "dd-mm-yyyy hh:mm";
    new_car.rented_to = "***";
    new_car.return_date = "dd-mm-yyyy hh:mm";
    new_car.price = price;

    list.push_back(new_car);
}

// Returns the number of cars available for renting at the moment
int CarDB::available_car_count(){
    int count = 0;
    for(auto car : list){
        if(car.Available){
            count++;
        }
    }
    return count;
}

// Displays the list of all available cars in tabular format
void CarDB::Show_Available_Cars(){
    int index = 0;
    for(auto car : list){
        if(car.Available){
            if(index == 0){
                cout<<"          Model          | Vehicle Number |   Conditon   | Price (per day) "<<endl;
                cout<<"==========================================================================================================================="<<endl;
            }
            index++;
            string ind = to_string(index);
            ind += ".";
            cout<<setfill(' ')<<setw(4)<<left<<ind<<setfill(' ')<<setw(25)<<left<<car.Model<<"  "<<left<<setfill(' ')<<setw(10)<<car.Vehicle_Number<<"     "<<left<<setfill(' ')<<setw(10)<<car.condition<<"     "<<left<<setfill(' ')<<setw(5)<<car.price<<endl;
        }
    }
    if(index == 0){
        cout<<"Sorry, no cars are available for renting.\n";
    }
}

// Updates the availability status of all the cars in the list to "Available"
void CarDB::makeAvailable(vector<string> cars){
    if(cars.size() == 0) return;
    Car* car;
    for(auto vehicle_num : cars){
        car = Find(vehicle_num);
        if(car == dummy) continue;
        car->Available = true;
        car->rented_to = "***";
        car->rented_on = "dd-mm-yyyy hh:mm";
        car->return_date = "dd-mm-yyyy hh:mm";
    }
}
