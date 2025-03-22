#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("\033[1;33m");
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    printf("\033[0m");
    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        printf("\033[0m");
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        printf("\033[0m");
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        printf("\033[0m");
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*do_encoding function*/
Status do_encoding(EncodeInfo *encInfo)
{
    /*calling open_files function*/
    Status op = open_files(encInfo);
    /*condition to check the return value from the open_files function*/
    if(op==e_success)
    {
        /*declaring the string variable*/
        char str[100];
        /*prompt message to enter the magic string*/
        printf("\033[1;36m");
        printf("Enter the magic string: ");
        printf("\033[0m");
        /*read the magic string from the user*/
        scanf("%[^\n]",str);
        /*copying magic string length to magic_len*/
        encInfo->magic_len=strlen(str);
        /*copying magic string to magic_str*/
        encInfo->magic_str=str;
        /*calling check_capacity function*/
        Status cap=check_capacity(encInfo);
        /*condition to check the return value from the check_capacity function*/
        if(cap==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("Capacity is low");
            printf("\033[0m");
            return e_failure;
        }
        /*if returned value other than e_failure perform next operations*/
        else
        {
            /*calling copy_bmp_header function*/
            Status copy=copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);
            /*condition to check the return value from the copy_bmp_header function*/
            if(copy==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Copying header file failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_magic_string_size function*/
            Status ma_len = encode_magic_string_size(encInfo->magic_len, encInfo);
            /*condition to check the return value from the encode_magic_string_size function*/
            if(ma_len==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding magic string length failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_magic_string function*/
            Status mag_str = encode_magic_string(encInfo->magic_str,encInfo);
            /*condition to check the return value from the encode_magic_string function*/
            if(mag_str==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding magic string failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_secret_file_extn_size function*/
            Status ex_len = encode_secret_file_extn_size(encInfo->extn_size,encInfo);
            /*condition to check the return value from the encode_secret_file_extn_size function*/
            if(ex_len==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding extention length failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_secret_file_extn function*/
            Status  ex_str = encode_secret_file_extn(encInfo->extn,encInfo);
            /*condition to check the return value from the encode_secret_file_extn function*/
            if(ex_str==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding extention failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_secret_file_size function*/
            Status sec_len = encode_secret_file_size(encInfo->file_size,encInfo);
            /*condition to check the return value from the encode_secret_file_size function*/
            if(sec_len==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding secret file size failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling encode_secret_file_data function*/
            Status sec_da = encode_secret_file_data(encInfo);
            /*condition to check the return value from the encode_secret_file_data function*/
            if(sec_da==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding secret file data failed!!");
                printf("\033[0m");
                return e_failure;
            }
            /*calling copy_remaining_img_data function*/
            Status rem = copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
            /*condition to check the return value from the copy_remaining_img_data function*/
            if(rem==e_failure)
            {
                /*if returned value is e_failure print error message and return e_failure*/
                printf("\033[1;31m");
                printf("Copying remaining data failed!!");
                printf("\033[0m");
                return e_failure;
            }
        }       
    }
    /*closing all the opened files*/
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    /*prompt message indicating end of the encoding process and returning e_success*/
    printf("\033[1;32m");
    printf("Encoding successfull!!\n");
    printf("\033[0m");
    return e_success;
}

/*check_capacity function*/
Status check_capacity(EncodeInfo *encInfo)
{
    /*calling get_image_size_for_bmp function and storing the return value in bmp_size*/
    uint bmp_size=get_image_size_for_bmp(encInfo->fptr_src_image);
    /*finding the size required to store magic_len and storing the size in magic_str_size*/
    uint magic_str_size=sizeof(encInfo->magic_len);
    /*performing strstr operation to extract the secret file extension*/
    char *ex=strstr(encInfo->secret_fname,".");
    /*dynamically allocating memory to store the extension*/
    encInfo->extn = malloc(strlen(ex) + 1);
    /*copying the extracted extension to the allocated memory*/
    strcpy(encInfo->extn,ex);
    /*finding the extension size using strlen*/
    encInfo->extn_size=strlen(ex);
    /*finding the size required to store extn_size and storing the size in ex_size*/
    uint ex_size=sizeof(uint);
    /*callling get_file_size and store the returned size in file_size*/
    uint file_len=get_file_size(encInfo->fptr_secret,encInfo);
    /*finding the size required to store file_len and storing the size in file_size*/
    uint file_size=sizeof(uint);
    /*calculating the byte capacity*/
    uint capacity=magic_str_size + (encInfo->magic_len) + (encInfo->extn_size) + ex_size + file_len+file_size;
    /*bringing back offset to start*/
    rewind(encInfo->fptr_src_image);
    /*printing calculated capacity*/
    printf("\033[1;34m");
    printf("Calculated capacity: %u\n", capacity);
    printf("\033[0m");
    /*condition to check whether the capacity required to encode is less than bmp_size*/
    if(((capacity*8)+54)<=bmp_size)
    {
        /*if condition is satisfied return e_success*/
        return e_success;
    }
    else
    {
        /*if condition is not satisfied return e_failure*/ 
        return e_failure;
    }
}

/*get_file_size function*/
uint get_file_size(FILE *fptr,EncodeInfo *encInfo)
{
    /*moving the offset to an end using fseek*/
    fseek(fptr, 0, SEEK_END);  
    /*read the offset position using ftell and store it in file_size*/
    uint file_size = ftell(fptr);
    /*storing the file_size in file_size varibale present in structure*/
    encInfo->file_size = file_size;
    /*bringing back the */
    rewind(fptr);  
    /*returning the file size*/
    return (file_size) ;
}

/*copy_bmp_header function*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*Declaring the string variable temp*/
    char temp[54];
    /*reading 54 bytes from the source file*/
    fread(temp,1,54,fptr_src_image);
    /*write the 54 bytes to the destination file*/
    fwrite(temp,1,54,fptr_dest_image);
    /*returning e_success*/
    return e_success;
}

/*encode_byte_to_lsb function*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*loop to perform the bitwise operation to replace the lsb of the each buffer data with bits of charcter data*/
    for(int i=7;i>=0;i--)
    {
        /*performing & operation with data and 1 left shifted i times and right shift the opearted data*/
        uint data_bit=(data & (1<<i))>>i;
        /*clear the lsb bit of the 'i'th buffer data*/
        image_buffer[7-i]=image_buffer[7-i] & 0xFE;
        /*replacing lsb with the fetched bit using or operation*/
        image_buffer[7-i]=image_buffer[7-i] | data_bit;
    }
    /*returning e_success*/
    return e_success;
}

/*encode_int_to_lsb function*/
Status encode_int_to_lsb(int data, char *image_buffer)
{
    /*loop to perform the bitwise operation to replace the lsb of the each buffer data with bits of int data*/
    for(int i=31;i>=0;i--)
    {
        /*performing & operation with data and 1 left shifted i times and right shift the opearted data*/
        uint data_bit=(data & (1<<i))>>i;
        /*clear the lsb bit of the 'i'th buffer data*/
        image_buffer[31-i]=image_buffer[31-i] & 0xFE;
        /*replacing lsb with the fetched bit using or operation*/
        image_buffer[31-i]=image_buffer[31-i] | data_bit;
    }
    /*returning e_success*/
    return e_success;
}

/*encode_magic_string_size function*/
Status encode_magic_string_size(int magic_str_len, EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,encInfo->fptr_src_image);
    /*calling encode_int_to_lsb function*/
    Status in_ls = encode_int_to_lsb(magic_str_len, buff);
    /*if function encode_int_to_lsb returns e_success write the buffer data to the destination file*/
    if(in_ls==e_success)
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    /*returning e_success*/
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[8];
    /*loop to encode the magic string */
    for(int i=0;magic_string[i]!=0;i++)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,1,8,encInfo->fptr_src_image);
        /*calling encode_byte_to_lsb function*/
        Status by_ls = encode_byte_to_lsb(magic_string[i], buff);
        /*if function encode_byte_to_lsb returns e_success write the buffer data to the destination file*/
        if(by_ls==e_success)
        fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    /*returning e_success*/
    return e_success;
}

/*encode_secret_file_extn_size function*/
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,encInfo->fptr_src_image);
    /*calling encode_int_to_lsb function*/
    Status in_ls = encode_int_to_lsb(extn_size, buff);
    /*if function encode_int_to_lsb returns e_success write the buffer data to the destination file*/
    if(in_ls==e_success)
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    /*returning e_success*/
    return e_success;
}

