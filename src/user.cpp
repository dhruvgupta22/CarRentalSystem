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
int Find(vector<string> v, string s);
vector<string> createCarList(string s);

// Customer Functions

// Customer Constructor
Customer::Customer(string name, string userid, string pass, vector<string> rcl, int dues, int rec){
    Name = name;
    UserID = userid;
    pass = password;
    RentedCarsList = rcl;
    Dues = dues;
    record = rec;
}

// Rent a car to the user based on his renting limit.
// Renting limit is decided based on the customer record.
void Customer::Rent_Car(CarDB &car_db, int type){
    if(RentedCarsList.size() >= renting_limit()){
        cout<<"Sorry, you have reached your renting limit.\n";
        return;
    }
    else if(car_db.available_car_count() == 0){
        cout<<"Sorry, no car is available for renting.\n";
        return;
    }
    cout<<"Enter the Vehicle Number of the car you want to rent : ";
    string vehicle_num;
    cin>>vehicle_num;
    Car* car = car_db.Find(vehicle_num);
    while(car == car_db.dummy || !car->Available){
        if(car == car_db.dummy){
            cerr<<"This Vehicle Number does not exist !\n";
        }
        else if(!car->Available){
            cerr<<"Sorry, this car is not available at the moment.\n";
        }
        cout<<"Enter the Vehicle Number of the car you want to rent : ";
        string vehicle_num;
        cin>>vehicle_num;
        car = car_db.Find(vehicle_num);
    }
    cout<<"Enter the number of days you want to rent for : ";
    int num_days = 0;
    cin>>num_days;
    while(num_days < 0 || num_days > MAX_RENT_LIMIT){
        if(num_days > MAX_RENT_LIMIT){
            cout<<"Sorry, but you can rent only for maximum "<<MAX_RENT_LIMIT<<" days. Please choose a number between 1 and "<<MAX_RENT_LIMIT<<"\n";
        }
        else if(num_days < 0){
            cout<<"Please enter a number between 1 and "<<MAX_RENT_LIMIT<<"\n";
        }
        cin>>num_days;
    }
    car->Available = false;
    car->rented_to = Name;
    car->rented_on = Get_Current_Date_Time();
    car->return_date = Calculate_Return_Date(car->rented_on, num_days);

    this->RentedCarsList.push_back(car->Vehicle_Number);
    cout<<"Car renting was successful! You have rented \'"<<car->Model<<"\' with license plate \'"<<car->Vehicle_Number<<"\' on "<<car->rented_on<<".\n"
        <<"You have to return it by "<<car->return_date<<". Returning after the due date and time will result in an additional fine of $"<<FINE_PER_DAY<<" per day.\nEnjoy your ride!\n";
    
    // Rent is added as dues if user is employee/manager.
    if(type == 2 || type == 3){
        int car_rent = car->Calculate_Rent(car->rented_on, car->return_date, type);
        Dues += car_rent;
        cout<<"$"<<car_rent<<" rent was added to your account.\n";
    }
}

// Displays the list of all the cars rented by the users along with their details and the amount due for each car.
/* NOTE : Since we are assuming that rent is already payed by the customer while renting. only due fine is shown.
          For employee/manager, the amount shown includes rent. */
void Customer::View_Rented_Cars_and_Dues(CarDB &car_db, int type){
    if(RentedCarsList.size() == 0){
        cout<<"You have not rented any car.\n";
        return;
    }
    int total_rent = 0;
    int total_fine = 0;
    int total_dues = 0;
    string curr_date_time = Get_Current_Date_Time();
    cout<<endl;
    cout<<"          Model          | Vehicle Number |     Rented On     |     Return Date     | Price (per day) | Due Fine \n";
    cout<<"=================================================================================================================\n";
    for(auto vehicle_num : RentedCarsList){
        Car* car = car_db.Find(vehicle_num);
        int car_fine = car->Calculate_Fine(curr_date_time, car->return_date);
        total_fine += car_fine;
        cout<<setfill(' ')<<setw(25)<<left<<car->Model<<"     "<<setfill(' ')<<setw(10)<<left<<car->Vehicle_Number<<"     "<<setfill(' ')<<setw(20)<<left<<car->rented_on<<" "<<setfill(' ')<<setw(20)<<left<<car->return_date<<"\t    "<<setfill(' ')<<setw(5)<<left<<car->price<<"\t   "<<setfill(' ')<<setw(5)<<left<<car_fine<<endl;
    }
    cout<<endl;
    if(type == 1){
        cout<<"Total Due Fine : "<<total_fine<<endl;
    }
    if(type == 2 || type == 3){
        cout<<"Total Due Amount : "<<Dues<<endl;
    }
}

