#ifndef MOBILE_H
#define MOBILE_H

#define MOBILE_SUCCESS 0
#define MOBILE_FAILURE 1
#define MOBILE_SEARCH_FAILED 2;
#define MOBILE_MODIFY_FAILED 3;
#define MOBILE_DELETE_FAILED 4;
#define MOBILE_NON_NDX_MODIFY_FAILED 5;
struct Mobile{
    int model_number; // primary key
    char mobile_company[30];
    char model_name[30];
    char model[65]; //composite key with mobile_company and model_name
    char date_of_relese[30];
    char connectivity[30]; 
    float screen_size; //put constraints
    int battery_capacity;
    int camera_megapixel; 
    char processor[30];
    int weight;   
    int price;
    char manufaturing_status[30];
};

int add_mobile(int model_number, char* mobile_company , char* model_name , char* date_of_relese,
                    char* connectivity , float screen_size , int battery_capacity , int camera_megapixel ,
                    char * processor, int weight , int price , char * manufacturing_status);

void print_mobile_features(struct Mobile *m); // to print mobile features

int search_mobile( int model_number, struct Mobile *m , int if_print); //uses search by ndx key

int modify_mobile_features(int model_number , struct Mobile *m); //to modify mobile attributes

int delete_mobile (int model_number); // to delete mobile entity from the data base

int search_by_mobile_company_and_model (char* mobile_company , char* model , struct Mobile *m , int if_print); //search ny non ndx key

int matcher_mobile( struct Mobile *m,char* model ); // used as a matcher for non ndx key


#endif