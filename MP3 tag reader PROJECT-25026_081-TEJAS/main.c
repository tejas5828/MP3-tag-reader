/*
Documentation:

Name : Tejas Jaypal Narade

Date : 22/12/2025.

Title : MP3 Tag reader

Description :  
  MP3 tag reader that extracts and displays ID3 metadata such as song title, artist, album, year, and genre. 
  The project uses file handling and binary data processing to parse MP3 frames, improving understanding of 
  low-level file formats and memory handling.

view (-v) : To view song file content 

edit (-e) : To edit song file content 

help (-h) : to seek help for tag information

Features

1)Reads ID3 metadata from MP3 files

2) Displays song title, artist, album, year, genre, and comments

3) Uses binary file handling (fread, fwrite, fseek)

4) Validates MP3 file format before processing

5) User-friendly command-line interface

6) Efficient memory usage with structured data handling

Conclusion :
 This project successfully demonstrates reading and handling MP3 ID3 metadata using the C programming language.
 It enhanced understanding of binary file processing, file I/O operations, and structured data handling, while 
 providing a reliable solution to extract and manage audio file information.


Sample input : Name : song.mp3

Sample output : Name : temp.mp3
*/

#include<stdio.h>
#include<string.h>
#include"type.h"
#include"view.h"
#include"edit.h"

Operationtype check_operation_type(char*);

int main(int argc,char*argv[])
{
    ViewInfo vi_info;
    EditInfo ed_info;
    if (argc>=2)
    {
        if (check_operation_type(argv[1])==s_view)
        {
            if (read_and_validate(argv,&vi_info)==v_success)
            {
                do_view(&vi_info);
                return v_success;
            }
            else
            {
                printf("ERROR: Unable to view song deatils due to Invalid argc\n");
                return v_failure;
            }  
        }  
        else if(check_operation_type(argv[1])==s_edit)
        {
            if (read_and_validatee(argv,&ed_info)==e_success)
            {
                do_edit(&ed_info);
                return e_success;
            }
            else
            {
                return e_failure;
            }   
        }
        else if (check_operation_type(argv[1])==s_help)
        {
            printf("\t\n==> MENU TO EDIT SONG <==\n\n");
            printf("Basic opertions to perform : \n\n");
            printf("-h : To seek help for tag\n");
            printf("-v : To view song tag\n");
            printf("-e : To edit somg tags\n\n");

            printf("Following are some options to edit tags : \n\n");
            printf("-t : Modifies Title \n");
            printf("-A : Modifies Artist name \n");
            printf("-a : Modifies Album name \n");
            printf("-y : Modifies Year \n");
            printf("-m : Modifies Content type tag \n");
            printf("-c : Modifies Composer tag\n\n");
        }
        else
        {
            printf("ERROR: Unsupported operation type\n");
            return e_failure;
        }
        
    }  
    return v_success;
}

Operationtype check_operation_type(char*symbol)
{
  if (strcmp(symbol,"-v")==0)
  {
    return s_view;
  }
  else  if (strcmp(symbol,"-e")==0)
  {
    return s_edit; 
  } 
  else if(strcmp(symbol,"-h")==0)
  {
    return s_help;
  }
  else
  {
    return s_unsupported;
  }
}