// Change password
void Customer::Change_Password(){
    string curr_pass, new_pass, conf_pass;
    do{
        cout<<"Enter Current Password : ";
        cin>>curr_pass;
        if(curr_pass != password){
            cerr<<"Incorrect Password. Please try again.\n";
        }
    }while(curr_pass != password);
    do{
        cout<<"Enter New Password : ";
        cin>>new_pass;
        cout<<"Confirm New Password : ";
        cin>>conf_pass;
        if(new_pass != conf_pass){
            cerr<<"Passwords do not match. Please try again.\n";
        }
    }while(new_pass != conf_pass);
    password = new_pass;
}

// Calculates total dues for a customer for all the rented cars, based on his type.
int Customer::Calculate_Dues(CarDB car_db, int type){
    int total_rent = 0;
    int total_fine = 0;
    int total_dues = 0;
    string curr_date_time = Get_Current_Date_Time();
    for(auto vehicle_num : RentedCarsList){
        Car* car = car_db.Find(vehicle_num);
        int car_rent = car->Calculate_Rent(car->return_date, car->rented_on, type);
        int car_fine = car->Calculate_Fine(curr_date_time, car->return_date);
        int car_dues = car_rent + car_fine;
        total_rent += car_rent;
        total_fine += car_fine;
        total_dues += car_dues;
    }
    return total_dues;
}

// Returns the renting limit of the user based on his record
int Customer::renting_limit(){
    int rl = 0;
    if(rl < 0) rl = 0;
    else{
        rl = ceil((float)record/3);
    }
    return rl;
}
//-----------------------------------------------------------------------------------------------------------------//

// Employee Functions


// Employee Constructor
Employee::Employee(string name, string userid, string pass, vector<string> rcl, int dues, int rec, string vc){
    Name = name;
    UserID = userid;
    pass = password;
    RentedCarsList = rcl;
    Dues = dues;
    record = rec;
    verification_code = vc;
}

// Used by employee to Return Customer's rented car, after assessing its condition.
// Also updates the customer record based on the relative condition of the car.
void Employee::Return_Customer_Car(CustomerDB &customer_db, CarDB &car_db){
    cout<<"Enter the User ID of the customer : ";
    string userid;
    cin>>userid;
    Customer* customer = customer_db.Find(userid);
    while(customer == customer_db.dummy){
        cerr<<"User not found! Please enter a valid User ID.\n";
        cout<<"Enter the User ID of the customer : ";
        cin>>userid;
        customer = customer_db.Find(userid);
    }
    if(customer->RentedCarsList.size() == 0){
        cerr<<"The customer has not rented any car.\n";
        return;
    }
    cout<<"Enter the Vehicle Number of the car : ";
    string vehicle_num;
    cin>>vehicle_num;
    Car* car = car_db.Find(vehicle_num);
    int idx = Find(customer->RentedCarsList, vehicle_num);
    while(car == car_db.dummy || idx == -1){
        if(car == car_db.dummy){
            cerr<<"Car not found! Please enter a valid vehicle number.\n";
        }
        else if(idx == -1){
            cerr<<"You have not rented this car! Please enter a valid vehicle number.\n";
        }
        cout<<"Enter the Vehicle Number of the car : ";
        cin>>vehicle_num;
        car = car_db.Find(vehicle_num);
        idx = Find(customer->RentedCarsList, vehicle_num);
    }
    string curr_date_time = Get_Current_Date_Time();
    int fine = car->Calculate_Fine(curr_date_time, car->return_date);
    cout<<"The Car was found - \n"
        <<"Model : "<<car->Model<<endl
        <<"Vehicle Number : "<<car->Vehicle_Number<<endl
        <<"Rented on : "<<car->rented_on<<endl
        <<"Returned on : "<<car->return_date<<endl
        <<"Fine to be payed : "<<fine<<endl
        <<"\n";
    
    int evaluation = car->Evaluate_Condition_and_Update_Details(); // evaluation stores the updation value for customer record

    customer->record += evaluation;

    auto it = customer->RentedCarsList.begin();
    customer->RentedCarsList.erase(it + idx);

    cout<<"Car \'"<<car->Vehicle_Number<<"\'"<<"was successfully returned on "<<Get_Current_Date_Time()<<".\n";
}

