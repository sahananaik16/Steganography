#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

/*do_decoding function*/
Status do_decoding(DecodeInfo *decInfo)
{
    /*calling open_files function*/
    Status op = open_defiles(decInfo);
    /*condition to check the return value from the open_files function*/
    if(op==e_failure)
    {
        /*if return value is e_failure return e_failure*/
        return e_failure;
    }
    /*if return value is e_success continue with decoding operations*/
    else
    {
        /*calling skip_bmp_header function*/
        Status skip = skip_bmp_header(decInfo->fptr_stego_image);
        /*condition to check the return value from the skip_bmp_header function*/
        if(skip==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("skipping bmp header is failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*calling decode_magic_string_size function*/
        Status ma_size = decode_magic_string_size(decInfo);
        /*condition to check the return value from the decode_magic_string_size function*/
        if(ma_size==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding magic string size failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*declaring the string str with magic string length as size*/
        char str[decInfo->magic_string_len +1];
        /*calling decode_magic_string function*/
        Status ma_str = decode_magic_string(str,decInfo);
        /*condition to check the return value from the decode_magic_string function*/
        if(ma_str==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding magic string failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*declaring str1 string*/
        char str1[20];
        /*prompt message to enter the magic string*/
        printf("\033[1;36m");
        printf("Enter the magic string: ");
        printf("\033[0m");
        /*read the magic string from the user*/
        scanf("%s",str1);
        /*comparing the decoded magic string and user entered string*/
        if(strcmp(str,str1)!=0)
        {
            /*if string string is not matching printing error message and returning e_failure*/
            printf("\033[1;31m");
            printf("Magic string is not matching!!");
            printf("\033[0m");
            return e_failure;
        }
        /*calling decode_secret_file_extn_size function*/
        Status et_siz = decode_secret_file_extn_size(decInfo);
        /*condition to check the return value from the decode_secret_file_extn_size function*/
        if(et_siz==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding secret file extension size failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*declaring the string str with file extention length as size*/
        char str2[decInfo->file_extn_size +1];
        /*calling decode_secret_file_extn function*/
        Status extn = decode_secret_file_extn(str2,decInfo);
        /*condition to check the return value from the decode_secret_file_extn function*/
        if(extn==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding secret file extension failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*merging the output name with extension*/
        strcat(decInfo->output_fname,str2);
        /*opening the output file*/
        decInfo->fptr_output_file = fopen(decInfo->output_fname, "w");
        /*calling decode_secret_file_size function*/
        Status sec_siz = decode_secret_file_size(decInfo);
        /*condition to check the return value from the decode_secret_file_size function*/
        if(sec_siz==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding secret file size failed!");
            printf("\033[0m");
            return e_failure;
        }
        /*calling decode_secret_file_data function*/
        Status sec_da = decode_secret_file_data(decInfo);
        /*condition to check the return value from the decode_secret_file_data function*/
        if(sec_da==e_failure)
        {
            /*if returned value is e_failure print error message and return e_failure*/
            printf("\033[1;31m");
            printf("decoding secret file data failed!");
            printf("\033[0m");
            return e_failure;
        }
    }
    /*closing the opened files*/
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output_file);
    printf("\033[1;32m");
    printf("Decoding successfull!!");
    printf("\033[0m");
}

/*open_defiles function*/
Status open_defiles(DecodeInfo *decInfo)
{
    /*opening source Image file*/
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    /*Error handling*/
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
        printf("\033[1;31m");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        printf("\033[0m");

    	return e_failure;
    }
    return e_success;
}

/*skip_bmp_header function*/
Status skip_bmp_header(FILE *fptr_stego_image)
{
    /*moving offset to 54 th position from 0th position*/
    fseek(fptr_stego_image, 54, SEEK_SET);
    /*returning e_success*/
    return e_success;
}

/*decode_magic_string_size function*/
Status decode_magic_string_size(DecodeInfo *decInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,decInfo->fptr_stego_image);
    /*calling decode_lsb_to_int function and storing the returned value in data*/
    uint data = decode_lsb_to_int(buff);
    /*storing the data in magic_string_len variable present in structure*/
    decInfo->magic_string_len=data;
    /*returning e_success*/
    return e_success;
}

/*decode_magic_string function*/
Status decode_magic_string(char *mag_str,DecodeInfo *decInfo)
{
    /*declaring the charater array buffer*/
    char buff[8];
    /*loop to decode the magic string */
    for(int i=0;i<decInfo->magic_string_len;i++)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,8,1,decInfo->fptr_stego_image);
        /*calling decode_lsb_to_byte and storing the returned data in mag_str string*/
        mag_str[i]= decode_lsb_to_byte(buff);
    } 
    /*adding \0 at the end of the string*/
    mag_str[decInfo->magic_string_len]='\0';
    /*returning e_success*/
    return e_success;
}

/*decode_secret_file_extn_size function*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,decInfo->fptr_stego_image);
    /*calling decode_lsb_to_int function and storing the returned value in data*/
    uint data = decode_lsb_to_int(buff);
    /*storing the data in file_extn_size variable present in structure*/
    decInfo->file_extn_size=data;
    /*returning e_success*/
    return e_success;
}

