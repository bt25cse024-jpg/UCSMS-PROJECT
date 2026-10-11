#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "management_system.h"

void initialize_app(app* a){
    a->customer_list = NULL;
    a->professional_list = NULL;
    a->service_list = NULL;
    a->request_list = NULL;
}

// size functions
int getCustomerListSize(app* a){
    int size = 0;
    if(a->customer_list != NULL){
        Customer* curr = a->customer_list->next;
        do{
            size++;
            curr = curr->next;
        } while(curr != a->customer_list->next);
    }
    return size;
}

int getProfessionalListSize(app* a){
    int size = 0;
    if(a->professional_list != NULL){
        Professional* curr = a->professional_list->next;
        do{
            size++;
            curr = curr->next;
        } while(curr != a->professional_list->next);
    }
    return size;
}

int getServiceListSize(app* a){
    int size = 0;
    if(a->service_list != NULL){
        Service* curr = a->service_list->next;
        do{
            size++;
            curr = curr->next;
        } while(curr != a->service_list->next);
    }
    return size;
}

int getServiceRequestListSize(app* a){
    int size = 0;
    if(a->request_list != NULL){
        ServiceRequest* curr = a->request_list->next;
        do{
            size++;
            curr = curr->next;
        } while(curr != a->request_list->next);
    }
    return size;
}

// searching functions

Customer* searchCustomerByDetails(app* a, const char* name, int pincode){
    int n = getCustomerListSize(a);
    Customer* found_cust = NULL;
    if(n != 0){
        Customer* curr = a->customer_list->next;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(strcmp(curr->name, name) == 0 && curr->pincode == pincode){
                found_cust = curr;
                found = 1;
            }
            curr = curr->next;
        }
    }
    
    return found_cust;
}

Customer* searchCustomerById(app* a, int customer_id){
    int n = getCustomerListSize(a);
    Customer* found_cust = NULL;
    if(n != 0){
        Customer* curr = a->customer_list->next;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(curr->id == customer_id){
                found_cust = curr;
                found = 1;
            }
            curr = curr->next;
        }
    }

    return found_cust;
}

Professional* searchProfessionalByDetails(app* a, const char* name, int pincode){
    int n = getProfessionalListSize(a);
    Professional* found_pro = NULL;
    if(n != 0){
        Professional* curr = a->professional_list->next;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(strcmp(curr->name, name) == 0 && curr->pincode == pincode){
                found_pro = curr;
                found = 1;
            }
            curr = curr->next;
        }
    }

    return found_pro;
}

Professional* searchProfessionalById(app* a, int pro_id){
    int n = getProfessionalListSize(a);
    Professional* found_pro = NULL;
    if(n != 0){
        Professional* curr = a->professional_list->next;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(curr->id == pro_id){
                found_pro = curr;
                found = 1;
            }
            curr = curr->next;
        }
    }

    return found_pro;
}

Service* searchInProById(Professional* p, int service_id){
    Service* found_service = NULL;
    int found = 0;
    for(int i = 0; i < p->services_count && !found; i++){
        if(p->service_provided[i]->id == service_id){
            found_service = p->service_provided[i];
            found = 1;
        }
    }

    return found_service;
}

Service* searchServiceByDetails(app* a, const char* name){
    int n = getServiceListSize(a);
    Service* found_service = NULL;
    if(n != 0){
        Service* curr = a->service_list->next;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(strcmp(curr->name, name) == 0){
                found_service = curr;
                found = 1;
            }
            curr = curr->next;
        }
    }

    return found_service;
}

// creating customer

Customer* createCustomer(app* a, const char* name, int pincode, long long contactNo){
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));

    if(newCustomer != NULL){
        int n = getCustomerListSize(a);

        newCustomer->id = n + 1;
        newCustomer->name = (char*)malloc(strlen(name) + 1);
        strcpy(newCustomer->name, name);
        newCustomer->pincode = pincode;
        newCustomer->contactNo = contactNo;
        newCustomer->next = NULL;
    }

    return newCustomer;
}