// Used by employee to return his rented car.
// Also adds the fine to be payed to his account.
void Employee::Return_Own_Car(CarDB &car_db){
    cout<<"Enter employee verification code : ";
    string verify_code;
    cin>>verify_code;
    while(verify_code != verification_code){
        cout<<"Incorrect Verification Code! Please try again.\n";
        cout<<"Enter employee verification code : ";
        cin>>verify_code;
    }
    if(RentedCarsList.size() == 0){
        cerr<<"You have not rented any car.\n";
        return;
    }
    cout<<"Enter the Vehicle Number of the car : ";
    string vehicle_num;
    cin>>vehicle_num;
    Car* car = car_db.Find(vehicle_num);
    int idx = Find(RentedCarsList, vehicle_num);
    while(car == car_db.dummy || idx == -1){
        if(car == car_db.dummy){
            cerr<<"Car not found! Please enter a valid vehicle number.\n";
        }
        else if(idx == -1){
            cerr<<"You have not rented this car! Please enter a valid vehicle number.\n";
        }
        cout<<"Enter the Vehicle Number of the car : ";
        cin>>vehicle_num;
        car = car_db.Find(vehicle_num);
        idx = Find(RentedCarsList, vehicle_num);
    }
    string curr_date_time = Get_Current_Date_Time();
    int rent = car->Calculate_Rent(curr_date_time, car->rented_on, 1);
    int fine = car->Calculate_Fine(curr_date_time, car->return_date);
    cout<<"The Car was found - \n"
        <<"Model : "<<car->Model<<endl
        <<"Vehicle Number : "<<car->Vehicle_Number<<endl
        <<"Rented on : "<<car->rented_on<<endl
        <<"Returned on : "<<car->return_date<<endl
        <<"Rent to be payed : "<<rent<<endl
        <<"Fine to be payed : "<<fine<<endl
        <<"Total Amount to be payed : "<<(rent+fine)<<endl
        <<"\n";
    
    int evaluation = car->Evaluate_Condition_and_Update_Details();

    record += evaluation;

    auto it = RentedCarsList.begin();
    RentedCarsList.erase(it + idx);
    Dues += fine;

    cout<<"Car \'"<<car->Vehicle_Number<<"\'"<<"was successfully returned on "<<Get_Current_Date_Time()<<".\n$"<<fine<<" fine was added to your account.\n";
}
//---------------------------------------------------------------------------------------------------------------------------------//

// Manager Functions

// Manager Constructor 
Manager::Manager(string name, string userid, string pass, vector<string> rcl, int dues, int rec, string vc){
    Name = name;
    UserID = userid;
    pass = password;
    RentedCarsList = rcl;
    Dues = dues;
    record = rec;
    verification_code = vc;
}

// Used by manager to clear dues of an employee
void Manager::Clear_Employee_Dues(EmployeeDB &employee_db){
    string user_id;
    Employee* employee;
    do{
        cout<<"Enter the User ID of the employee you want to clear dues for : ";
        cin>>user_id;
        employee = employee_db.Find(user_id);
        if(employee == employee_db.dummy){
            cerr<<"Employee not found in the database. Please enter valid User ID.\n";
        }
    }while(employee == employee_db.dummy);

    cout<<"Do you want to clear all dues for "<<user_id<<" ? [Y/N] : ";
    char c;
    do{
        cin>>c;
        if(c != 'Y' && c != 'y' && c != 'N' && c != 'n'){
            cout<<"Please enter valid input : Y for Yes, N for No\n";
        }
    }while(c != 'Y' && c != 'y' && c != 'N' && c != 'n');

    if(c == 'Y' || c == 'y'){
        employee->Dues = 0;
        cout<<"All Dues of "<<user_id<<" were cleared.";
    }
}