/*decode_secret_file_extn function*/
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    /*declaring the charater array buffer*/
    char buff[8];
    /*loop to decode the file extension*/
    for(int i=0;i<decInfo->file_extn_size;i++)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,8,1,decInfo->fptr_stego_image);
        /*calling decode_lsb_to_byte and storing the returned data in file_extn string*/
        file_extn[i]= decode_lsb_to_byte(buff);
    } 
    /*adding \0 at the end of the string*/
    file_extn[decInfo->file_extn_size]='\0';
    /*returning e_success*/
    return e_success;
}

/*decode_secret_file_size function*/
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    /*declaring the charater array buffer*/
    char buff[32];
    /*read the 32 bytes from the source file and store it in buffer*/
    fread(buff,1,32,decInfo->fptr_stego_image);
    /*calling decode_lsb_to_int function and storing the returned value in data*/
    uint data = decode_lsb_to_int(buff);
    /*storing the data in sec_file_size variable present in structure*/
    decInfo->sec_file_size=data;
    /*returning e_success*/
    return e_success;
}

/*decode_secret_file_data function*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    /*declaring the string str with secret file size as size*/
    char str[decInfo->sec_file_size +1];
    /*declaring the charater array buffer*/
    char buff[8];
    /*loop to decode the secret file data */
    for(int i=0;i<decInfo->sec_file_size;i++)
    {
        /*reading 8 bytes from the source file*/
        fread(buff,8,1,decInfo->fptr_stego_image);
        /*calling decode_lsb_to_byte and storing the returned data in str string*/
        str[i]= decode_lsb_to_byte(buff);
    } 
    /*adding \0 at the end of the string*/
    str[decInfo->sec_file_size]='\0';
    /*writing the content of the secret file data to the destination file*/
    fwrite(str,1,decInfo->sec_file_size,decInfo->fptr_output_file);
    /*returning e_success*/
    return e_success;
}

/*decode_lsb_to_int function*/
uint decode_lsb_to_int(char buffer[])
{
    /*declaring variable num and data and initialized with 0*/
    int num=0,data=0;
    /*loop to perform the bitwise operation to extract the lsb of the each buffer data */
    for(int i=31;i>=0;i--)
    {
        /*perform AND operation between each buffer and 1 and left shift the obtained value*/
        data=(buffer[31-i] & 1)<<i;
        /*performing OR operation between num and data*/
        num=(num | data);
    }
    /*returning integer value*/
    return num;
}

char decode_lsb_to_byte(char buffer[])
{
    /*declaring variable num and data and initialized with 0*/
    char num=0,data=0;
    /*loop to perform the bitwise operation to extract the lsb of the each buffer data */
    for(int i=7;i>=0;i--)
    {
        /*perform AND operation between each buffer and 1 and left shift the obtained value*/
        data=(buffer[7-i] & 1)<<i;
        /*performing OR operation between num and data*/
        num=(num | data);
    }
    /*returning character value*/
    return num;

}