status_code Register_Customer(app* a){

    status_code sc = Success;

    printf("Please enter customer details :\n");
    int pincode;
    long long contactNo;
    char name[NAME_SIZE];

    printf("Enter your name : ");
    scanf(" %49[^\n]", name);
    printf("Enter your pin-code : ");
    scanf("%d", &pincode);
    printf("Enter your contact number : ");
    scanf("%lld", &contactNo);

    Customer* foundCustomer = searchCustomerByDetails(a, name, pincode);

    if(foundCustomer == NULL){
        Customer* newCustomer = createCustomer(a, name, pincode, contactNo);
        
        if(newCustomer == NULL){
            sc = Faliure;
        } else {
            if(a->customer_list == NULL){
                a->customer_list = newCustomer;
                newCustomer->next = newCustomer;
            } else{
                newCustomer->next = a->customer_list->next;
                a->customer_list->next = newCustomer;
                a->customer_list = newCustomer;
            }
        }
    } else {
        printf("Customer with name '%s' and pincode '%d' already exists with id '%d'\n", foundCustomer->name, foundCustomer->pincode, foundCustomer->id);
        sc = Faliure;
    }

    return sc;

}

// creating professional


void addServices(app* a, Professional* p){
    int n = getServiceListSize(a);
    if(n == 0){
        printf("There are no services available yet, please come later.\n");
    } else {
        char choice = 'y';
        while(choice == 'y' && p->services_count < MAX_PROVIDED_SERVICES && p->services_count < n){
            printf("ID    :    Service    :    Base Price\n");
            Service* curr = a->service_list->next;

            printf("Service List :\n");
            for(int i = 0; i < n; i++){
                printf("%d    :    %s    :    %d$\n", curr->id, curr->name, curr->best_price);
                curr = curr->next;
            }
            printf("Enter the id of the service you want to include : ");
            int service_id;
            scanf("%d", &service_id);

            Service* foundService = searchInProById(p, service_id);

            if(foundService == NULL){
                for(int i = 0; i < n && curr->id != service_id; i++) curr = curr->next;
                if(curr->id == service_id){
                    p->service_provided[p->services_count] = curr;
                    p->services_count++;
                } else {
                    printf("Please enter correct id.\n");
                }
                
                printf("Do you want to continue adding services : ");
                scanf(" %c", &choice);

                if(choice == 'y' && p->services_count == MAX_PROVIDED_SERVICES){
                    printf("You have reached the maximum limit of services you can provide.\n");
                } else if(choice == 'y' && p->services_count == n){
                    printf("You already provide all the available services.\n");
                }

            } else {
                printf("Service with id '%d' already exists in your list.\n", foundService->id);
                printf("Try adding another service.\n");
            }
        }
    }
}

status_code addServicesToProfessional(app* a){
    status_code sc = Success;
    int professional_id;

    printf("Enter professional id : ");
    scanf("%d", &professional_id);

    Professional* p = searchProfessionalById(a, professional_id);

    if(p == NULL){
        printf("Professional with id '%d' does not exist.\n", professional_id);
        sc = Faliure;
    } else if(p->services_count == MAX_PROVIDED_SERVICES){
        printf("Professional '%s' already provides the maximum of %d services.\n", p->name, MAX_PROVIDED_SERVICES);
        sc = Faliure;
    } else {
        addServices(a, p);
        if(p->available == True){
            assingProToServiceRequest(a, p);
        }
    }

    return sc;
}

Professional* createProfessional(app* a, const char* name, int pincode, long long contactNo){
    Professional* newProfessional = (Professional*)malloc(sizeof(Professional));

    if(newProfessional != NULL){
        int n = getProfessionalListSize(a);
        newProfessional->id = n + 1;
        newProfessional->name = (char*)malloc(strlen(name) + 1);
        strcpy(newProfessional->name, name);
        newProfessional->pincode = pincode;
        newProfessional->contactNo = contactNo;
        newProfessional->available = True;
        newProfessional->services_count = 0;
        newProfessional->next = NULL;

        char choice;
        printf("Do you want to add services(y/n) : ");
        scanf(" %c", &choice);
        if(choice == 'y'){
            addServices(a, newProfessional);
        }
    }

    return newProfessional;
}

