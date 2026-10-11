#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "management_system.h"

void showAllDetails(app* a){
    displayAllCustomers(a);
    displayAllProfessionals(a);
    displayAllServices(a);
    displayAllServiceRequests(a);
}

void loadData(app* a){
    FILE* fp = fopen("sample_data.txt", "r");
    if(fp == NULL){
        printf("loadData: could not open 'sample_data.txt'.\n");
    } else {
        char name[100];
        int count, i, j, pincode, price, sid, service_count;
        long long contact;

        /* services */
        if(fscanf(fp, "%d", &count) != 1){
            count = 0;
        }
        for(i = 0; i < count; i++){
            if(fscanf(fp, "%99s %d", name, &price) == 2){
                Service* s = createService(a, name, price);
                if(s == NULL){
                    printf("loadData: could not create service '%s'.\n", name);
                } else if(a->service_list == NULL){
                    a->service_list = s;
                    s->next = s;
                } else {
                    s->next = a->service_list->next;
                    a->service_list->next = s;
                    a->service_list = s;
                }
            }
        }

        /* customers */
        if(fscanf(fp, "%d", &count) != 1){
            count = 0;
        }
        for(i = 0; i < count; i++){
            if(fscanf(fp, "%99s %d %lld", name, &pincode, &contact) == 3){
                Customer* c = createCustomer(a, name, pincode, contact);
                if(c == NULL){
                    printf("loadData: could not create customer '%s'.\n", name);
                } else if(a->customer_list == NULL){
                    a->customer_list = c;
                    c->next = c;
                } else {
                    c->next = a->customer_list->next;
                    a->customer_list->next = c;
                    a->customer_list = c;
                }
            }
        }

        /* professionals: name pincode contact service_count service_ids */
        if(fscanf(fp, "%d", &count) != 1){
            count = 0;
        }
        for(i = 0; i < count; i++){
            if(fscanf(fp, "%99s %d %lld %d", name, &pincode, &contact, &service_count) == 4){
                Professional* p = (Professional*)malloc(sizeof(Professional));
                if(p == NULL){
                    printf("loadData: could not create professional '%s'.\n", name);
                } else {
                    p->id = getProfessionalListSize(a) + 1;
                    p->name = (char*)malloc(strlen(name) + 1);
                    if(p->name != NULL){
                        strcpy(p->name, name);
                    }
                    p->pincode = pincode;
                    p->contactNo = contact;
                    p->available = True;
                    p->services_count = 0;
                    p->next = NULL;

                    int n_services = getServiceListSize(a);
                    for(j = 0; j < service_count; j++){
                        if(fscanf(fp, "%d", &sid) == 1){
                            Service* s = NULL;
                            if(a->service_list != NULL){
                                s = a->service_list->next;
                                for(int k = 0; k < n_services && s->id != sid; k++){
                                    s = s->next;
                                }
                            }

                            if(s == NULL || s->id != sid){
                                printf("loadData: service id '%d' not found, skipped for '%s'.\n", sid, name);
                            } else if(searchInProById(p, sid) != NULL){
                                printf("loadData: service id '%d' repeated for '%s', skipped.\n", sid, name);
                            } else if(p->services_count >= MAX_PROVIDED_SERVICES){
                                printf("loadData: '%s' already has %d services, service id '%d' skipped.\n", name, MAX_PROVIDED_SERVICES, sid);
                            } else {
                                p->service_provided[p->services_count] = s;
                                p->services_count++;
                            }
                        }
                    }

                    if(p->name == NULL){
                        printf("loadData: could not create professional '%s'.\n", name);
                        free(p);
                    } else if(a->professional_list == NULL){
                        a->professional_list = p;
                        p->next = p;
                    } else {
                        p->next = a->professional_list->next;
                        a->professional_list->next = p;
                        a->professional_list = p;
                    }
                }
            }
        }

        fclose(fp);
    }
}