/*encode_secret_file_extn function*/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[8];
    /*loop to encode the file extension */
    for(int i=0;file_extn[i]!=0;i++)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,1,8,encInfo->fptr_src_image);
        /*calling encode_byte_to_lsb function*/
        Status by_ls = encode_byte_to_lsb(file_extn[i], buff);
        /*if function encode_byte_to_lsb returns e_success write the buffer data to the destination file*/
        if(by_ls==e_success)
        fwrite(buff,1,8,encInfo->fptr_stego_image);
        
    }
    /*returning e_success*/
    return e_success;
}

/*encode_secret_file_size function*/
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,encInfo->fptr_src_image);
    /*calling encode_int_to_lsb function*/
    Status in_ls = encode_int_to_lsb(file_size, buff);
    /*if function encode_int_to_lsb returns e_success write the buffer data to the destination file*/
    if(in_ls==e_success)
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    /*returning e_success*/
    return e_success;
}

/*encode_secret_file_data function*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*declaring the charater array buffer*/
    char buff[8];
    /*declaring the charater ch*/
    char ch;
    /*loop to encode the file data*/
    while ((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,1,8,encInfo->fptr_src_image);
        /*calling encode_byte_to_lsb function*/
        Status by_ls = encode_byte_to_lsb(ch, buff);
        /*if function encode_byte_to_lsb returns e_success write the buffer data to the destination file*/
        if(by_ls==e_success)
        fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    /*returning e_success*/
    return e_success;
}

/*copy_remaining_img_data function*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*declaring the charater ch*/
    char ch;
    /*loop to copy the remaining data in source file to destination file*/
    while( fread(&ch,1,1,fptr_src) )
    {      
        /*writing data to destination file*/
        fwrite(&ch,1,1,fptr_dest);
    }
    /*returning e_success*/
    return e_success;
}