status_code Register_Professional(app* a){
    status_code sc = Success;

    printf("Please enter professional details :\n");
    int pincode;
    long long contactNo;
    char name[NAME_SIZE];

    printf("Enter your name : ");
    scanf(" %49[^\n]", name);
    printf("Enter your pin-code : ");
    scanf("%d", &pincode);
    printf("Enter your contact number : ");
    scanf("%lld", &contactNo);
    
    Professional* foundProfessional = searchProfessionalByDetails(a, name, pincode);

    if(foundProfessional == NULL){
        Professional* newProfessional = createProfessional(a, name, pincode, contactNo);
        
        if(newProfessional == NULL){
            sc = Faliure;
        } else {
            if(a->professional_list == NULL){
                a->professional_list = newProfessional;
                newProfessional->next = newProfessional;
            } else {
                newProfessional->next = a->professional_list->next;
                a->professional_list->next = newProfessional;
                a->professional_list = newProfessional;
            }
            assignProToServiceRequest(a, newProfessional);
        }


    } else {
        printf("Professional with name '%s' and pincode '%d' already exists with id '%d'\n", foundProfessional->name, foundProfessional->pincode, foundProfessional->id);
        sc = Faliure;
    }

    return sc;
}

// creating service

Service* createService(app* a, const char* name, int price){
    Service* newService = (Service*)malloc(sizeof(Service));

    if(newService != NULL){
        int n = getServiceListSize(a);
        newService->id = n + 1;
        newService->name = (char*)malloc(strlen(name) + 1);
        strcpy(newService->name, name);
        newService->best_price = price;

        newService->next = NULL;
    }
    return newService;
}

status_code Add_Service(app* a){
    status_code sc = Success;

    printf("Please enter service details :\n");
    char name[NAME_SIZE];
    int price;

    printf("Enter your name : ");
    scanf(" %49[^\n]", name);
    printf("Enter your base price : ");
    scanf("%d", &price);

    Service* foundService = searchServiceByDetails(a, name);
    
    if(foundService == NULL){
        Service* newService = createService(a, name, price);
        
        if(newService == NULL){
            sc = Faliure;
        } else {
            if(a->service_list == NULL){
                a->service_list = newService;
                newService->next = newService;
            } else {
                newService->next = a->service_list->next;
                a->service_list->next = newService;
                a->service_list = newService;
            }
        }
    } else {
        printf("Service with name '%s' already exists with id '%d'\n", foundService->name, foundService->id);
        sc = Faliure;
    }

    return sc;
}

// creating service request

void assigneDateTo(Date* date){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    date->year = tm.tm_year + 1900;
    date->month = tm.tm_mon + 1;
    date->day = tm.tm_mday;
    date->hour = tm.tm_hour;
    date->min = tm.tm_min;
    date->sec = tm.tm_sec;
}

Professional* searchProForService(app* a, int service_id, int pincode){
    int n = getProfessionalListSize(a);
    Professional* found_pro = NULL;
    if(n != 0){
        Professional* curr = a->professional_list->next;
        
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            Service* found_service = searchInProById(curr, service_id);
            if(found_service != NULL && curr->available == True){
                if(curr->pincode == pincode){
                    found_pro = curr;
                    found = 1;
                } else if(found_pro == NULL){
                    found_pro = curr;
                }
            }
            curr = curr->next;
        }
    }
    return found_pro;
}

ServiceRequest* createServiceRequestHelper(app* a, Customer* customer, Service* service){
    ServiceRequest* newReq = (ServiceRequest*)malloc(sizeof(ServiceRequest));

    if(newReq != NULL){
        int n = getServiceRequestListSize(a);
        newReq->id = n + 1;
        newReq->customer = customer;
        newReq->service = service;
        assigneDateTo(&newReq->date);
        Professional* assigned_professional = searchProForService(a, service->id, customer->pincode);
        if(assigned_professional == NULL){
            newReq->assigned_professional = NULL;
            newReq->status = Pending;
            printf("No professional available, request is pending\n");
        } else {
            newReq->assigned_professional = assigned_professional;
            newReq->status = Assigned;
            assigned_professional->available = False;
            printf("Assigned Professional to your request is '%s', id '%d', contact no. '%lld'\n", assigned_professional->name, assigned_professional->id, assigned_professional->contactNo);
        }
        newReq->next = NULL;
    }

    return newReq;
}