void freeAll(app* a){
    if(a->customer_list != NULL){
        Customer* curr = a->customer_list->next;
        a->customer_list->next = NULL;
        while(curr != NULL){
            Customer* nxt = curr->next;
            free(curr->name);
            free(curr);
            curr = nxt;
        }
        a->customer_list = NULL;
    }
    if(a->professional_list != NULL){
        Professional* curr = a->professional_list->next;
        a->professional_list->next = NULL;
        while(curr != NULL){
            Professional* nxt = curr->next;
            free(curr->name);
            free(curr);
            curr = nxt;
        }
        a->professional_list = NULL;
    }
    if(a->service_list != NULL){
        Service* curr = a->service_list->next;
        a->service_list->next = NULL;
        while(curr != NULL){
            Service* nxt = curr->next;
            free(curr->name);
            free(curr);
            curr = nxt;
        }
        a->service_list = NULL;
    }
    if(a->request_list != NULL){
        ServiceRequest* curr = a->request_list->next;
        a->request_list->next = NULL;
        while(curr != NULL){
            ServiceRequest* nxt = curr->next;
            free(curr);
            curr = nxt;
        }
        a->request_list = NULL;
    }
}

void printMenu(){
    printf("\n========== URBAN COMPANY SERVICE MANAGEMENT ==========\n");
    printf(" 1. Register customer\n");
    printf(" 2. Register professional\n");
    printf(" 3. Add service\n");
    printf(" 4. Create service request\n");
    printf(" 5. Complete service request\n");
    printf(" 6. Cancel service request\n");
    printf(" 7. Show available professionals for a service\n");
    printf(" 8. Show services provided by a professional\n");
    printf(" 9. Show all requests of a customer\n");
    printf("10. Show services by decreasing number of requests\n");
    printf("11. Show professionals by decreasing completed requests\n");
    printf("12. Show pending requests\n");
    printf("13. Show service history of a professional\n");
    printf("14. Load sample data (testing)\n");
    printf("15. Show everything (testing)\n");
    printf("16. Add services to a professional\n");
    printf("17. Exit\n");
    printf("======================================================\n");
}
 
int main(){
    app a;
    initialize_app(&a);
 
    int choice;
    int id = 0;
    int quit = 0;
    while(!quit){
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
 
        if(choice == 1){
            status_code sc = Register_Customer(&a);
            if(sc == Success){
                printf("Customer registered with id '%d'.\n", a.customer_list->id);
            } else {
                printf("Customer could not be registered.\n");
            }
        } else if(choice == 2){
            status_code sc = Register_Professional(&a);
            if(sc == Success){
                printf("Professional registered with id '%d'.\n", a.professional_list->id);
            } else {
                printf("Professional could not be registered.\n");
            }
        } else if(choice == 3){
            status_code sc = Add_Service(&a);
            if(sc == Success){
                printf("Service added with id '%d'.\n", a.service_list->id);
            } else {
                printf("Service could not be added.\n");
            }
        } else if(choice == 4){
            status_code sc = createServiceRequest(&a);
            if(sc == Success){
                printf("Service request process finished.\n");
            } else {
                printf("Service request was not created.\n");
            }
        } else if(choice == 5){
            printf("Enter request id to complete : ");
            scanf("%d", &id);
            completeServiceRequest(&a, id);
        } else if(choice == 6){
            printf("Enter request id to cancel : ");
            scanf("%d", &id);
            cancelServiceRequest(&a, id);
        } else if(choice == 7){
            printf("Enter service id to find professionals : ");
            scanf("%d", &id);
            displayProProvidingService(&a, id);
        } else if(choice == 8){
            printf("Enter professional id : ");
            scanf("%d", &id);
            displayServicesByPro(&a, id);
        } else if(choice == 9){
            printf("Enter customer id : ");
            scanf("%d", &id);
            displayCustomerAllRequests(&a, id);
        } else if(choice == 10){
            displayServicesByDecrServiceRequests(&a);
        } else if(choice == 11){
            displayProfessionalsByDecrServiceCompleted(&a);
        } else if(choice == 12){
            displayPendingRequests(&a);
        } else if(choice == 13){
            printf("Enter professional id : ");
            scanf("%d", &id);
            displayServicesCompletedByPro(&a, id);
        } else if(choice == 14){
            loadData(&a);
        } else if(choice == 15){
            showAllDetails(&a);
        } else if(choice == 16){
            addServicesToProfessional(&a);
        } else if(choice == 17){
            quit = 1;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    freeAll(&a);
    printf("Goodbye.\n");
    return 0;
}