#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"view.h"
#include"type.h"

Song read_and_validate(char *argv[],ViewInfo*vi_info)
{
   char*res;
   res=strstr(argv[2],".mp3");
   if (res!=NULL)
   {
    vi_info->song_name=argv[2];
   }
   else
   {
    printf("ERROR: argv[2] Song.mp3 is empty\n");
    return v_failure;
   }
   
   return v_success;
}

Song open_file(ViewInfo *vi_info)
{
  vi_info->fptr_song=fopen(vi_info->song_name,"rb");
  
  if (vi_info->fptr_song == NULL)
  {
    printf("ERROR: song file is empty\n");
    return v_failure;
  }
  
  return v_success;
}

Song skip_header(ViewInfo *vi_info)
{
   if(fseek(vi_info->fptr_song,10,SEEK_SET) == 0)
   {
    //printf("10 bytes Header skipped successfully\n");
    
    /*long pos=ftell(vi_info->fptr_song);
      printf("%ld",pos);*/
     return v_success;
   }
   printf("ERROR : 11 bytes header not skipped\n");
   return v_failure;
}

Song read_tag(ViewInfo *vi_info)
{
    char frame_id[5];
    unsigned char sizebyte[4];
    int frame_size,i;

    for (i=0;i<6;i++)
    {
        if(fread(frame_id,1,4,vi_info->fptr_song)!=4)
        {
            printf("ERROR: NOT READ PROPERLY\n");
            return v_failure;
        }
        frame_id[4]='\0';

        if (frame_id==NULL)
        {
            printf("ERROR: Tag buffer is empty\n");
            return v_failure;
        }
        
        if(fread(sizebyte,1,4,vi_info->fptr_song)!=4)
        {
            printf("ERROR: Tag not readed properly\n");
            return v_failure;
        }
        
        frame_size=((sizebyte[0]<<24) | (sizebyte[1]<<16) | (sizebyte[2]<<8) | (sizebyte[3]));
        
        fseek(vi_info->fptr_song,2,SEEK_CUR);
       
        char *data=malloc(frame_size+1);
        if (!data)
        {
           printf("ERROR: Memory allocation failed\n");
           return v_failure;
        }
        
        fread(data,1,frame_size,vi_info->fptr_song);
        data[frame_size]='\0';

        if (strcmp(frame_id,"TIT2")==0)
        {
            printf("Title         : %s\n",data+1);
        }
        else if (strcmp(frame_id,"TPE1")==0)
        {
            printf("Artist name   : %s\n",data+1);
        }
         else if (strcmp(frame_id,"TALB")==0)
        {
            printf("Album name    : %s\n",data+1);
        }
         else if (strcmp(frame_id,"TYER")==0)
        {
            printf("Year          : %s\n",data+1);
        }
         else if (strcmp(frame_id,"TCON")==0)
        {
            printf("Content type  : %s\n",data+1);
        }
        else if(strcmp(frame_id,"COMM")==0)
        {
            printf("Composer      : %s\n",data+1);
        }
        free(data);
    }
    return v_success;
}
 
Song do_view(ViewInfo *vi_info)
{
    //call function to open file
    if (open_file(vi_info) == v_failure)
    {
        printf("ERROR: Unable to open file\n");
        return v_failure;
    }
    
    if (skip_header(vi_info) == v_failure)
    {
        printf("ERROR: Unable to skip header file\n");
        return v_failure;
    }
    
    if (read_tag(vi_info) == v_failure)
    {
        printf("ERROR: Unable to read the tag\n");
        return v_failure;
    }
       
}