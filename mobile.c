#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "mobile.h"

int add_mobile(int model_number, char* mobile_company , char* model_name , char* date_of_relese,
                    char* connectivity , float screen_size , int battery_capacity , int camera_megapixel ,
                    char * processor, int weight , int price , char * manufacturing_status)
                    {
                        struct Mobile buffer;
                        buffer.model_number=model_number;
                        strcpy(buffer.mobile_company,mobile_company);
                        strcpy(buffer.model_name,model_name);
                        strcpy(buffer.model,mobile_company);
                        strcat(buffer.model,"+");
                        strcat(buffer.model,model_name);
                        strcpy(buffer.date_of_relese,date_of_relese);
                        strcpy(buffer.connectivity,connectivity);
                        buffer.screen_size = screen_size;
                        buffer.battery_capacity = battery_capacity;
                        buffer.camera_megapixel = camera_megapixel;
                        strcpy(buffer.processor,processor);
                        buffer.weight = weight;
                        buffer.price = price;
                        strcpy(buffer.manufaturing_status,manufacturing_status);

                        int status;
                        status = put_rec_by_key(model_number,&buffer);
                        if(status != PDS_SUCCESS)
                        {
                            return MOBILE_FAILURE;
                        }
                        return MOBILE_SUCCESS;
                    }

void print_mobile_features(struct Mobile *m)
{
    printf("************************************\n");
    printf("model_number  %d\n",m->model_number);
    printf("company  %s\n",m->mobile_company);
    printf("model_name  %s\n",m->model_name);
    printf("model %s  (composite key)\n",m->model);
    printf("date of relese  %s\n",m->date_of_relese);
    printf("connectivity  %s\n",m->connectivity);
    printf("model_number  %f\n",m->screen_size);
    printf("battery capacity  %d\n",m->battery_capacity);
    printf("camera_megapixel  %d\n",m->camera_megapixel);
    printf("processor  %s\n",m->processor);
    printf("weight  %d\n",m->weight);
    printf("price  %d\n",m->price);
    printf("manufacturing_status  %s\n",m->manufaturing_status);
    printf("************************************\n");
}

int search_mobile( int model_number, struct Mobile *m ,int if_print)
{
    int status;
    status = get_rec_by_ndx_key(model_number,m);
    if(status != PDS_SUCCESS)
    {
        return MOBILE_SEARCH_FAILED;
    }
    if(if_print == 1)
    {
        print_mobile_features(m);
    }
    return MOBILE_SUCCESS;
}

int modify_mobile_features(int model_number , struct Mobile *m)
{
    int status = modify_rec_by_key(model_number, m);
    if(status != PDS_SUCCESS)
    {
        return MOBILE_MODIFY_FAILED;
    }
    return MOBILE_SUCCESS;
}

int delete_mobile(int model_number)
{
    int status = delete_rec_by_ndx_key(model_number);
    if(status != PDS_SUCCESS)
    {
        return MOBILE_DELETE_FAILED;
    }
    return MOBILE_SUCCESS;
}

int matcher_mobile( struct Mobile *m,char* model )
{
    if(strcmp(model,m->model) != 0)
    {
        return 0;
    }
    return 1;
}

int search_by_mobile_company_and_model (char* mobile_company , char* model_name , struct Mobile *m , int if_print)
{
    char temp[100];
    strcpy(temp,mobile_company);
    strcat(temp,"+");
    strcat(temp,model_name);
    int io_count;
    int status = get_rec_by_non_ndx_key(temp,m,matcher_mobile,&io_count);
    if(status != PDS_SUCCESS)
    {
        return MOBILE_NON_NDX_MODIFY_FAILED;
    }
    if(if_print==1)
    {
        print_mobile_features(m);
    }
    return MOBILE_SUCCESS;
}



