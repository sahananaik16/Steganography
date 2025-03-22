#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>

/*main function*/
int main(int argc,char *argv[])
{
    /*declaring encInfo variable of EncodeInfo structure type */
    EncodeInfo encInfo;
    /*declaring decInfo variable of DecodeInfo structure type */
    DecodeInfo decInfo;
    uint img_size;

    /*calling check_operation_type function*/
    OperationType ret= check_operation_type(argc,argv);
    /*condition to check the return value from the check_operation_type function*/
    /*condition to check whether the return value is e_encode*/
    if(ret==e_encode)
    {
        /*calling read_and_validate_encode_args function*/
        Status en=read_and_validate_encode_args(argc,argv, &encInfo);
        /*condition to check return value from read_and_validate_encode_args function*/
        if(en==e_success)
        {
            /*if return value is e_success calling do_encoding function*/
            Status e= do_encoding(&encInfo);
            /*condition to check return value from do_encoding function*/
            if(e==e_failure)
            {
                /*if return value is e_failure printing error message and return e_failure*/
                printf("\033[1;31m");
                printf("Encoding failed");
                printf("\033[0m");
                return e_failure;
            }
        }
    }
    /*condition to check whether the return value is e_decode*/
    else if(ret==e_decode)
    {
        /*calling read_and_validate_decode_args function*/
        Status de = read_and_validate_decode_args(argc,argv,&decInfo);
        /*condition to check return value from read_and_validate_decode_args function*/
        if(de==e_success)
        {
            /*if return value is e_success calling do_decoding function*/
            Status d= do_decoding(&decInfo);
            /*condition to check return value from do_decoding function*/
            if(d==e_failure)
            {
                /*if return value is e_failure printing error message and return e_failure*/
                printf("\033[1;31m");
                printf("Decoding failed");
                printf("\033[0m");
                return e_failure;
            }
        }
    }
    /*if the return value is other than e_encode or e_decode printing error message*/
    else
    {
        printf("\033[1;31m");
        printf("unsupported");
        printf("\033[0m");
    }
    return 0;
}


/*check_operation_type function*/
OperationType check_operation_type(int argc,char *argv[])
{
    /*condition to check whethervthe argc count is 0 or not*/
    if(argc!=0)
    {
        /*condition to check presence of -e which resembles encoding*/
        if(!strcmp(argv[1],"-e"))
        {
            /*if -e present return e_encode*/
            return e_encode;
        }
        /*condition to check presence of -d which resembles decoding*/
        else if(!strcmp(argv[1],"-d"))
        {
            /*if -d present return e_decode*/
            return e_decode;
        }
        /*if other arguments other than -e or -d is present printing error message and returning e_unsupported*/
        else
        {
            printf("\033[1;31m");
            printf("Invalid arguments!\ngive input\n ./a.out -e .... for encoding\n ./a.out -d ..... for decoding");
            printf("\033[0m");
            return e_unsupported;
        }
    }
    /*if no argument is mentioned printing error message and returning e_unsupported*/
    printf("\033[1;31m");
    printf("No operation specified!\n");
    printf("\033[0m");
    return e_unsupported;
}


/*read_and_validate_encode_args function*/
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)
{
    /*condition to check whether argc count is 4 or 5*/
    if(argc==4 || argc==5)
    {
        /*condition to check whether file mentioned in argv[2] is bmp file or not*/
        if(strstr(argv[2],".bmp")==NULL)
        {
            /*if bmp file is not mentioned printing error message and returning e_failure*/
            printf("\033[1;31m");
            printf("Invalid bmp file name!!");
            printf("\033[0m");
            return e_failure;
        }
        /*condition to check whether file mentioned in argv[3] is txt or c file or not*/
        if((strstr(argv[3],".txt")==NULL) && (strstr(argv[3],".c")==NULL))
        {
            /*if txt or c file is not mentioned printing error message and returning e_failure*/
            printf("\033[1;31m");
            printf("Invalid extension!!File should have .txt or .c extension");
            printf("\033[0m");
            return e_failure;
        }
        /*condition to check whether argument 5 is present or not*/
        if (argc == 5)
        {
            /*condition to check whether file mentioned in argv[4] is bmp file or not*/
            if (strstr(argv[4], ".bmp") == NULL)
            {
                /*if bmp file is not mentioned printing error message and returning e_failure*/
                printf("\033[1;31m");
                printf("Invalid bmp file name!!\n");
                printf("\033[0m");
                return e_failure;
            }
            /*storing the address of agrv[5]*/
            encInfo->stego_image_fname=argv[4];
        }
        else
        {
            /*if 5th argument is not specified storing the default name*/
            char str[]="stego.bmp";
            /*storing the address of default file name*/
            encInfo->stego_image_fname=str;
        }
        /*storing the address of source image name*/
        encInfo->src_image_fname=argv[2];
        /*storing the address of secret file name*/
        encInfo->secret_fname=argv[3];
        printf("\033[1;35m");
        printf("Encoding CLA data copied\n");
        printf("\033[0m");
        /*returning e_success*/
        return e_success;
    }
    /*if no of arguments in CLA is other than 4 or 5 printing the error message and returning e_failure*/
    else
    {
        printf("\033[1;31m");
        printf("Invalid no.of arguments!!\nCLA should be ./a.out -e .bmp file .c.txt file .bmp file(optional)\n");
        printf("\033[0m");
        return e_failure;
    }
}

/*read_and_validate_decode_args function*/
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
    /*condition to check whether argc count is 3 or 4*/
    if(argc==3 || argc==4)
    {
        /*condition to check whether file mentioned in argv[2] is bmp file or not*/
        if(strstr(argv[2],".bmp")==NULL)
        {
            /*if bmp file is not mentioned printing error message and returning e_failure*/
            printf("\033[1;31m");
            printf("Invalid bmp file name!!");
            printf("\033[0m");
            return e_failure;
        }
        /*condition to check whether argument 4 is NULL or not*/
        if(argv[3]!=NULL)
        {
            /*declaring string str*/
            char str[50];
            /*extrcting the output name given from the user, without extension and storing it*/
            for(int i=0;(argv[3])[i]!='.';i++)
            {
                str[i]=(argv[3])[i];
            }
            /*storing the extracted file name*/
            strcpy(decInfo->output_fname,str);

        }
        else
        {
            /*if 4th argument is not specified storing the default name*/
            char str2[50]="output";
            strcpy(decInfo->output_fname,str2);
        }
        /*storing the address of source file name*/
        decInfo->stego_image_fname=argv[2];
        printf("\033[1;35m");
        printf("Decoding CLA data copied\n");
        printf("\033[0m");
        /*returning e_success*/
        return e_success;
    }
    else
    {
        /*if no of arguments in CLA is other than 4 or 5 printing the error message and returning e_failure*/
        printf("\033[1;31m");
        printf("Invalid no.of arguments!!\nCLA should be ./a.out -d .bmp file .c.txt file(optional)\n");
        printf("\033[0m");
        return e_failure;
    }
}