status_code createServiceRequest(app* a){
    status_code sc = Success;

    int cust_size = getCustomerListSize(a), serv_size = getServiceListSize(a);
    
    if(cust_size != 0 && serv_size != 0){
        int customer_id, service_id;
        int customer_selected = 0, service_selected = 0;
        Customer* customer = NULL;
        while(sc == Success && !customer_selected){
            printf("Enter your customer id : ");
            scanf("%d", &customer_id);
            
            customer = searchCustomerById(a, customer_id);
            
            if(customer == NULL){
                printf("No customer with id '%d' found, please enter correct id\n", customer_id);
                printf("Do you want to try again or exit (y/n) : ");
                char c;
                scanf(" %c", &c);
                if(c != 'y'){
                    sc = Faliure;
                }
            } else {
                customer_selected = 1;
            }
        }
        
        while(sc == Success && !service_selected){
            printf("<Select the service you want>\n");
            Service* curr = a->service_list->next;
            printf("---------------------------------------------------------------------------\n");
            printf("Service Id |    Service Name                |       Base Price\n");
            printf("---------------------------------------------------------------------------\n");
            for(int i = 0; i < serv_size; i++){
                printf("%d      |      %s   |  %d\n", curr->id, curr->name, curr->best_price);
                curr = curr->next;
            }
            printf("Enter service id from above : ");
            scanf("%d", &service_id);
            
            for(int i = 0; i < serv_size && curr->id != service_id; i++){
                curr = curr->next;
            }
            if(curr->id == service_id){
                ServiceRequest* newReq = createServiceRequestHelper(a, customer, curr);
                
                if(newReq == NULL){
                    sc = Faliure;
                } else {
                    if(a->request_list == NULL){
                        a->request_list = newReq;
                        newReq->next = newReq;
                    } else {
                        newReq->next = a->request_list->next;
                        a->request_list->next = newReq;
                        a->request_list = newReq;
                    }
                }
                service_selected = 1;
            } else {
                printf("No service with id '%d' found, please enter correct id\n", service_id);
                printf("Do you want to try again or exit (y/n) : ");
                char c;
                scanf(" %c", &c);
                if(c == 'n'){
                    sc = Faliure;
                }
            }
        }
    } else {
        sc = Faliure;
        if(cust_size == 0){
            printf("No customers found.\n");
        } else {
            printf("No services found.\n");
        }
    }
    
    return sc;

}

// service request functions

void assignProToServiceRequest(app* a, Professional* pro){
    int n = getServiceRequestListSize(a);
    if(n != 0){
        ServiceRequest* curr = a->request_list->next;
        ServiceRequest* chosen = NULL;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(curr->status == Pending){
                Service* found_service = searchInProById(pro, curr->service->id);
                if(found_service != NULL){
                    if(chosen == NULL){
                        chosen = curr;
                    }
                    if(curr->customer->pincode == pro->pincode){
                        chosen = curr;
                        found = 1;
                    }
                }
            }
            curr = curr->next;
        }
        if(chosen != NULL){
            chosen->assigned_professional = pro;
            chosen->status = Assigned;
            pro->available = False;
        }
    }
}

void completeServiceRequest(app* a, int request_id){
    int n = getServiceRequestListSize(a);
    if(n != 0){
        ServiceRequest* curr = a->request_list->next;
        ServiceRequest* found_req = NULL;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(curr->id == request_id){
                found_req = curr;
                found = 1;
            }
            curr = curr->next;
        }
        
        if(found_req != NULL){
            if(found_req->status == Assigned){
                Professional* assigned_professional = found_req->assigned_professional;
                assigned_professional->available = True;
                found_req->status = Completed;
                assignProToServiceRequest(a, assigned_professional);
                printf("Service request with id '%d' has been marked as completed.\n", request_id);
            } else {
                printf("Service request with id '%d' is not in 'Assigned' and cannot be marked as completed.\n", request_id);
            }
        } else {
            printf("No service request with id '%d' found.\n", request_id);
        }
    } else {
        printf("No service request with id '%d' found.\n", request_id);
    }

}

