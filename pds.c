#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "bst.h"

struct PDS_RepoInfo repo_handle;
int pds_open(char *repo_name,int rec_size)
{
    if(repo_handle.repo_status==PDS_REPO_OPEN)
    {
        return PDS_REPO_ALREADY_OPEN;
    }
    strcpy(repo_handle.pds_name,repo_name);
    char temp1[30];
    char temp2[30];
    strcpy(temp1,repo_handle.pds_name);
    strcpy(temp2,repo_handle.pds_name);
    strcat(temp1,".dat");
    strcat(temp2,".ndx");
    repo_handle.pds_data_fp = (FILE*)fopen(temp1,"ab+");
    fclose(repo_handle.pds_data_fp);
    repo_handle.pds_data_fp=fopen(temp1,"rb+");
    repo_handle.pds_ndx_fp = (FILE*)fopen(temp2,"ab+");
    if(repo_handle.pds_data_fp == NULL || repo_handle.pds_ndx_fp == NULL)
    {
        return PDS_FILE_ERROR;
    }
    repo_handle.repo_status=PDS_REPO_OPEN;
    repo_handle.rec_size = rec_size;
    repo_handle.pds_bst = NULL;
    int pds_load_ndx()
    {
        fseek(repo_handle.pds_ndx_fp,0,SEEK_END);
        if(ftell(repo_handle.pds_ndx_fp)==0)
        {
            return 1;
        }
        fseek(repo_handle.pds_ndx_fp,0,SEEK_SET);
        while(!feof(repo_handle.pds_ndx_fp))
        {
            struct PDS_NdxInfo *buffer;
            buffer = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
            fread(buffer,sizeof(struct PDS_NdxInfo),1,repo_handle.pds_ndx_fp);
            bst_add_node(&repo_handle.pds_bst,buffer->key,buffer);
        }
        return 0;
    }
    pds_load_ndx();
    fclose(repo_handle.pds_ndx_fp);

    return PDS_SUCCESS;
}
//check for dublicate keys
int put_rec_by_key(int key, void *rec)
{
    if(repo_handle.repo_status==PDS_REPO_OPEN)
    {
        fseek(repo_handle.pds_data_fp,0,SEEK_END);
        int off = ftell(repo_handle.pds_data_fp);
        struct PDS_NdxInfo *buffer;
        buffer = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        buffer->key=key;
        buffer->offset = off;
        buffer->is_deleted = 0;
        int status = bst_add_node(&repo_handle.pds_bst,key,buffer);
        if(status == BST_DUP_KEY)
        {
            return PDS_ADD_FAILED;
        }
        fwrite(&key,sizeof(int),1,repo_handle.pds_data_fp);
        fwrite(rec,repo_handle.rec_size,1,repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
    {
        return PDS_REPO_NOT_OPEN;
    }    
}
//check if file is not open 
int get_rec_by_ndx_key(int key , void *rec)
{   
    if(repo_handle.repo_status==PDS_REPO_OPEN)
    {   
        struct BST_Node *buffer ;
        buffer = bst_search(repo_handle.pds_bst,key);
        if(buffer == NULL)
        {
            return PDS_REC_NOT_FOUND;
        }
        struct PDS_NdxInfo *temp ;
        temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        temp =buffer->data;
        if(temp->is_deleted == 1)
        {
            return PDS_REC_NOT_FOUND;
        }
        int temp_key;
        fseek(repo_handle.pds_data_fp,temp->offset,SEEK_SET);
        fread(&temp_key,sizeof(int),1,repo_handle.pds_data_fp);
        if(temp_key != key)
        {
            return PDS_REC_NOT_FOUND;
        }
        fread(rec,repo_handle.rec_size,1,repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
    {
        return PDS_REPO_NOT_OPEN;
    }  
}
int modify_rec_by_key(int key, void *rec)
{
    if(repo_handle.repo_status==PDS_REPO_OPEN)
    {   
        struct BST_Node *buffer ;
        buffer = bst_search(repo_handle.pds_bst,key);
        if(buffer == NULL)
        {
            return PDS_MODIFY_FAILED;
        }
        struct PDS_NdxInfo *temp ;
        temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        temp =buffer->data;
        if(temp->is_deleted == 1)
        {
            return PDS_MODIFY_FAILED;
        }
        fseek(repo_handle.pds_data_fp,temp->offset,SEEK_SET);
        fwrite(&key,sizeof(int),1,repo_handle.pds_data_fp);
        fwrite(rec,repo_handle.rec_size,1,repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
    {
        return PDS_REPO_NOT_OPEN;
    }  
}
int delete_rec_by_ndx_key( int key )
{    
    if(repo_handle.repo_status==PDS_REPO_OPEN)
    {   
        struct BST_Node *buffer ;
        buffer = bst_search(repo_handle.pds_bst,key);
        if(buffer == NULL)
        {
            return PDS_DELETE_FAILED;
        }
        struct PDS_NdxInfo *temp ;
        temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        temp =buffer->data;
        temp->is_deleted = 1;
        return PDS_SUCCESS;
    }
    else
    {
        return PDS_REPO_NOT_OPEN;
    }  
}
int get_rec_by_non_ndx_key(void *key,void *rec,int (*matcher)(void *rec, void *key), int *io_count )
{
    if(repo_handle.repo_status == PDS_REPO_OPEN)
    {
        fseek(repo_handle.pds_data_fp,0,SEEK_SET);
        int buffer_key; 
        
        while(!feof(repo_handle.pds_data_fp))
        {
            // printf("ttttttttttttttttttttttttttttttttttttt\n");
            fread(&buffer_key,sizeof(int),1,repo_handle.pds_data_fp);
            fread(rec,repo_handle.rec_size,1,repo_handle.pds_data_fp);
            *io_count = *io_count+1;
            if(matcher(rec,key)==1)
            {
                struct BST_Node *buffer ;
                buffer = bst_search(repo_handle.pds_bst,buffer_key);
                struct PDS_NdxInfo *temp;
                temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
                temp =buffer->data;
                if(temp->is_deleted==1)
                {
                    return PDS_REC_NOT_FOUND;
                }
                // printf("xxxx %d XXXXX\n",*io_count);
                return PDS_SUCCESS;
            }
        }
        return PDS_REC_NOT_FOUND;
    }
    else
    {
        return PDS_REPO_NOT_OPEN;
    }
    
}
int pds_close()
{
    if(repo_handle.repo_status != PDS_REPO_OPEN)
    {
        return PDS_REPO_NOT_OPEN;
    }
    char temp1[30];
    strcpy(temp1,repo_handle.pds_name);
    strcat(temp1,".ndx");
    repo_handle.pds_ndx_fp = (FILE*)fopen(temp1,"wb+");
    if(repo_handle.pds_ndx_fp == NULL)
    {
        return PDS_NDX_SAVE_FAILED;
    }
    fseek(repo_handle.pds_ndx_fp,0,SEEK_SET);
    struct BST_Node *buffer;
    void bst_write(struct BST_Node *root)
    {
        if(root == NULL)
        {
            return;
        }
        else
        {
            struct BST_Node *buffer ;
            buffer = bst_search(repo_handle.pds_bst,root->key);
            struct PDS_NdxInfo *temp;
            temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
            temp =buffer->data;
            if(temp->is_deleted==0)
            {
                fwrite(root->data,sizeof(struct PDS_NdxInfo),1,repo_handle.pds_ndx_fp);
            }
            bst_write(root->left_child);
            bst_write(root->right_child);
        }
    }
    bst_write(repo_handle.pds_bst);
    fclose(repo_handle.pds_data_fp);
    fclose(repo_handle.pds_ndx_fp);
    bst_destroy(repo_handle.pds_bst);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}