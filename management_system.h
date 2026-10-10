#ifndef URBAN_MANAGEMENT_SYSTEM_H
#define URBAN_MANAGEMENT_SYSTEM_H


#define MAX_PROVIDED_SERVICES 5
#define NAME_SIZE 50

typedef enum {Pending, Assigned, Completed, Cancelled} req_status;
typedef enum {False, True} boolean;

typedef enum {Faliure, Success} status_code;

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} Date;

typedef struct Customer_type{
    int id;
    char* name;
    int pincode;
    int contactNo;
    struct Customer_type* next;
} Customer;

typedef struct Service_type{
    int id;
    char* name;
    int best_price;
    struct Service_type* next;
} Service;

typedef struct Professional_type{
    int id;
    char* name;
    int pincode;
    int contactNo;
    boolean available;
    int services_count;
    Service* service_provided[MAX_PROVIDED_SERVICES];
    struct Professional_type* next;
} Professional;

typedef struct ServiceRequest_type{
    int id;
    Customer* customer;
    Service* service;
    Professional* assigned_professional;
    Date date;
    req_status status;
    struct ServiceRequest_type* next;
} ServiceRequest;

typedef struct app_type {
    int next_cust_id;
    int next_pro_id;
    int next_service_id;
    int next_req_id;

    Customer* customer_list;
    Professional* professional_list;
    Service* service_list;

    // Service request lists
    ServiceRequest* request_list;
} app;

void initialize_app(app*);


// searching functions
Customer* searchCustomerByDetails(app*, const char*, int);
// creating customer
Customer* createCustomer(app*, const char*, int, int);
status_code Register_Customer(app*);
// creating professional
void addServices(app*, Professional*);
Professional* createProfessional(app*, const char*, int, int);
status_code Register_Professional(app*);
// adding new services
Service* createService(app*, const char*, int);
status_code Add_Service(app*);
// creating service request
Professional* searchProForService(app*, int, int);
Customer* searchCustomerById(app*, int);
ServiceRequest* createServiceRequestHelper(app*, Customer*, Service*);
status_code createServiceRequest(app*);

// service functions
void completeServiceRequest(app*, int);
void cancelServiceRequest(app*, int);

// searching functions
void displayProProvidingService(app*, int);
void displayServicesByPro(app*, int);
void displayCustomerAllRequests(app*, int);

void displayPendingRequests(app*);
void displayServicesCompletedByPro(app*, int);

// sorting functions
void displayServicesByDecrServiceRequests(app*);
void displayProfessionalsByDecrServiceCompleted(app*);

#endif