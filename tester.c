#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "mobile.h"
#include "pds.h"
int main()
{
    int file_status=0;
    int status;
    int input;
    struct Mobile *buffer_rec;
    buffer_rec = (struct Mobile *)malloc(sizeof(struct Mobile));
    while(1)
    {
        printf("=========================================================\n");
        printf("select operation\n");
        printf("1 for open PDS \n");
        printf("2 for add_mobile\n");
        printf("3 for search mobile (ndx search)\n");
        printf("4 for modify mobile features\n");
        printf("5 for delete mobile\n");
        printf("6 for search by mobile company and model (non ndx search)\n"); 
        printf("7 for closing PDS\n");
        printf("8 for exit\n");
        printf("=========================================================\n");
        scanf("%d",&input);
        if(input ==1)
        {
            //pds_open
            status = pds_open("scandemo",sizeof(struct Mobile));
            if(status == PDS_SUCCESS)
            {
                file_status=1;
                printf("pds opened successfully\n");
            }
            else
            {
                printf("pds open failed\n");
            }     
        }
        else if(input==2)
        {
            //add_mobile
            int buffer_model_number;
            char buffer_model_number_char[100];
            // buffer_model_number_char = buffer_model_number.ToString();
            // printf("=========================================================\n");
            // itoa(buffer_model_number, buffer_model_number_char, 10);
            // buffer_model_number_char = itoa(buffer_model_number);
            // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
            printf("enter model number (key)\n");
            scanf("%d",&buffer_model_number);

            sprintf(buffer_model_number_char,"%d", buffer_model_number);

            char buffer_mobile_company[100];
            strcpy(buffer_mobile_company,"company-");
            strcat(buffer_mobile_company,buffer_model_number_char);

            char buffer_model_name[100];
            strcpy(buffer_model_name,"model-");
            strcat(buffer_model_name,buffer_model_number_char);

            char buffer_model[100];
            strcpy(buffer_model,buffer_mobile_company);
            strcat(buffer_model,"+");
            strcat(buffer_model,buffer_model_number_char);

            char buffer_date[100];
            strcpy(buffer_date, "date-");
            strcat(buffer_date,buffer_model_number_char);

            char buffer_connectivity[100];
            strcpy(buffer_connectivity,"connectivity-");
            strcat(buffer_connectivity,buffer_model_number_char);

            float buffer_screen_size;
            buffer_screen_size = (float)buffer_model_number;

            int buffer_battery_capacity;
            buffer_battery_capacity = buffer_model_number;

            int buffer_camera_megapixel;
            buffer_camera_megapixel = buffer_model_number;

            char buffer_processor[100];
            strcpy(buffer_processor,"processor-");
            strcat(buffer_processor,buffer_model_number_char);

            int buffer_weight;
            buffer_weight = buffer_model_number;

            int buffer_price;
            buffer_price = buffer_model_number;

            char buffer_status[100];
            strcpy(buffer_status,"manufacturing_status-");
            strcat(buffer_status,buffer_model_number_char);

            status = add_mobile(buffer_model_number,buffer_mobile_company,buffer_model_name,buffer_date,buffer_connectivity,buffer_screen_size,buffer_battery_capacity,buffer_camera_megapixel,buffer_processor,buffer_weight,buffer_price,buffer_status);
            if(status == MOBILE_SUCCESS)
            {
                printf("moblie added successfully\n");
            }
            else
            {
                printf("mobile add failed\n");
            }  
        }
        else if(input == 3)
        {
            //search mobile
            int buffer_model_number_for_search;
            printf("enter model number for search \n");
            scanf("%d",&buffer_model_number_for_search); 
            status = search_mobile(buffer_model_number_for_search,buffer_rec,1);
            if(status != MOBILE_SUCCESS)
            {
                printf("Mobile not found\n");
            } 
        }
        else if(input == 4)
        {
            //modify mobile features
            int temp_key;
            printf("enter model number \n");
            scanf("%d",&temp_key);
            int temp_status = get_rec_by_ndx_key(temp_key,buffer_rec);
            print_mobile_features(buffer_rec);
            if(temp_status != PDS_SUCCESS)
            {
                printf("KEY not found\n");
            }
            else
            {
                int temp_input;
                while(1)
                {
                    printf("****************************\n");
                    printf("press \n");
                    printf("1 for date of relese\n");
                    printf("2 for connectivity\n");
                    printf("3 for screen size\n");
                    printf("4 for battery capacity\n");
                    printf("5 for camera megapixel\n");
                    printf("6 for processor\n");
                    printf("7 for weight\n");
                    printf("8 for price\n");
                    printf("9 for manufaturing status\n");
                    printf("10 to exit\n");
                    printf("****************************\n");
                    
                    scanf("%d",&temp_input);
                    if(temp_input==10)
                    {
                        printf("the new features are \n");
                        print_mobile_features(buffer_rec);
                        modify_mobile_features(temp_key,buffer_rec);
                        break;
                    }
                    else if(temp_input==1)
                    {
                        strcat(buffer_rec->date_of_relese,"-modified");
                        printf("modified date as %s\n",buffer_rec->date_of_relese);
                    }
                    else if(temp_input ==2)
                    {
                        strcat(buffer_rec->connectivity,"-modified");
                        printf("modified connectivity as %s\n",buffer_rec->connectivity);
                    }
                    else if(temp_input ==3)
                    {
                        float temp_screen;
                        printf("enter new screen size\n");
                        scanf("%f",&temp_screen);
                        buffer_rec->screen_size = temp_screen;
                        printf("modified screen size as %f\n",buffer_rec->screen_size);
                    }
                    else if(temp_input ==4)
                    {
                        int temp_battery_capacity;
                        printf("enter new battery capacity\n");
                        scanf("%d",&temp_battery_capacity);
                        buffer_rec->battery_capacity = temp_battery_capacity;
                        printf("modified battery capacity as %d\n",buffer_rec->battery_capacity);
                    }
                    else if(temp_input ==5)
                    {
                        int temp_camera_megapixel;
                        printf("enter new camera megapixel\n");
                        scanf("%d",&temp_camera_megapixel);
                        buffer_rec->camera_megapixel = temp_camera_megapixel;
                        printf("modified camera megapixel as %d\n",buffer_rec->camera_megapixel);
                    }
                    else if(temp_input ==6)
                    {
                        strcat(buffer_rec->processor,"-modified");
                        printf("modified processor as %s\n",buffer_rec->processor);
                    }
                    else if(temp_input ==7)
                    {
                        int temp_weight;
                        printf("enter new weight\n");
                        scanf("%d",&temp_weight);
                        buffer_rec->weight = temp_weight;
                        printf("modified weight as %d\n",buffer_rec->weight);
                    }
                    else if(temp_input ==8)
                    {
                        int temp_price;
                        printf("enter new price\n");
                        scanf("%d",&temp_price);
                        buffer_rec->price = temp_price;
                        printf("modified price as %d\n",buffer_rec->price);
                    }
                    else if(temp_input ==9)
                    {
                        strcat(buffer_rec->manufaturing_status,"-modified");
                        printf("modified manufacturing status as %s\n",buffer_rec->manufaturing_status);
                    }
                }
            }
            
        }
        else if(input ==5)
        {
            //delete mobile
            printf("enter model number to be deleted\n");
            int delete_mobile_model;
            scanf("%d",&delete_mobile_model);
            int delete_status = search_mobile(delete_mobile_model,buffer_rec,0);
            if(delete_status != MOBILE_SUCCESS)
            {
                printf("not able to delete the given model\n");
            }
            else
            {
                delete_status = delete_mobile(delete_mobile_model);
                if(delete_status != MOBILE_SUCCESS)
                {
                    printf("not able to delete the given model\n");
                }
                printf("mobile deleted successfully\n");
            }    
        }
        else if(input ==6)
        {
            //non ndx search
            char search_mobile_company[100];
            char search_model_name[100];
            printf("enter mobile company (company-model_number)\n");
            scanf("%s",search_mobile_company);
            printf("enter model name (model-model_number)\n");
            scanf("%s",search_model_name);
            int non_ndx_seach_status;
            non_ndx_seach_status = search_by_mobile_company_and_model(search_mobile_company,search_model_name,buffer_rec,1);
            if(non_ndx_seach_status != MOBILE_SUCCESS)
            {
                printf("search failed \n");
            }
        }
        else if(input ==7)
        {
            int close_status = pds_close();
            if(close_status != PDS_SUCCESS)
            {
                printf("pds not closed\n");
            }
            else
            {
                printf("pds closed successfully\n");
                file_status=0;
            }
            
        }
        else if(input == 8)
        {
            if(file_status==1)
            {
                int close_status = pds_close();
                if(close_status != PDS_SUCCESS)
                {
                    printf("pds not closed\n");
                }
                else
                {
                    printf("pds closed successfully\n");
                    file_status=0;
                }
            }
            printf("thankyou \n");
            break;
        }
    }
}