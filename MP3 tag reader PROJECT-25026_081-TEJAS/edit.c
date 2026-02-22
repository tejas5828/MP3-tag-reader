#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"edit.h"
#include"type.h"

Song read_and_validatee(char*argv[],EditInfo*ed_info)     //Reading and validating command line arguments 
{
  if((strcmp(argv[2], "-t")==0) || (strcmp(argv[2],"-A")==0) || (strcmp(argv[2],"-a") == 0) || (strcmp(argv[2],"-y")==0) || (strcmp(argv[2],"-m")==0) || (strcmp(argv[2],"-c")==0 ))
  {
    strcpy(ed_info->option,argv[2]);
  }
  else
  {
    printf("ERROR: ARGV[2] Is Not Matching\n");
    return e_failure;
  } 
    if (argv[3]==NULL || strlen(argv[3])==0)
    {
      printf("ERROR: Argument 3 is empty\n");
      return e_failure;
    }
    ed_info->new_tag_value=argv[3];

    char *res1;
    res1=strstr(argv[4],".mp3");
    if (res1!=NULL)
    {
      ed_info->src_song_name=argv[4];
      return e_success;
    }
    else
    {
      printf("ERROR: argument 4 is empty\n");
      return e_failure;
    }
  
  return e_success;  
}

Song open_filee(EditInfo*ed_info)            //Opening file 
{
  ed_info->fptr_src_song=fopen(ed_info->src_song_name,"rb");
  if(ed_info->fptr_src_song == NULL)
  {
    printf("ERROR: Source song file is empty\n");
    return e_failure;
  }
  ed_info->temp_song_name="temp.mp3";
  ed_info->fptr_temp_song=fopen(ed_info->temp_song_name,"wb");

  if (ed_info->fptr_temp_song == NULL)
  {
    printf("ERROR: Unable to oen temp file\n");
    return e_failure;
  }
  
  return e_success;
}

Song copy_header_bytes(EditInfo*ed_info)
{
  char buff[10];

  if(fread(buff,1,10,ed_info->fptr_src_song)!=10)      //reading 10 bytes of header data from source song and storing to buff 
  {     
    printf("ERROR; Header read failed\n");
    return e_failure;
  }

  if(fwrite(buff,1,10,ed_info->fptr_temp_song) !=10)  //Writing 10 bytes of  that header data stored in buff to temp song file
  {
    printf("ERROR: Header write failed\n");
    return e_failure;
  }

  //long ps=ftell(ed_info->fptr_temp_song);
  //printf("%ld",ps);

  return e_success;
}

Song read_edit_tag(EditInfo *ed_info)
{
  char usr_buff[5] = {0};       //To take i/p from user
  unsigned char sizebytes[4];   //Frame sizebyte
  unsigned char flags[2];       //tag flags
  unsigned int frame_size, tag_size;  
  char frame_id[5] = {0};        //frame id string

  char *option[]={"-t","-A","-a","-y","-m","-c"};
  char *ptr[]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};

  for(int i=0;i<6;i++)
  {
    if(!strcmp(ed_info->option, option[i]))
    {
      strcpy(usr_buff, ptr[i]);
    }
  }

  unsigned char tagsize_bytes[4];
  fseek(ed_info->fptr_src_song, 6, SEEK_SET);          //To set file pointer at 6 th pos in src song
  fread(tagsize_bytes,1,4,ed_info->fptr_src_song);     //read data from src song to  frame sizebyte

  tag_size = (tagsize_bytes[0] & 0x7F) << 21 | (tagsize_bytes[1] & 0x7F) << 14 | (tagsize_bytes[2] & 0x7F) << 7  | (tagsize_bytes[3] & 0x7F); //Converting 

  fseek(ed_info->fptr_src_song, 10, SEEK_SET);

  long tag_end = 10 + tag_size;

  while(ftell(ed_info->fptr_src_song) < tag_end && fread(frame_id,1,4,ed_info->fptr_src_song) == 4)      //Running while loop till source song file  size 
  {
    frame_id[4]='\0';

    if (frame_id[0]==0)
    {
      break;
    }
    
    fread(sizebytes,1,4,ed_info->fptr_src_song);                                            //Reading 4 bytes at a time from source song to frame size byte
    frame_size = (sizebytes[0]<<24)|(sizebytes[1]<<16)|(sizebytes[2]<<8)|sizebytes[3];      //Converting frame size byte little endiance to big endians
    fread(flags,1,2,ed_info->fptr_src_song);                                                //Reading 2 bytes of flag for every iteration

    if (frame_size<=0 || frame_size>tag_end)
    {
      break;
    }
    
    if(!strcmp(frame_id, usr_buff))                                         //Comparing frame id string with user given string or buff
    {
      unsigned int text_size = strlen(ed_info->new_tag_value) + 1;          //Storing length of new tag value + 1 in text_size

      unsigned char new_size_bytes[4] = {(text_size >> 24) & 0xFF,(text_size >> 16) & 0xFF,(text_size >> 8) & 0xFF,text_size & 0xFF};   //Converting data from big endians to little endians 

      fwrite(frame_id,1,4,ed_info->fptr_temp_song);          //Writing frame id data to temp song file 
      fwrite(new_size_bytes,1,4,ed_info->fptr_temp_song);    //Writing new_size _bytes data to temp song file
      fwrite(flags,1,2,ed_info->fptr_temp_song);             //writing 2 bytes of flag in temp song file

      fputc(0x00, ed_info->fptr_temp_song);                                                        //Writes text encoding info  
      fwrite(ed_info->new_tag_value,1,strlen(ed_info->new_tag_value),ed_info->fptr_temp_song);     //Write new tag content in temp song 

      fseek(ed_info->fptr_src_song, frame_size, SEEK_CUR);         //Moving file pointer to frame size value in source song  
    }
    else
    {
      char *buffer = malloc(frame_size);                     //Dyanmically allocating memory to buffer
      fread(buffer,1,frame_size,ed_info->fptr_src_song);     //Reading frame size times from source song to buffer

      fwrite(frame_id,1,4,ed_info->fptr_temp_song);          //Writing 4 bytes from frame id to temp song 4 bytes per iteration
      fwrite(sizebytes,1,4,ed_info->fptr_temp_song);         //Writing 4 byte from size byte buffer to temp song file enery iteration
      fwrite(flags,1,2,ed_info->fptr_temp_song);             //writing 2 bytes of flag to temp file every iteration
      fwrite(buffer,1,frame_size,ed_info->fptr_temp_song);   //Writing 1 byte frame size to temp song every iteration

      free(buffer);
    }
  }

    //Copy remaining song data as it is
    int ch;
    while((ch = fgetc(ed_info->fptr_src_song)) != EOF)
    {
      fputc(ch, ed_info->fptr_temp_song);
    }
      //long f=ftell(ed_info->fptr_temp_song);
      //printf("\nTEMP : %ld\n",f);
    return e_success;
}


Song do_edit(EditInfo*ed_info)
{
  if(open_filee(ed_info) == e_failure)
  {
    printf("ERROR: In opening file\n");
    return e_failure;
  }

  if (copy_header_bytes(ed_info) == e_failure)
  {
    printf("ERROR: In copying bytes\n");
    return e_failure;
  }
  
  if (read_edit_tag(ed_info) == e_failure)
  {
    printf("ERROR: In read and edit tag\n");
    return e_failure;
  }
  return e_success;
}