// Displays the list of all the cars and their details
void Manager::View_All_Cars(CarDB &car_db){
    int index = 0;
    cout<<endl;
    for(auto car : car_db.list){
        if(index == 0){
            // string s[4] = {"Model","Vehicle Number","Condition","Price"}
            cout<<"          Model          | Vehicle Number |   Conditon   | Available |          Rented To          |     Rented On     |     Return Date     | Price (per day) "<<endl;
            cout<<"==============================================================================================================================================================="<<endl;
        }
        index++;
        string avai = car.Available ? "Yes" : "No";
        string ind = to_string(index);
        ind += ".";
        cout<<setfill(' ')<<setw(4)<<left<<ind<<setfill(' ')<<setw(25)<<left<<car.Model<<"  "<<left<<setfill(' ')<<setw(10)<<car.Vehicle_Number<<"     "<<left<<setfill(' ')<<setw(10)<<car.condition<<"     "<<left<<setfill(' ')<<setw(10)<<avai<<"  "<<left<<setfill(' ')<<setw(30)<<car.rented_to<<left<<setfill(' ')<<setw(20)<<car.rented_on<<left<<setfill(' ')<<setw(20)<<car.return_date<<"    "<<left<<setfill(' ')<<setw(5)<<car.price<<endl;
    }
    if(index == 0){
        cout<<"There are no cars in the Database.\n";
    }
    cout<<endl;
}

// Creates a new Car object and add it to the database
void Manager::Add_Car(CarDB &car_db){
    string model, vehicle_num, condition;
    int price;
    cout<<"Enter Car Details - \n"
        <<"Model : ";
    cin.ignore();
    getline(cin,model);
    do{
        cout<<"Vehicle Number : ";
        cin>>vehicle_num;
        if(car_db.Find(vehicle_num) != car_db.dummy){
            cerr<<"This Vehivle Number already exists. Please try again.\n";
        }
    }while(car_db.Find(vehicle_num) != car_db.dummy);
    
    cout<<"Condition : ";
    cin>>condition;
    cout<<"Price of Rent (per day) : ";
    cin>>price;

    car_db.Add(model, vehicle_num, condition, price);
}

// Deletes a Car object from the database
void Manager::Delete_Car(CarDB &car_db){
    cout<<"Enter the Vehicle Number of the car : ";
    string vehicle_num;
    cin>>vehicle_num;
    Car* car = car_db.Find(vehicle_num);
    while(car == car_db.dummy || !car->Available){
        if(car == car_db.dummy){
            cerr<<"Car not found! Please enter a valid vehicle number.\n";
            continue;
        }
        if(!car->Available){
            cerr<<"This car is on rent. Cannot delete this car at the moment.\n";
        }
        cout<<"Enter the Vehicle Number of the car : ";
        cin>>vehicle_num;
        car = car_db.Find(vehicle_num);
    }

    auto it = car_db.list.begin();
    for(int i=0; i<car_db.list.size(); i++){
        if(car_db.list[i].Vehicle_Number == car->Vehicle_Number){
            car_db.list.erase(it + i);
            break;
        }
    }
    cout<<"The car was successfully deleted from the database.\n";
}

// Edit the details of a car
void Manager::Edit_Car_Details(CarDB &car_db){
    string vehicle_num, condition, rented_to, rented_on, return_date;
    int price;
    Car* car;
    do{
        cout<<"Enter the Vehicle Number of the car you want to edit : ";
        cin>>vehicle_num;
        car = car_db.Find(vehicle_num);
        if(car == car_db.dummy){
            cerr<<"Car not found in the database. Please enter valid Vehicle Number.\n";
        }
        else if(!car->Available){
            cerr<<"Car is on rent at the moment. Cannot change details.\n";
        }
    }while(car == car_db.dummy || !car->Available);
    cout<<"Enter new details of the car - \n"
        <<"Condition (Fair/Good/Excellent): ";
    cin>>condition;
    while(condition != "Fair" && condition != "Good" && condition != "Excellent"){
        cout<<"Please enter valid input.\n";
        cout<<"Condition (Fair/Good/Excellent): ";
        cin>>condition;
    }
    cout<<"Price of Rent (per day) : ";
    cin>>price;

    car->condition = condition;
    car->price = price;
    cout<<"Car detials were successfully updated.\n";
}