void cancelServiceRequest(app* a, int request_id){

    int n = getServiceRequestListSize(a);
    if(n != 0){
        ServiceRequest* curr = a->request_list->next;
        ServiceRequest* found_req = NULL;
        int found = 0;
        for(int i = 0; i < n && !found; i++){
            if(curr->id == request_id){
                found_req = curr;
                found = 1;
            }
            curr = curr->next;
        }
        
        if(found_req != NULL){
            if(found_req->status == Pending || found_req->status == Assigned){
                Professional* assigned_professional = found_req->assigned_professional;
                if(assigned_professional != NULL){
                    assigned_professional->available = True;
                    assignProToServiceRequest(a, assigned_professional);
                }
                found_req->status = Cancelled;
                printf("Service request with id '%d' has been cancelled.\n", request_id);
            } else {
                printf("Service request with id '%d' is not in 'Pending' or 'Assigned' status and cannot be cancelled.\n", request_id);
            }
        } else {
            printf("No service request with id '%d' found.\n", request_id);
        }
    } else {
        printf("No service request with id '%d' found.\n", request_id);
    }
}

// details displaying functions

void displayProProvidingService(app* a, int service_id){
    int n = getProfessionalListSize(a);
    if(n != 0){
        Professional* curr = a->professional_list->next;
        printf("---------------------------------------------------------------------------\n");
        printf("Professional Id |    Professional Name                |       Contact No\n");
        printf("---------------------------------------------------------------------------\n");
        for(int i = 0; i < n; i++){
            Service* foundService = searchInProById(curr, service_id);
            if(foundService != NULL && curr->available == True){
                printf("%d      |      %s   |  %lld\n", curr->id, curr->name, curr->contactNo);
            }
            curr = curr->next;
        }
    } else {
        printf("No professionals found.\n");
    }
}

void displayServicesByPro(app* a, int pro_id){
    int n = getProfessionalListSize(a);
    if(n != 0){
        Professional* curr = a->professional_list->next;
        for(int i = 0; i < n && curr->id != pro_id; i++){
            curr = curr->next;
        }
        if(curr->id == pro_id){
            printf("---------------------------------------------------------------------------\n");
            printf("Service Id |    Service Name                |       Base Price\n");
            printf("---------------------------------------------------------------------------\n");
            for(int i = 0; i < curr->services_count; i++){
                printf("%d      |      %s   |  %d\n", curr->service_provided[i]->id, curr->service_provided[i]->name, curr->service_provided[i]->best_price);
            }
        } else {
            printf("No professional with id '%d' found.\n", pro_id);
        }
    } else {
        printf("No professionals found.\n");
    }
}

const char* getStatus(req_status status){
    const char* status_str;
    if(status == Pending){
        status_str = "Pending";
    } else if(status == Assigned){
        status_str = "Assigned";
    } else if(status == Completed){
        status_str = "Completed";
    } else {
        status_str = "Cancelled";
    }
    return status_str;
}

void displayCustomerAllRequests(app* a, int customer_id){
    int n = getServiceRequestListSize(a);
    Customer* customer = searchCustomerById(a, customer_id);
    if(n != 0 && customer != NULL){
        ServiceRequest* curr = a->request_list->next;
        printf("---------------------------------------------------------------------------\n");
        printf("Request Id |    Service Name                |       Status\n");
        printf("---------------------------------------------------------------------------\n");
        for(int i = 0; i < n; i++){
            if(curr->customer->id == customer_id){
                printf("%d      |      %s   |  %s\n", curr->id, curr->service->name, getStatus(curr->status));
            }
            curr = curr->next;
        }
    } else {
        if(customer == NULL){
            printf("No customer with id '%d' found.\n", customer_id);
        } else {
            printf("No service requests found for customer with id '%d'.\n", customer_id);
        }
    }
}

// sorting functions

