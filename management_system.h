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
    long long contactNo;
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
    long long contactNo;
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
    Customer* customer_list;
    Professional* professional_list;
    Service* service_list;
    // Service request lists
    ServiceRequest* request_list;
} app;

// initialization
void initialize_app(app*);

// size functions
int getCustomerListSize(app*);
int getProfessionalListSize(app*);
int getServiceListSize(app*);
int getServiceRequestListSize(app*);

// searching functions
Customer* searchCustomerByDetails(app*, const char*, int);
Customer* searchCustomerById(app*, int);
Professional* searchProfessionalByDetails(app* , const char* , int);
Professional* searchProfessionalById(app*, int);
Service* searchInProById(Professional*, int);
Service* searchServiceByDetails(app*, const char*);

// creating customer
Customer* createCustomer(app*, const char*, int, long long);
status_code Register_Customer(app*);
// creating professional
void addServices(app*, Professional*);
Professional* createProfessional(app*, const char*, int, long long);
status_code Register_Professional(app*);
// adding new services
Service* createService(app*, const char*, int);
status_code Add_Service(app*);
// creating service request
void assigneDateTo(Date*);
Professional* searchProForService(app*, int, int);
ServiceRequest* createServiceRequestHelper(app*, Customer*, Service*);
status_code createServiceRequest(app*);

// service request functions
void assignProToServiceRequest(app*, Professional*);
void completeServiceRequest(app*, int);
void cancelServiceRequest(app*, int);

// details displaying functions
void displayProProvidingService(app*, int);
void displayServicesByPro(app*, int);
void displayCustomerAllRequests(app*, int);

// sorting functions
void mergeSortForServReqCount(void**, int*, int, int);
void mergeForServReqCount(void**, int*, int, int, int);

void displayServicesByDecrServiceRequests(app*);
void displayProfessionalsByDecrServiceCompleted(app*);

// service request displaying functions
void displayPendingRequests(app*);
void displayServicesCompletedByPro(app*, int);

#endif