// Displays the list of all the customers and their details
void Manager::View_All_Customers(CustomerDB &customer_db){
    int index = 0;
    cout<<endl;
    for(auto customer : customer_db.list){
        if(index == 0){
            // string s[4] = {"Model","Vehicle Number","Condition","Price"}
            cout<<"              Name             |     User ID     |        Cars Rented        | Record "<<endl;
            cout<<"========================================================================================"<<endl;
        }
        index++;
        string rent_cars = "";
        if(customer.RentedCarsList.size() == 0){
            rent_cars = "None";
        }
        else{
            for(int i=0; i<customer.RentedCarsList.size(); i++){
                rent_cars += customer.RentedCarsList[i];
                if(i < customer.RentedCarsList.size()-1){
                    rent_cars += ",";
                }
            }
        }
        string ind = to_string(index) + ".";
        cout<<left<<setfill(' ')<<setw(4)<<ind<<left<<setfill(' ')<<setw(30)<<customer.Name<<left<<setfill(' ')<<setw(15)<<customer.UserID<<"    "<<left<<setfill(' ')<<setw(25)<<rent_cars<<"    "<<left<<setfill(' ')<<setw(5)<<customer.record<<endl;
        
    }
    if(index == 0){
        cout<<"There are no customers in the Database.\n";
    }
    cout<<endl;
}

// Creates a new Customer object and add it to the database
void Manager::Add_Customer(CustomerDB &customer_db){
    string name, user_id, pass, conf_pass;
    int record;
    cout<<"Enter Customer Details - \n"
        <<"Name : ";
    cin.ignore();
    getline(cin, name);
    do{
        cout<<"User ID : ";
        cin>>user_id;
        if(customer_db.Find(user_id) != customer_db.dummy){
            cerr<<"This User ID is already taken. Please try again.\n";
        }
    }while(customer_db.Find(user_id) != customer_db.dummy);
    
    cout<<"Intial record : ";
    cin>>record;
    do{
        cout<<"Create Password : ";
        cin>>pass;
        cout<<"Confirm Password : ";
        cin>>conf_pass;
        if(pass != conf_pass){
            cout<<"Passwords do not match ! Please try again.\n";
        }
    }while(pass != conf_pass);
    customer_db.Add(name, user_id, pass, record);
}

// Delete a Customer object from the database
void Manager::Delete_Customer_Acc(CustomerDB &customer_db, CarDB &car_db){
    cout<<"Enter the User ID of the customer : ";
    string user_id;
    cin>>user_id;
    Customer* customer = customer_db.Find(user_id);
    while(customer == customer_db.dummy){
        cerr<<"Customer not found! Please enter a valid User ID.\n";
        cin>>user_id;
        customer = customer_db.Find(user_id);
    }
    vector<string> cars;
    auto it = customer_db.list.begin();
    for(int i=0; i<customer_db.list.size(); i++){
        if(customer_db.list[i].UserID == customer->UserID){
            cars = customer->RentedCarsList;
            customer_db.list.erase(it + i);
            break;
        }
    }
    car_db.makeAvailable(cars);
    cout<<"The customer was successfully deleted from the database. Cars rented by the customer were added to the databse.\n";
}

// Edit Customer Details
void Manager::Edit_Customer_Details(CustomerDB &customer_db, CarDB &car_db){
    string user_id, name, rent_cars;
    int record, dues;
    Customer* customer;
    do{
        cout<<"Enter the User ID of the customer you want to edit : ";
        cin>>user_id;
        customer = customer_db.Find(user_id);
        if(customer == customer_db.dummy){
            cerr<<"Customer not found in the database. Please enter valid User ID.\n";
        }
    }while(customer == customer_db.dummy);

    cout<<"Enter new details of the customer - \n"
        <<"Name: ";
    cin.ignore();
    getline(cin, name);
    cout<<"Record : ";
    cin>>record;
    cout<<"Enter Due : ";
    cin>>dues;
    customer->Name = name;
    customer->record = record;
    customer->Dues = dues;
}

