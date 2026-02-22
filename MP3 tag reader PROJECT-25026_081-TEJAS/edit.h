#ifndef EDIT_H
#define EDIT_H

#include<stdio.h>
#include"type.h"

typedef struct
{
    char *src_song_name;     //To store name of source song
    FILE*fptr_src_song;      //To store address of source song

    char option[3];
    char *new_tag_value;
    
    char*temp_song_name;     //To store name of destination song
    FILE*fptr_temp_song;     //To store address of temp song

}EditInfo;

Song read_and_validatee(char *argv[],EditInfo *ed_info);
Song do_edit(EditInfo*ed_info);
Song open_filee(EditInfo*ed_info);

#endif