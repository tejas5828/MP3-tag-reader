#ifndef VIEW_H
#define VVIEW_H

#include<stdio.h>
#include"type.h"


typedef struct 
{
   char *song_name;  //To store song name
   FILE*fptr_song;   //To store address of song

}ViewInfo;

Song read_and_validate(char*argv[],ViewInfo*vi_info);
Song do_view(ViewInfo *vi_info);


#endif