// Displays the list of all the employees and their details
void Manager::View_All_Employees(EmployeeDB &employee_db){
    int index = 0;
    cout<<endl;
    for(auto employee : employee_db.list){
        if(index == 0){
            // string s[4] = {"Model","Vehicle Number","Condition","Price"}
            cout<<"              Name             |     User ID     |        Cars Rented        | Record |   Dues   "<<endl;
            cout<<"==================================================================================================="<<endl;
        }
        index++;
        string rent_cars = "";
        if(employee.RentedCarsList.size() == 0){
            rent_cars = "None";
        }
        else{
            for(int i=0; i<employee.RentedCarsList.size(); i++){
                rent_cars += employee.RentedCarsList[i];
                if(i < employee.RentedCarsList.size()-1){
                    rent_cars += ",";
                }
            }
        }
        string ind = to_string(index) + ".";
        cout<<left<<setfill(' ')<<setw(4)<<ind<<left<<setfill(' ')<<setw(30)<<employee.Name<<left<<setfill(' ')<<setw(15)<<employee.UserID<<"    "<<left<<setfill(' ')<<setw(25)<<rent_cars<<"    "<<left<<setfill(' ')<<setw(5)<<employee.record<<"  "<<left<<setfill(' ')<<setw(10)<<employee.Dues<<endl;
        
    }
    if(index == 0){
        cout<<"There are no employees in the Database.\n";
    }
    cout<<endl;
}

// Creates a new Employee object and add it to the database
void Manager::Add_Employee(EmployeeDB &employee_db){
    string name, user_id, pass, conf_pass, verify_code;
    int record;
    cout<<"Enter Employee Details - \n"
        <<"Name : ";
    cin.ignore();
    getline(cin, name);
    do{
        cout<<"User ID : ";
        cin>>user_id;
        if(employee_db.Find(user_id) != employee_db.dummy){
            cerr<<"This User ID is already taken. Please try again.\n";
        }
    }while(employee_db.Find(user_id) != employee_db.dummy);
    do{
        cout<<"Create Password : ";
        cin>>pass;
        cout<<"Confirm Password : ";
        cin>>conf_pass;
        if(pass != conf_pass){
            cout<<"Passwords do not match ! Please try again.\n";
        }
    }while(pass != conf_pass);
    cout<<"Employee Verification Code : ";
    cin>>verify_code;
    cout<<"Intial record : ";
    cin>>record;
    employee_db.Add(name, user_id, pass, verify_code, record);
}

// Deletes an Employee object from the database
void Manager::Delete_Employee_Acc(EmployeeDB &employee_db, CarDB &car_db){
    cout<<"Enter the User ID of the employee : ";
    string user_id;
    cin>>user_id;
    Customer* employee = employee_db.Find(user_id);
    while(employee == employee_db.dummy){
        cerr<<"Employee not found! Please enter a valid User ID.\n";
        cin>>user_id;
        employee = employee_db.Find(user_id);
    }
    vector<string> cars;
    auto it = employee_db.list.begin();
    for(int i=0; i<employee_db.list.size(); i++){
        if(employee_db.list[i].UserID == employee->UserID){
            employee_db.list.erase(it + i);
            cars = employee->RentedCarsList;
            break;
        }
    }
    car_db.makeAvailable(cars);
    cout<<"The employee was successfully deleted from the database. Cars rented by the employee were added to the databse.\n";
}

// Edit Employee details
void Manager::Edit_Employee_Details(EmployeeDB &employee_db, CarDB &car_db){
    string user_id, name, rent_cars;
    int record, dues;
    Employee* employee;
    do{
        cout<<"Enter the User ID of the employee you want to edit : ";
        cin>>user_id;
        employee = employee_db.Find(user_id);
        if(employee == employee_db.dummy){
            cerr<<"Employee not found in the database. Please enter valid User ID.\n";
        }
    }while(employee == employee_db.dummy);

    cout<<"Enter new details of the employee - \n"
        <<"Name: ";
    cin.ignore();
    getline(cin, name);
    cout<<"Record : ";
    cin>>record;
    cout<<"Enter Dues : ";
    cin>>dues;
    employee->Name = name;
    employee->record = record;
    employee->Dues = dues;
}

//---------------------------------------------------------------------------------------------------------------//

// Customer Database Functions

// Populates the CustomerDB object with the data read from the text database file
void CustomerDB::Populate(vector<string> customer_lines){
    for(int i = 0; i < customer_lines.size(); i += 7){
        Customer temp_customer;
    
        temp_customer.Name = customer_lines[i];
        temp_customer.UserID = customer_lines[i+1];
        temp_customer.password = customer_lines[i+2];
        temp_customer.RentedCarsList = createCarList(customer_lines[i+3]);
        temp_customer.Dues = stoi(customer_lines[i+4]);
        temp_customer.record = stoi(customer_lines[i+5]);
        
        list.push_back(temp_customer);
    }
}

