#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "management_system.h"

void initialize_app(app* a){
    a->next_cust_id = 0;
    a->next_pro_id = 0;
    a->next_service_id = 0;
    a->customer_list = NULL;
    a->professional_list = NULL;
    a->service_list = NULL;
}

Customer* searchCustomerByDetails(app* a, const char* name, int pincode){
    Customer* curr = a->customer_list->next;
    Customer* found_cust = NULL;
    int found = 0;
    for(int i = 0; i < a->next_cust_id && !found; i++){
        if(strcmp(curr->name, name) == 0 && curr->pincode == pincode){
            found_cust = curr;
            found = 1;
        }
    }

    return found_cust;
}

Professional* searchProfessionalByDetails(app* a, const char* name, int pincode){
    Professional* curr = a->professional_list->next;
    Professional* found_pro = NULL;
    int found = 0;
    for(int i = 0; i < a->next_pro_id && !found; i++){
        if(strcmp(curr->name, name) == 0 && curr->pincode == pincode){
            found_pro = curr;
            found = 1;
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

Customer* createCustomer(app* a, const char* name, int pincode, int contactNo){
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));

    if(newCustomer != NULL){
        a->next_cust_id++;

        newCustomer->id = a->next_cust_id;
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
    int pincode, contactNo;
    char name[NAME_SIZE];

    printf("Enter your name : ");
    scanf(" %49[^\n]", name);
    printf("Enter your pin-code : ");
    scanf("%d", &pincode);
    printf("Enter your contact number : ");
    scanf("%d", &contactNo);

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

void addServices(app* a, Professional* p){
    if(a->next_service_id == 0){
        printf("There are no services available yes, please come later.\n");
    } else {
        char choice = 'y';
        while(choice == 'y' && p->services_count < MAX_PROVIDED_SERVICES){
            printf("ID    :    Service    :    Base Price\n");
            Service* curr = a->service_list->next;

            printf("Service List :\n");
            for(int i = 0; i < a->next_service_id; i++){
                printf("%d    :    %s    :    %d$", curr->id, curr->name, curr->best_price);
            }
            printf("Enter the id of the service you want to include : \n");
            int service_id;
            scanf("%d", &service_id);

            Service* foundService = searchInProById(p, service_id);

            if(foundService == NULL){
                for(int i = 0; i < a->next_service_id && curr->id != service_id; i++) curr = curr->next;
                if(curr == NULL){
                    printf("Please enter correct id.\n");
                } else {
                    p->service_provided[p->services_count] = curr;
                    p->services_count++;
                }
                
                printf("Do you want to continue adding services : ");
                scanf("%c", &choice);
            } else {
                printf("Service with id '%d' already exists in your list.\n", foundService->id);
                printf("Try adding another service.\n");
            }
        }
    }
}

Professional* createProfessional(app* a, const char* name, int pincode, int contactNo){
    Professional* newProfessional = (Professional*)malloc(sizeof(Professional));

    if(newProfessional != NULL){
        a->next_pro_id++;

        newProfessional->id = a->next_pro_id;
        newProfessional->name = (char*)malloc(strlen(name) + 1);
        strcpy(newProfessional->name, name);
        newProfessional->pincode = pincode;
        newProfessional->contactNo = contactNo;
        newProfessional->next = NULL;

        char choice;
        printf("Do you want to add services(y/n) : ");
        scanf("%c", &choice);
        if(choice == 'y'){
            addServices(a, newProfessional);
        }
    }

    return newProfessional;
}

status_code Register_Professional(app* a){
    status_code sc = Success;

    printf("Please enter professional details :\n");
    int pincode, contactNo;
    char name[NAME_SIZE];

    printf("Enter your name : ");
    scanf(" %49[^\n]", name);
    printf("Enter your pin-code : ");
    scanf("%d", &pincode);
    printf("Enter your contact number : ");
    scanf("%d", &contactNo);
    
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
        }
    } else {
        printf("Professional with name '%s' and pincode '%d' already exists with id '%d'\n", foundProfessional->name, foundProfessional->pincode, foundProfessional->id);
        sc = Faliure;
    }

    return sc;
}

Service* createService(app* a, const char* name, int price){
    Service* newService = (Service*)malloc(sizeof(Service));

    if(newService != NULL){
        a->next_service_id++;

        newService->id = a->next_service_id;
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
    Professional* curr = a->professional_list->next;

    Professional* found_pro = NULL;
    int found = 0;
    for(int i = 0; i < a->next_pro_id && !found; i++){
        int service_found = 0;
        for(int j = 0; j < curr->services_count && !service_found; j++){
            if(curr->service_provided[j]->id == service_id && curr->available == True){
                if(curr == NULL){
                    found_pro = curr;
                    service_found = 1;
                } else if(curr->pincode == pincode){
                    found_pro = curr;
                    service_found = 1;
                    found = 1;
                }
            }
        }
    }
    return found_pro;
}

Customer* searchCustomerById(app* a, int customer_id){
    Customer* curr = a->customer_list->next;
    Customer* found_cust = NULL;
    int found = 0;
    for(int i = 0; i < a->next_cust_id && !found; i++){
        if(curr->id == customer_id){
            found_cust = curr;
            found = 1;
        }
    }

    return found_cust;
}

ServiceRequest* createServiceRequestHelper(app* a, Customer* customer, Service* service){
    ServiceRequest* newReq = (ServiceRequest*)malloc(sizeof(ServiceRequest));

    if(newReq != NULL){
        a->next_req_id++;
        newReq->id = a->next_req_id;
        newReq->customer = customer;
        newReq->service = service;
        assigneDateTo(&newReq->date);
        Professional* assigned_professional = searchProForService(a, service->id, customer->pincode);
        if(assigned_professional == NULL){
            newReq->assigned_professional = NULL;
            newReq->status = Pending;
        } else {
            newReq->assigned_professional = assigned_professional;
            newReq->status = Assigned;
        }
        newReq->next = NULL;
    }

    return newReq;
}

status_code createServiceRequest(app* a){
    status_code sc = Success;

    int customer_id, service_id;
    int customer_selected = 0, service_selected = 0;
    Customer* customer = NULL;
    while(sc == Success && !customer_selected){
        printf("Enter your customer id : ");
        scanf("%d", &customer_id);

        customer = searchCustomerById(a, customer_id);

        if(customer == NULL){
            printf("No customer with id '%d' found, please enter correct id\n");
            printf("Do you want to try again or exit (y/n) : ");
            char c;
            scanf("%c", &c);
            if(c == 'n'){
                sc = Faliure;
            }
        } else {
            customer_selected = 1;
        }
    }

    
    while(sc == Success && !service_selected){
        printf("<Select the service you want>\n");
        Service* curr = a->service_list->next;
        printf("---------------------------------------------------------------------------");
        printf("Service Id |    Service Name                |       Base Price");
        printf("---------------------------------------------------------------------------");
        for(int i = 0; i < a->next_service_id; i++){
            printf("%d      |      %s   |  %d", curr->id, curr->name, curr->best_price);
            curr = curr->next;
        }
        printf("Enter service id from above : ");
        scanf("%d", &service_id);
        
        for(int i = 0; i < a->next_service_id && curr->id != service_id; i++){
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
        }
    }
    
    return sc;

}

void assignProToServiceRequest(app* a, Professional* pro){
    if(a->request_list != NULL){
        ServiceRequest* curr = a->request_list->next;
        int found = 0;
        for(int i = 0; i < a->next_req_id && !found; i++){
            if(curr->status == Pending){
                Service* found_service = searchInProById(pro, curr->service->id);
                if(found_service != NULL){
                    curr->assigned_professional = pro;
                    curr->status = Assigned;
                    pro->available = False;
                    found = 1;
                }
            }
            curr = curr->next;
        }
    }
}

void completeServiceRequest(app* a, int request_id){

    if(a->request_list != NULL){
        ServiceRequest* curr = a->request_list->next;
        ServiceRequest* found_req = NULL;
        int found = 0;
        for(int i = 0; i < a->next_req_id && !found; i++){
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
                printf("Service request with id '%d' is not in 'Assigned' status and cannot be marked as completed.\n", request_id);
            }
        } else {
            printf("No service request with id '%d' found.\n", request_id);
        }
    } else {
        printf("No service request with id '%d' found.\n", request_id);
    }

}

void cancelServiceRequest(app* a, int request_id){

    if(a->request_list != NULL){
        ServiceRequest* curr = a->request_list->next;
        ServiceRequest* found_req = NULL;
        int found = 0;
        for(int i = 0; i < a->next_req_id && !found; i++){
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
                }
                found_req->status = Cancelled;
                assignProToServiceRequest(a, assigned_professional);
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

void displayProProvidingService(app* a, int service_id){
    Professional* curr = a->professional_list->next;
    printf("Professional Id |    Professional Name                |       Contact No");
    printf("---------------------------------------------------------------------------");
    for(int i = 0; i < a->next_pro_id; i++){
        Service* foundService = searchInProById(curr, service_id);
        if(foundService != NULL){
            printf("%d      |      %s   |  %d", curr->id, curr->name, curr->contactNo);
        }
        curr = curr->next;
    }
}

void displayServicesByPro(app* a, int pro_id){
    Professional* curr = a->professional_list->next;
    for(int i = 0; i < a->next_pro_id && curr->id != pro_id; i++){
        curr = curr->next;
    }
    if(curr->id == pro_id){
        printf("Service Id |    Service Name                |       Base Price");
        printf("---------------------------------------------------------------------------");
        for(int i = 0; i < curr->services_count; i++){
            printf("%d      |      %s   |  %d", curr->service_provided[i]->id, curr->service_provided[i]->name, curr->service_provided[i]->best_price);
        }
    } else {
        printf("No professional with id '%d' found.\n", pro_id);
    }
}

void displayCustomerAllRequests(app* a, int customer_id){
    ServiceRequest* curr = a->request_list->next;
    printf("Request Id |    Service Name                |       Status");
    printf("---------------------------------------------------------------------------");
    for(int i = 0; i < a->next_req_id; i++){
        if(curr->customer->id == customer_id){
            printf("%d      |      %s   |  %d", curr->id, curr->service->name, curr->status);
        }
        curr = curr->next;
    }
}

void mergeForServReqCount(Service** service_request_arr, int* service_request_count, int left, int mid, int right){
    int i = left;
    int j = mid + 1;
    int k = 0;

    Service** temp_service_arr = (Service**)malloc(sizeof(Service*) * (right - left + 1));
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

void mergeSortForServReqCount(Service** service_request_arr, int* service_request_count, int left, int right){
    if(left < right){
        int mid = left + (right - left) / 2;
        mergeSortForServReqCount(service_request_arr, service_request_count, left, mid);
        mergeSortForServReqCount(service_request_arr, service_request_count, mid + 1, right);
        mergeForServReqCount(service_request_arr, service_request_count, left, mid, right);
    }
}

void dsiplayServicesByDecrServiceRequests(app* a){

    if(a->next_service_id != 0 && a->next_req_id != 0){
        int* service_request_count = (int*)malloc(sizeof(int) * a->next_service_id);
        Service** service_request_arr = (Service**)malloc(sizeof(Service*) * a->next_service_id);

        Service* curr = a->service_list->next;
        for(int i = 0; i < a->next_service_id; i++){
            service_request_count[i] = 0;
            service_request_arr[i] = curr;
            curr = curr->next;
        }

        ServiceRequest* req_curr = a->request_list->next;
        for(int i = 0; i < a->next_req_id; i++){
            service_request_count[req_curr->service->id - 1]++;
            req_curr = req_curr->next;
        }

        mergeSortForServReqCount(service_request_arr, service_request_count, 0, a->next_service_id - 1);

        printf("---------------------------------------------------------------------------");
        printf("Service Id |    Service Name                |       Service Request Count");
        printf("---------------------------------------------------------------------------");

        for(int i = 0; i < a->next_service_id; i++){
            printf("Service Id : %d | Service Name : %s | Service Request Count : %d\n", service_request_arr[i]->id, service_request_arr[i]->name, service_request_count[i]);
        }
    } else {
        printf("No services or service requests found.\n");
    }
}

void displayProfessionalsByDecrServiceCompleted(app* a){

    if(a->next_pro_id != 0 && a->next_req_id != 0){
        int* service_completed_count = (int*)malloc(sizeof(int) * a->next_pro_id);
        Professional** professional_arr = (Professional**)malloc(sizeof(Professional*) * a->next_pro_id);

        Professional* curr = a->professional_list->next;
        for(int i = 0; i < a->next_pro_id; i++){
            service_completed_count[i] = 0;
            professional_arr[i] = curr;
            curr = curr->next;
        }

        ServiceRequest* req_curr = a->request_list->next;
        for(int i = 0; i < a->next_req_id; i++){
            if(req_curr->status == Completed){
                service_completed_count[req_curr->assigned_professional->id - 1]++;
            }
            req_curr = req_curr->next;
        }

        mergeSortForServReqCount((Service**)professional_arr, service_completed_count, 0, a->next_pro_id - 1);

        printf("---------------------------------------------------------------------------");
        printf("Professional Id |    Professional Name                |       Service Completed Count");
        printf("---------------------------------------------------------------------------");

        for(int i = 0; i < a->next_pro_id; i++){
            printf("Professional Id : %d | Professional Name : %s | Service Completed Count : %d\n", professional_arr[i]->id, professional_arr[i]->name, service_completed_count[i]);
        }
    } else {
        printf("No professionals or service requests found.\n");
    }
}

void displayPendingRequests(app* a){
    if(a->next_req_id != 0){
        ServiceRequest* curr = a->request_list->next;
        printf("---------------------------------------------------------------------------");
        printf("Request Id |    Customer Name                |       Service Name | Status");
        printf("---------------------------------------------------------------------------");
        for(int i = 0; i < a->next_req_id; i++){
            if(curr->status == Pending){
                printf("Request Id : %d | Customer Name : %s | Service Name : %s | Status : Pending\n", curr->id, curr->customer->name, curr->service->name);
            }
            curr = curr->next;
        }
    } else {
        printf("No service requests found.\n");
    }
}

void displayServicesCompletedByPro(app* a, int pro_id){
    if(a->next_req_id != 0){

        Professional* found_pro = searchProfessionalById(a, pro_id);

        if(found_pro != NULL){
            ServiceRequest* curr = a->request_list->next;
            printf("---------------------------------------------------------------------------");
            printf("Request Id |    Customer Name                |       Service Name | Status");
            printf("---------------------------------------------------------------------------");
            for(int i = 0; i < a->next_req_id; i++){
                if(curr->assigned_professional != NULL && curr->assigned_professional->id == pro_id && curr->status == Completed){
                    printf("Request Id : %d | Customer Name : %s | Service Name : %s | Status : Completed\n", curr->id, curr->customer->name, curr->service->name);
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