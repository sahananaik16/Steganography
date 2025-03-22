#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types


typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint file_extn_size;
    uint sec_file_size;
    uint magic_string_len;

    /* Output File Info */
    char output_fname[50];
    FILE *fptr_output_file;

} DecodeInfo;



/* Read and validate decode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_defiles(DecodeInfo *decInfo);

/* skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/* decode Magic String length */
Status decode_magic_string_size(DecodeInfo *decInfo);

/* decode Magic String */
Status decode_magic_string(char *mag_str,DecodeInfo *decInfo);

/* decode secret file extenstion length */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*decode lsb to byte*/
char decode_lsb_to_byte(char buffer[]);

/*decode lsb to int*/
uint decode_lsb_to_int(char buffer[]);

#endif