void mergeForServReqCount(void** service_request_arr, int* service_request_count, int left, int mid, int right){
    int i = left;
    int j = mid + 1;
    int k = 0;

    void** temp_service_arr = (void**)malloc(sizeof(void*) * (right - left + 1));
    int* temp_count_arr = (int*)malloc(sizeof(int) * (right - left + 1));

    while(i <= mid && j <= right){
        if(service_request_count[i] >= service_request_count[j]){
            temp_service_arr[k] = service_request_arr[i];
            temp_count_arr[k] = service_request_count[i];
            i++;
        } else {
            temp_service_arr[k] = service_request_arr[j];
            temp_count_arr[k] = service_request_count[j];
            j++;
        }
        k++;
    }

    while(i <= mid){
        temp_service_arr[k] = service_request_arr[i];
        temp_count_arr[k] = service_request_count[i];
        i++;
        k++;
    }

    while(j <= right){
        temp_service_arr[k] = service_request_arr[j];
        temp_count_arr[k] = service_request_count[j];
        j++;
        k++;
    }

    for(i = left, k = 0; i <= right; i++, k++){
        service_request_arr[i] = temp_service_arr[k];
        service_request_count[i] = temp_count_arr[k];
    }

    free(temp_service_arr);
    free(temp_count_arr);
}

void mergeSortForServReqCount(void** service_request_arr, int* service_request_count, int left, int right){
    if(left < right){
        int mid = left + (right - left) / 2;
        mergeSortForServReqCount(service_request_arr, service_request_count, left, mid);
        mergeSortForServReqCount(service_request_arr, service_request_count, mid + 1, right);
        mergeForServReqCount(service_request_arr, service_request_count, left, mid, right);
    }
}

void displayServicesByDecrServiceRequests(app* a){
    int serv_size = getServiceListSize(a), req_size = getServiceRequestListSize(a);
    if(serv_size != 0 && req_size != 0){
        int* service_request_count = (int*)malloc(sizeof(int) * serv_size);
        Service** service_request_arr = (Service**)malloc(sizeof(Service*) * serv_size);

        Service* curr = a->service_list->next;
        for(int i = 0; i < serv_size; i++){
            service_request_count[i] = 0;
            service_request_arr[i] = curr;
            curr = curr->next;
        }

        ServiceRequest* req_curr = a->request_list->next;
        for(int i = 0; i < req_size; i++){
            service_request_count[req_curr->service->id - 1]++;
            req_curr = req_curr->next;
        }

        mergeSortForServReqCount((void**)service_request_arr, service_request_count, 0, serv_size - 1);

        printf("---------------------------------------------------------------------------\n");
        printf("Service Id |    Service Name                |       Service Request Count\n");
        printf("---------------------------------------------------------------------------\n");

        for(int i = 0; i < serv_size; i++){
            printf("%d      |       %s      |       %d\n", service_request_arr[i]->id, service_request_arr[i]->name, service_request_count[i]);
        }
        free(service_request_count);
        free(service_request_arr);          
    } else {
        printf("No services or service requests found.\n");
    }
}

void displayProfessionalsByDecrServiceCompleted(app* a){
    int pro_size = getProfessionalListSize(a), req_size = getServiceRequestListSize(a);
    if(pro_size != 0 && req_size != 0){
        int* service_completed_count = (int*)malloc(sizeof(int) * pro_size);
        Professional** professional_arr = (Professional**)malloc(sizeof(Professional*) * pro_size);

        Professional* curr = a->professional_list->next;
        for(int i = 0; i < pro_size; i++){
            service_completed_count[i] = 0;
            professional_arr[i] = curr;
            curr = curr->next;
        }

        ServiceRequest* req_curr = a->request_list->next;
        for(int i = 0; i < req_size; i++){
            if(req_curr->status == Completed){
                service_completed_count[req_curr->assigned_professional->id - 1]++;
            }
            req_curr = req_curr->next;
        }

        mergeSortForServReqCount((void**)professional_arr, service_completed_count, 0, pro_size - 1);

        printf("---------------------------------------------------------------------------\n");
        printf("Professional Id |    Professional Name                |       Service Completed Count\n");
        printf("---------------------------------------------------------------------------\n");

        for(int i = 0; i < pro_size; i++){
            printf("     %d     |       %s      |       %d\n", professional_arr[i]->id, professional_arr[i]->name, service_completed_count[i]);
        }
        free(service_completed_count);
        free(professional_arr);
    } else {
        printf("No professionals or service requests found.\n");
    }
}