// Finds a particular customer in the customer database using his unique User ID
Customer* CustomerDB::Find(string userid){
    for(auto &entry : this->list){
        if(entry.UserID == userid){
            Customer* temp = &entry;
            return temp;
        }
    }
    return dummy;
}

// Adds a Customer object to the database
void CustomerDB::Add(string name, string userid, string pass){
    Customer C;
    C.Name = name;
    C.UserID = userid;
    C.password = pass;
    C.RentedCarsList = {};
    C.Dues = 0;
    C.record = DEFAULT_RECORD;
    list.push_back(C);
}

// Adds a Customer object to the database
void CustomerDB::Add(string name, string userid, string pass, int record){
    Customer C;
    C.Name = name;
    C.UserID = userid;
    C.password = pass;
    C.RentedCarsList = {};
    C.Dues = 0;
    C.record = record;
    list.push_back(C);
}

// Populates the EmployeeDB object with the data read from the text database file
void EmployeeDB::Populate(vector<string> employee_lines){
    for(int i = 0; i < employee_lines.size(); i += 8){
        Employee temp_employee;

        temp_employee.Name = employee_lines[i];
        temp_employee.UserID = employee_lines[i+1];
        temp_employee.password = employee_lines[i+2];
        temp_employee.RentedCarsList = createCarList(employee_lines[i+3]);
        temp_employee.Dues = stoi(employee_lines[i+4]);
        temp_employee.record = stoi(employee_lines[i+5]);
        temp_employee.verification_code = employee_lines[i+6];
        
        list.push_back(temp_employee);
    }
}

// Finds a particular employee in the employee database using his unique User ID
Employee* EmployeeDB::Find(string userid){
    for(auto &entry : list){
        if(entry.UserID == userid){
            Employee* temp = &entry;
            return temp;
        }
    }
    return dummy;
}

// Adds an Employee object to the database
void EmployeeDB::Add(string name, string userid, string pass){
    Employee E;
    E.Name = name;
    E.UserID = userid;
    E.password = pass;
    E.RentedCarsList = {};
    E.Dues = 0;
    E.record = 8;
    E.verification_code = "EMP001";
    list.push_back(E);
}

// Adds an Employee object to the database
void EmployeeDB::Add(string name, string userid, string pass, string verify_code, int record){
    Employee E;
    E.Name = name;
    E.UserID = userid;
    E.password = pass;
    E.RentedCarsList = {};
    E.Dues = 0;
    E.record = record;
    E.verification_code = verify_code;
    list.push_back(E);
}

// Populates the ManagerDB object with the data read from the text database file
void ManagerDB::Populate(vector<string> manager_lines){
    for(int i = 0; i < manager_lines.size(); i += 8){
        Manager temp_manager;

        temp_manager.Name = manager_lines[i];
        temp_manager.UserID = manager_lines[i+1];
        temp_manager.password = manager_lines[i+2];
        temp_manager.RentedCarsList = createCarList(manager_lines[i+3]);
        temp_manager.Dues = stoi(manager_lines[i+4]);
        temp_manager.record = stoi(manager_lines[i+5]);
        temp_manager.verification_code = manager_lines[i+6];
        
        list.push_back(temp_manager);
    }
}

// Finds a particular manager in the manager database using his unique User ID
Manager* ManagerDB::Find(string userid){
    for(auto &entry : list){
        if(entry.UserID == userid){
            Manager* temp = &entry;
            return temp;
        }
    }
    return dummy;
}

// Adds a Manager object to the database
void ManagerDB::Add(string name, string userid, string pass){
    Manager M;
    M.Name = name;
    M.UserID = userid;
    M.password = pass;
    M.RentedCarsList = {};
    M.Dues = 0;
    M.record = 8;
    M.verification_code = "MGR001";
    list.push_back(M);
}

// Adds a Manager object to the database
void ManagerDB::Add(string name, string userid, string pass, string verify_code, int record){
    Manager M;
    M.Name = name;
    M.UserID = userid;
    M.password = pass;
    M.RentedCarsList = {};
    M.Dues = 0;
    M.record = record;
    M.verification_code = verify_code;
    list.push_back(M);
}