// service request displaying functions

void displayPendingRequests(app* a){
    int req_size = getServiceRequestListSize(a);
    if(req_size != 0){
        ServiceRequest* curr = a->request_list->next;
        printf("---------------------------------------------------------------------------\n");
        printf("Request Id |    Customer Name                |       Service Name | Status\n");
        printf("---------------------------------------------------------------------------\n");
        for(int i = 0; i < req_size; i++){
            if(curr->status == Pending){
                printf("        %d      |       %s      |       %s      |       Pending\n", curr->id, curr->customer->name, curr->service->name);
            }
            curr = curr->next;
        }
    } else {
        printf("No service requests found.\n");
    }
}

void displayServicesCompletedByPro(app* a, int pro_id){
    int req_size = getServiceRequestListSize(a);
    if(req_size != 0){

        Professional* found_pro = searchProfessionalById(a, pro_id);
        
        if(found_pro != NULL){
            printf("---------------------------------------------------------------------------\n");
            printf("Professional Id : %d |    Professional Name : %s\n", pro_id, found_pro->name);
            printf("---------------------------------------------------------------------------\n");
            ServiceRequest* curr = a->request_list->next;
            printf("---------------------------------------------------------------------------\n");
            printf("Request Id |    Customer Name                |       Service Name | Status\n");
            printf("---------------------------------------------------------------------------\n");
            for(int i = 0; i < req_size; i++){
                if(curr->assigned_professional != NULL && curr->assigned_professional->id == pro_id && curr->status == Completed){
                    printf("     %d     |       %s      |       %s      |       Completed\n", curr->id, curr->customer->name, curr->service->name);
                }
                curr = curr->next;
            }
        } else {
            printf("No professional with id '%d' found.\n", pro_id);
        }
    } else {
        printf("No service requests found.\n");
    }
}

// details displaying functions

void displayAllCustomers(app* a){
    int n = getCustomerListSize(a);
    printf("\n=== CUSTOMERS (%d) ===\n", n);
    if(n != 0){
        Customer* c = a->customer_list->next;
        printf("id    |    name    |    pincode    |    contact number\n");
        for(int i = 0; i < n; i++){
            printf("%d    |    %s    |    %d    |    %lld\n", c->id, c->name, c->pincode, c->contactNo);
            c = c->next;
        }
    }
}

void displayAllProfessionals(app* a){
    int n = getProfessionalListSize(a);
    printf("\n=== PROFESSIONALS (%d) ===\n", n);
    if(n != 0){
        Professional* p = a->professional_list->next;
        printf("id    |    name    |    pincode    |    availability    |    services\n");  
        for(int i = 0; i < n; i++){
            printf("%d    |    %s    |    %d    |    %s    |", p->id, p->name, p->pincode, p->available == True ? "available" : "busy");
            for(int j = 0; j < p->services_count; j++){
                printf(" %d", p->service_provided[j]->id);
            }
            p = p->next;
            printf("\n");
        }
    }
}

void displayAllServices(app* a){
    int n = getServiceListSize(a);
    printf("\n=== SERVICES (%d) ===\n", n);
    if(n != 0){
        Service* s = a->service_list->next;
        printf("id    |    name    |    base price\n"); 
        for(int i = 0; i < n; i++){
            printf("%d    |    %s    |    %d\n", s->id, s->name, s->best_price);
            s = s->next;
        }
    }
}

void displayAllServiceRequests(app* a){
    int n = getServiceRequestListSize(a);
    printf("\n=== SERVICE REQUESTS (%d) ===\n", n);
    if(n != 0){
        ServiceRequest* r = a->request_list->next;
        printf("id    |    customer id    |    service id    |    professional id    |    status\n");
        for(int i = 0; i < n; i++){
            printf("%d     |    %d    |    %d    |    %d    |     %s\n",
                   r->id, r->customer->id, r->service->id,
                   r->assigned_professional != NULL ? r->assigned_professional->id : 0,
                   getStatus(r->status));
            r = r->next;
        }
    }   
}

