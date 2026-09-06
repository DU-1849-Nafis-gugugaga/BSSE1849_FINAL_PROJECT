#include <stdio.h>
#include <stdlib.h>
#include <iup.h>
#include <string.h>
#pragma pack(push,1)
struct header{
      char signature[2];
      unsigned int file_size;
      unsigned short int reserved_1;
      unsigned short int reserved_2;
      unsigned int offset;
};
struct info_header{
      unsigned int size;
      unsigned int width;
      unsigned int hight;
      unsigned short int planes;
      unsigned short int bits_per_pixel;
      int compression;
      int image_size;
      int X_pixels_per_M;
      int Y_pixels_per_M;
      int colours_used;
      int important_colours;
};
struct colour_table{
      unsigned char blue;
      unsigned char green;
      unsigned char red;
      unsigned char reserved;
};
struct pixel_data{
      unsigned char blue;
      unsigned char green;
      unsigned char red;
};
struct image_data{
      struct header header_data;
      struct info_header info_header_data;
      struct colour_table colour_table_data;
      struct pixel_data *data;
};
#pragma pack(pop)
struct parsed_data{
      struct pixel_data **data;
};
struct dim_data{
      int hight;
      int width;
};
struct iup_data{
      int history;
      struct dim_data *dim_list;
      struct parsed_data *edit_list;
      struct pixel_data *temp;
      char **argv;
      struct image_data *selected_image_data;
};
int l;
int parse_image(char *file_name,FILE *bmp_reader,struct image_data *selected_image_data, struct parsed_data edit_list[],struct dim_data dim_list[]);
void flaten(struct parsed_data edit_list[],int history,struct pixel_data *temp,int width,int hight);
void render_image(struct pixel_data *temp,int width,int hight);
void v_flip(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void h_flip(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void colour_correction(struct parsed_data edit_list[],int width,int hight);
void grayscale(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void brightness(struct parsed_data edit_list[],int width,int hight,int history,int offset,struct dim_data dim_list[]);
void inversion(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void rotate(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
int crop(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[],int x,int y,int w,int h);
void blur(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void copy(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
void sharpen(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]);
int lim(int c);
void screen(struct pixel_data *temp,struct parsed_data edit_list[],struct dim_data dim_list[],char *argv[],int *history,struct image_data *selected_image_data);
int function_assign(Ihandle *button);
void open_image(Ihandle *button,struct iup_data *data);
void main(int argc,char *argv[]){
    FILE *bmp_reader;
    struct image_data selected_image_data;
    struct parsed_data edit_list[10];
    struct dim_data dim_list[10];
    /*int padding=parse_image(argv[1],bmp_reader,&selected_image_data,edit_list);
    dim_list[0].width=selected_image_data.info_header_data.width;
    dim_list[0].hight=selected_image_data.info_header_data.hight;*/
    dim_list[0].width=512;
    dim_list[0].hight=512;
    struct pixel_data *temp=malloc(sizeof(struct pixel_data)*dim_list[0].width*dim_list[0].hight);
    //v_flip(edit_list,dim_list[0].width,dim_list[0].hight,0,dim_list);
    //colour_correction(edit_list,dim_list[0].width,dim_list[0].hight);
    //flaten(edit_list,0,temp,dim_list[0].width,dim_list[0].hight);
    IupOpen(NULL,NULL);
    int history=0;
    l=history;
    screen(temp,edit_list,dim_list,argv,&history,&selected_image_data);
    IupClose();
    free(temp);
    for(int i=0;i<10;i++){
       for(int j=0;j<dim_list[i].hight;j++){
          free(edit_list[i].data[j]);
        }
    }
    return;
}
void screen(struct pixel_data *temp,struct parsed_data edit_list[],struct dim_data dim_list[],char *argv[],int *history,struct image_data *selected_image_data){
    static struct iup_data data;
    data.history=*history;
    data.temp=temp;
    data.edit_list=edit_list;
    data.dim_list=dim_list;
    data.argv=argv;
    data.selected_image_data=selected_image_data;
    
    Ihandle *image = IupImageRGB(data.dim_list[data.history].width, dim_list[data.history].hight,data.temp);
    Ihandle *img_window=IupLabel(NULL);
    IupSetAttributeHandle(img_window,"IMAGE",image);
    Ihandle *window=IupDialog(NULL);
    
    
    
    Ihandle *open_file_button=IupButton("Open image",NULL);
    Ihandle *apply_grayscale_button=IupButton("Apply Grayscale",NULL);
    Ihandle *brightness_button=IupButton("Brightness Adjustment",NULL);
    Ihandle *invert_button=IupButton("Invert selected image",NULL);
    Ihandle *h_flip_button=IupButton("Flip Horizontally",NULL);
    Ihandle *v_flip_button=IupButton("Flip Vertically",NULL);
    Ihandle *rotate_button=IupButton("Rotate image by 90 degree",NULL);
    Ihandle *crop_button=IupButton("Crop image",NULL);
    Ihandle *blur_button=IupButton("Blur image",NULL);
    Ihandle *sharpness_button=IupButton("Increase Sharpness of the image",NULL);
    Ihandle *undo_button=IupButton("Undo last change",NULL);
    Ihandle *save_button=IupButton("Save image",NULL);
    
    
    
    IupSetAttribute(open_file_button,"data",(char *)&data);
    IupSetAttribute(apply_grayscale_button,"data",(char *)&data);
    IupSetAttribute(brightness_button,"data",(char *)&data);
    IupSetAttribute(invert_button,"data",(char *)&data);
    IupSetAttribute(h_flip_button,"data",(char *)&data);
    IupSetAttribute(v_flip_button,"data",(char *)&data);
    IupSetAttribute(rotate_button,"data",(char *)&data);
    IupSetAttribute(crop_button,"data",(char *)&data);
    IupSetAttribute(blur_button,"data",(char *)&data);
    IupSetAttribute(sharpness_button,"data",(char *)&data);
    IupSetAttribute(undo_button,"data",(char *)&data);
    IupSetAttribute(save_button,"data",(char *)&data);
    
    
    
    IupSetAttribute(open_file_button,"function","Open image");
    IupSetAttribute(apply_grayscale_button,"function","Apply Grayscale");
    IupSetAttribute(brightness_button,"function","Brightness Adjustment");
    IupSetAttribute(invert_button,"function","Invert selected image");
    IupSetAttribute(h_flip_button,"function","Flip Horizontally");
    IupSetAttribute(v_flip_button,"function","Flip Vertically");
    IupSetAttribute(rotate_button,"function","Rotate image by 90 degree");
    IupSetAttribute(crop_button,"function","Crop image");
    IupSetAttribute(blur_button,"function","Blur image");
    IupSetAttribute(sharpness_button,"function","Increase Sharpness of the image");
    IupSetAttribute(undo_button,"function","Undo last change");
    IupSetAttribute(save_button,"function","Save image");
    
    
    IupSetAttributeHandle(open_file_button,"stored_image",image);
    IupSetAttributeHandle(apply_grayscale_button,"stored_image",image);
    IupSetAttributeHandle(brightness_button,"stored_image",image);
    IupSetAttributeHandle(invert_button,"stored_image",image);
    IupSetAttributeHandle(h_flip_button,"stored_image",image);
    IupSetAttributeHandle(v_flip_button,"stored_image",image);
    IupSetAttributeHandle(rotate_button,"stored_image",image);
    IupSetAttributeHandle(crop_button,"stored_image",image);
    IupSetAttributeHandle(blur_button,"stored_image",image);
    IupSetAttributeHandle(sharpness_button,"stored_image",image);
    IupSetAttributeHandle(undo_button,"stored_image",image);
    IupSetAttributeHandle(save_button,"stored_image",image);
    
    
    IupSetAttributeHandle(open_file_button,"window",window);
    IupSetAttributeHandle(apply_grayscale_button,"window",window);
    IupSetAttributeHandle(brightness_button,"window",window);
    IupSetAttributeHandle(invert_button,"window",window);
    IupSetAttributeHandle(h_flip_button,"window",window);
    IupSetAttributeHandle(v_flip_button,"window",window);
    IupSetAttributeHandle(rotate_button,"window",window);
    IupSetAttributeHandle(crop_button,"window",window);
    IupSetAttributeHandle(blur_button,"window",window);
    IupSetAttributeHandle(sharpness_button,"window",window);
    IupSetAttributeHandle(undo_button,"window",window);
    IupSetAttributeHandle(save_button,"window",window);
    
    
    
    IupSetCallback(open_file_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(apply_grayscale_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(brightness_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(invert_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(h_flip_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(v_flip_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(rotate_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(crop_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(blur_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(sharpness_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(undo_button,"ACTION",(Icallback)function_assign);
    IupSetCallback(save_button,"ACTION",(Icallback)function_assign);
    
    IupSetAttributeHandle(open_file_button,"img_window",img_window);
    IupSetAttributeHandle(apply_grayscale_button,"img_window",img_window);
    IupSetAttributeHandle(brightness_button,"img_window",img_window);
    IupSetAttributeHandle(invert_button,"img_window",img_window);
    IupSetAttributeHandle(h_flip_button,"img_window",img_window);
    IupSetAttributeHandle(v_flip_button,"img_window",img_window);
    IupSetAttributeHandle(rotate_button,"img_window",img_window);
    IupSetAttributeHandle(crop_button,"img_window",img_window);
    IupSetAttributeHandle(blur_button,"img_window",img_window);
    IupSetAttributeHandle(sharpness_button,"img_window",img_window);
    IupSetAttributeHandle(undo_button,"img_window",img_window);
    IupSetAttributeHandle(save_button,"img_window",img_window);
    
    
    
    
    Ihandle *control_panel=IupVbox(open_file_button,apply_grayscale_button,brightness_button,invert_button,h_flip_button,v_flip_button,rotate_button,crop_button,blur_button,sharpness_button,undo_button,save_button,NULL);

    IupSetAttributeHandle(open_file_button,"control_panel",control_panel);
    IupSetAttributeHandle(apply_grayscale_button,"control_panel",control_panel);
    IupSetAttributeHandle(brightness_button,"control_panel",control_panel);
    IupSetAttributeHandle(invert_button,"control_panel",control_panel);
    IupSetAttributeHandle(h_flip_button,"control_panel",control_panel);
    IupSetAttributeHandle(v_flip_button,"control_panel",control_panel);
    IupSetAttributeHandle(rotate_button,"control_panel",control_panel);
    IupSetAttributeHandle(crop_button,"control_panel",control_panel);
    IupSetAttributeHandle(blur_button,"control_panel",control_panel);
    IupSetAttributeHandle(sharpness_button,"control_panel",control_panel);
    IupSetAttributeHandle(undo_button,"control_panel",control_panel);
    IupSetAttributeHandle(save_button,"control_panel",control_panel);

    Ihandle *img_scroll=IupScrollBox(img_window);
    IupSetAttribute(img_scroll,"SCROLLBAR","YES");
    IupSetAttribute(img_scroll,"MAXSIZE","800x800");

    IupSetAttributeHandle(open_file_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(apply_grayscale_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(brightness_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(invert_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(h_flip_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(v_flip_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(rotate_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(crop_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(blur_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(sharpness_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(undo_button,"img_scroll",img_scroll);
    IupSetAttributeHandle(save_button,"img_scroll",img_scroll);
    
    Ihandle *ui=IupHbox(img_scroll,control_panel,NULL);
    IupSetAttribute(ui,"GAP","20");
    IupSetAttribute(ui,"MARGIN","20x20");
    window=IupDialog(ui);
    printf("\n %p---%p---%p",img_window,window,image);
    fflush(stdout);
    IupSetAttribute(window,"TITLE","BSSE 1849 Final Project");
    IupShowXY(window,IUP_CENTER,IUP_CENTER);
    
    
    IupMainLoop();
    IupClose();
}
int parse_image(char *file_name,FILE *bmp_reader,struct image_data *selected_image_data, struct parsed_data edit_list[],struct dim_data dim_list[]){
    bmp_reader=fopen(file_name,"rb");
    if(bmp_reader==NULL){
      return -1;
    }
    fread(&selected_image_data->header_data,sizeof(selected_image_data->header_data),1,bmp_reader);
    fread(&selected_image_data->info_header_data,sizeof(selected_image_data->info_header_data),1,bmp_reader);
    if(selected_image_data->header_data.signature[0]!='B' ||
       selected_image_data->header_data.signature[1]!='M' ||
       selected_image_data->info_header_data.bits_per_pixel!=24){
      fclose(bmp_reader);
      return -1;
    }
    fseek(bmp_reader,selected_image_data->header_data.offset,SEEK_SET);
    int padding=(4-(selected_image_data->info_header_data.width*3)%4)%4;
    edit_list[0].data=malloc(selected_image_data->info_header_data.hight*sizeof(struct pixel_data *));
    for(int i=0;i<selected_image_data->info_header_data.hight;i++){
       edit_list[0].data[i]=malloc(selected_image_data->info_header_data.width*sizeof(struct pixel_data));
       fread(edit_list[0].data[i],selected_image_data->info_header_data.width*sizeof(struct pixel_data),1,bmp_reader);
       fseek(bmp_reader,padding,SEEK_CUR);
       
    }
    dim_list[0].width=selected_image_data->info_header_data.width;
    dim_list[0].hight=selected_image_data->info_header_data.hight;
    fclose(bmp_reader);
    return padding;
}
void flaten(struct parsed_data edit_list[],int history,struct pixel_data *temp,int width,int hight){
    int k=0;
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
          temp[k]=edit_list[history].data[i][j];
          k++;
        }
    }
}
void render_image(struct pixel_data *temp,int width,int hight){
    IupOpen(NULL,NULL);
    Ihandle *window=IupDialog(NULL);
    Ihandle *image=IupImageRGB(width,hight,temp);
    Ihandle *img_window=IupLabel(NULL);
    IupSetAttributeHandle(img_window,"IMAGE",image);
    
    IupSetAttribute(window,"TITLE","Nafis's Image Editor");
    IupSetAttribute(window,"SIZE",NULL);
    IupSetAttribute(img_window,"SIZE",NULL);
    IupMap(window);
    IupShowXY(window,IUP_CENTER,IUP_CENTER);
    printf("\n %p---%p---%p",img_window,window,image);
    
    IupMainLoop();
    IupClose();
}
void colour_correction(struct parsed_data edit_list[],int width,int hight){
    unsigned char temp;
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
           temp=edit_list[0].data[i][j].blue;
           edit_list[0].data[i][j].blue=edit_list[0].data[i][j].red;
           edit_list[0].data[i][j].red=temp;
         }
    }
    
    return;
}
void v_flip(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    unsigned char temp;
    for(int i=0;hight-i>i;i++){
       for(int j=0;j<width;j++){
       
          temp=edit_list[history].data[i][j].blue;
          edit_list[history].data[i][j].blue=edit_list[history].data[hight-i-1][j].blue;
          edit_list[history].data[hight-i-1][j].blue=temp;
       
          temp=edit_list[history].data[i][j].green;
          edit_list[history].data[i][j].green=edit_list[history].data[hight-i-1][j].green;
          edit_list[history].data[hight-i-1][j].green=temp;
          
          temp=edit_list[history].data[i][j].red;
          edit_list[history].data[i][j].red=edit_list[history].data[hight-i-1][j].red;
          edit_list[history].data[hight-i-1][j].red=temp;
        }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;
}
void h_flip(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    unsigned char temp;
    for(int i=0;width-i>i;i++){
       for(int j=0;j<hight;j++){
       
          temp=edit_list[history].data[j][i].blue;
          edit_list[history].data[j][i].blue=edit_list[history].data[j][width-i-1].blue;
          edit_list[history].data[j][width-i-1].blue=temp;
       
          temp=edit_list[history].data[j][i].green;
          edit_list[history].data[j][i].green=edit_list[history].data[j][width-i-1].green;
          edit_list[history].data[j][width-i-1].green=temp;
          
          temp=edit_list[history].data[j][i].red;
          edit_list[history].data[j][i].red=edit_list[history].data[j][width-i-1].red;
          edit_list[history].data[j][width-i-1].red=temp;
        }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;
}
void grayscale(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    float gray;
    //gray=0.299R+0.587G+0.114B
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
          gray=0.299*edit_list[history].data[i][j].red+0.587*edit_list[history].data[i][j].green+0.114*edit_list[history].data[i][j].blue;
          edit_list[history].data[i][j].red=gray;
          edit_list[history].data[i][j].green=gray;
          edit_list[history].data[i][j].blue=gray;
        }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;
    dim_list[history].width=width;
    dim_list[history].hight=hight;
}
void brightness(struct parsed_data edit_list[],int width,int hight,int history,int offset,struct dim_data dim_list[]){
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
          if(edit_list[history].data[i][j].red+offset>255){
            edit_list[history].data[i][j].red=255;
          }
          else if(edit_list[history].data[i][j].red+offset<0){
              edit_list[history].data[i][j].red=0;
            }
          else{
              edit_list[history].data[i][j].red=edit_list[history].data[i][j].red+offset;
           }
          
          if(edit_list[history].data[i][j].green+offset>255){
            edit_list[history].data[i][j].green=255;
          }
          else if(edit_list[history].data[i][j].green+offset<0){
              edit_list[history].data[i][j].green=0;
            }
          else{
              edit_list[history].data[i][j].green=edit_list[history].data[i][j].green+offset;
           }
          if(edit_list[history].data[i][j].blue+offset>255){
            edit_list[history].data[i][j].blue=255;
          }
          else if(edit_list[history].data[i][j].blue+offset<0){
              edit_list[history].data[i][j].blue=0;
            }
          else{
              edit_list[history].data[i][j].blue=edit_list[history].data[i][j].blue+offset;
           }
          
          
          
          
        }
      }
    
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;
}
void inversion(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
          edit_list[history].data[i][j].red=255-edit_list[history].data[i][j].red;
          edit_list[history].data[i][j].green=255-edit_list[history].data[i][j].green;
          edit_list[history].data[i][j].blue=255-edit_list[history].data[i][j].blue;
        }
    }

    
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;
}
void rotate(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    edit_list[history].data=malloc(width*sizeof(struct pixel_data *));
    for(int i=0;i<width;i++){
       edit_list[history].data[i]=malloc(hight*sizeof(struct pixel_data));
    }
    for(int i=0;i<width;i++){
       for(int j=0;j<hight;j++){
          edit_list[history].data[i][j].red=edit_list[history-1].data[hight-j-1][i].red;
          edit_list[history].data[i][j].green=edit_list[history-1].data[hight-j-1][i].green;
          edit_list[history].data[i][j].blue=edit_list[history-1].data[hight-j-1][i].blue;
        }
    }
    dim_list[history].width=hight;
    dim_list[history].hight=width;
}
int crop(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[],int x,int y,int w,int h){
    if(x+w>width){printf("\nInvalid crop parameter.Width exceeded.");return 0;}
    if(y+h>hight){printf("\nInvalid crop parameter.Hight exceeded.");return 0;}
    edit_list[history].data=malloc(h*sizeof(struct pixel_data *));
    for(int i=0;i<h;i++){
       edit_list[history].data[i]=malloc(w*sizeof(struct pixel_data));
    }
    for(int i=0;i<h;i++){
       for(int j=0;j<w;j++){
          edit_list[history].data[i][j].red=edit_list[history-1].data[y+i][x+j].red;
          edit_list[history].data[i][j].green=edit_list[history-1].data[y+i][x+j].green;
          edit_list[history].data[i][j].blue=edit_list[history-1].data[y+i][x+j].blue;
          //printf(" %hu--%hu--%hu ",edit_list[history].data[i][j].red,edit_list[history].data[i][j].green,edit_list[history].data[i][j].blue);
        }
    }
    dim_list[history].width=w;
    dim_list[history].hight=h;
    return 1;
}
void copy(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    edit_list[history].data=malloc(hight*sizeof(struct pixel_data *));
    for(int i=0;i<hight;i++){
       edit_list[history].data[i]=malloc(width*sizeof(struct pixel_data));
     }
    for(int i=0;i<hight;i++){
       for(int j=0;j<width;j++){
          edit_list[history].data[i][j].red=edit_list[history-1].data[i][j].red;
          edit_list[history].data[i][j].green=edit_list[history-1].data[i][j].green;
          edit_list[history].data[i][j].blue=edit_list[history-1].data[i][j].blue;
        }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
    return;



}
void blur(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    for(int i=0;i<hight-2;i++){
       for(int j=0;j<width-2;j++){
          int red=0;
          int green=0;
          int blue=0;
          for(int k=0;k<3;k++){
             for(int l=0;l<3;l++){
                red=red+edit_list[history-1].data[i+k][j+l].red;
                green=green+edit_list[history-1].data[i+k][j+l].green;
                blue=blue+edit_list[history-1].data[i+k][j+l].blue;
              }
          }
          edit_list[history].data[i+1][j+1].red=red/9;
          edit_list[history].data[i+1][j+1].green=green/9;
          edit_list[history].data[i+1][j+1].blue=blue/9;
      }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
}
void sharpen(struct parsed_data edit_list[],int width,int hight,int history,struct dim_data dim_list[]){
    for(int i=0;i<hight-2;i++){
       for(int j=0;j<width-2;j++){
          int red=0;
          int green=0;
          int blue=0;
          int mat=1;
          for(int k=0;k<3;k++){
             for(int l=0;l<3;l++){
                if(mat==5){
                  red=red+edit_list[history-1].data[i+k][j+l].red*5;
                  green=green+edit_list[history-1].data[i+k][j+l].green*5;
                  blue=blue+edit_list[history-1].data[i+k][j+l].blue*5;
                }
                else if(mat%2==0){
                       red=red-edit_list[history-1].data[i+k][j+l].red;
                       green=green-edit_list[history-1].data[i+k][j+l].green;
                       blue=blue-edit_list[history-1].data[i+k][j+l].blue;
                  }
                mat++;
             }
            }
          red=lim(red);
          green=lim(green);
          blue=lim(blue);
          edit_list[history].data[i+1][j+1].red=red;
          edit_list[history].data[i+1][j+1].green=green;
          edit_list[history].data[i+1][j+1].blue=blue;
      }
    }
    dim_list[history].width=width;
    dim_list[history].hight=hight;
}
int lim(int c){
   if(c<=0){return 0;}
   else if(c>=255){return 255;}
   else{return c;}
}
int function_assign(Ihandle *button){
   char *function=IupGetAttribute(button,"function");
   struct iup_data *data=(struct iup_data *)IupGetAttribute(button,"data");
   
   if(strcmp(function,"Open image")==0){
     open_image(button,data);
   }
   else if(strcmp(function,"Apply Grayscale")==0){
      data->history++;
      copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
      grayscale(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
      flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
      
      Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
      Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
      Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
      Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
      Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
      Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
      Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
      IupSetAttributeHandle(img_window,"IMAGE",image);
    
      printf("\n %p---%p---%p",img_window,window,image);
      //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
      printf("===============================");
      IupSetAttribute(window,"SIZE",NULL);
      IupSetAttribute(img_window,"SIZE",NULL);
      IupSetAttribute(control_panel,"SIZE",NULL);
      IupSetAttribute(ui,"SIZE",NULL);
      printf("-------------------------------");
      IupRefresh(window);
      {
        int _w = IupGetInt(window, "NATURALWIDTH");
        int _h = IupGetInt(window, "NATURALHEIGHT");
        IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
        IupRefresh(window);
      }
      IupRedraw(window, 1);
      return;

    }
   else if(strcmp(function,"Brightness Adjustment")==0){
          int offset=10;
          int decision=IupGetParam("Brightness Adjustment",NULL,NULL,
                                    "Offset (-255 to 255): %i\n",
                                    &offset);
          if(!decision){ return; }

          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          brightness(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,offset,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;

    }
   else if(strcmp(function,"Invert selected image")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          inversion(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
   }
   else if(strcmp(function,"Flip Horizontally")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          h_flip(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
   }
   else if(strcmp(function,"Flip Vertically")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          v_flip(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
      
   }
   else if(strcmp(function,"Rotate image by 90 degree")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          rotate(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
   }
   else if(strcmp(function,"Crop image")==0){
          int x=200;
          int y=200;
          int w=100;
          int h=100;
          int decision=IupGetParam("Crop image",NULL,NULL,
                                    "X: %i\n"
                                    "Y: %i\n"
                                    "Width: %i\n"
                                    "Height: %i\n",
                                    &x,&y,&w,&h);
          if(!decision){ return; }

          data->history++;
          
          
          
          crop(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list,x,y,w,h);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
   }
   else if(strcmp(function,"Blur image")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          blur(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
   }
   else if(strcmp(function,"Increase Sharpness of the image")==0){
          data->history++;
          copy(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          sharpen(data->edit_list,data->dim_list[data->history-1].width,data->dim_list[data->history-1].hight,data->history,data->dim_list);
          flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
          
          
          Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
          Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
          Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
          Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
          Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
          Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
          Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
          IupSetAttributeHandle(img_window,"IMAGE",image);
    
          printf("\n %p---%p---%p",img_window,window,image);
          //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
          printf("===============================");
          IupSetAttribute(window,"SIZE",NULL);
          IupSetAttribute(img_window,"SIZE",NULL);
          IupSetAttribute(control_panel,"SIZE",NULL);
          IupSetAttribute(ui,"SIZE",NULL);
          printf("-------------------------------");
          IupRefresh(window);
          {
            int _w = IupGetInt(window, "NATURALWIDTH");
            int _h = IupGetInt(window, "NATURALHEIGHT");
            IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
            IupRefresh(window);
          }
          IupRedraw(window, 1);
          return;
      
   }
   else if(strcmp(function,"Undo last change")==0){
      data->history--;
      flaten(data->edit_list,data->history,data->temp,data->dim_list[data->history].width,data->dim_list[data->history].hight);
      Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
      Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
      Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
      Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
      Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
      Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
      Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
      IupSetAttributeHandle(img_window,"IMAGE",image);
    
      printf("\n %p---%p---%p",img_window,window,image);
      //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
      printf("===============================");
      IupSetAttribute(window,"SIZE",NULL);
      IupSetAttribute(img_window,"SIZE",NULL);
      IupSetAttribute(control_panel,"SIZE",NULL);
      IupSetAttribute(ui,"SIZE",NULL);
      printf("-------------------------------");
      IupRefresh(window);
      {
        int _w = IupGetInt(window, "NATURALWIDTH");
        int _h = IupGetInt(window, "NATURALHEIGHT");
        IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
        IupRefresh(window);
      }
      IupRedraw(window, 1);
      return;
      
      
      
   }
   else if(strcmp(function,"Save image")==0){
      char file_name[256]="output.bmp";
      int decision=IupGetParam("Save image",NULL,NULL,
                                "File name: %s\n",
                                file_name);
      if(!decision){ return; }

      struct pixel_data **temp_data;
      temp_data=malloc(sizeof(struct pixel_data *)*data->dim_list[data->history].hight);
      for(int i=0;i<data->dim_list[data->history].hight;i++){
         temp_data[i]=malloc(sizeof(struct pixel_data)*data->dim_list[data->history].width);
      }
      for(int i=0;i<data->dim_list[data->history].hight;i++){
         for(int j=0;j<data->dim_list[data->history].width;j++){
            temp_data[i][j]=data->edit_list[data->history].data[i][j];
          }
      }
      unsigned char t;
      for(int i=0;i<data->dim_list[data->history].hight;i++){
         for(int j=0;j<data->dim_list[data->history].width;j++){
             t=temp_data[i][j].blue;
             temp_data[i][j].blue=temp_data[i][j].red;
             temp_data[i][j].red=t;
           }
       }
       
      struct pixel_data *saver=malloc(sizeof(struct pixel_data)*data->dim_list[data->history].hight*data->dim_list[data->history].width);
      int k=0;
         for(int i=0;i<data->dim_list[data->history].hight;i++){
            for(int j=0;j<data->dim_list[data->history].width;j++){
               saver[k]=temp_data[i][j];
               k++;
             }
          }
         data->selected_image_data->info_header_data.width=data->dim_list[data->history].width;
         data->selected_image_data->info_header_data.hight=data->dim_list[data->history].hight;
      FILE *bmp_writer=fopen(file_name,"wb");
      fwrite(&data->selected_image_data->header_data,sizeof(data->selected_image_data->header_data),1,bmp_writer);
      fwrite(&data->selected_image_data->info_header_data,sizeof(data->selected_image_data->info_header_data),1,bmp_writer);
      char c;
      while(ftell(bmp_writer)!=data->selected_image_data->header_data.offset){
           fwrite(&c,1,1,bmp_writer);
      }
      fwrite(saver,sizeof(struct pixel_data)*data->dim_list[data->history].hight*data->dim_list[data->history].width,1,bmp_writer);
      fclose(bmp_writer);
      free(saver);
      for(int i=0;i<data->dim_list[data->history].hight;i++){
         free(temp_data[i]);
      }
      free(temp_data);
      printf("\nSaving successful");
   }
}
void open_image(Ihandle *button,struct iup_data *data){
    Ihandle *image_selector=IupFileDlg();
    IupSetAttribute(image_selector,"DIALOGTYPE","OPEN");
    IupSetAttribute(image_selector,"TITLE","Select a 24-bit bmp image");
    IupSetAttribute(image_selector,"FILTERALL","NO");
    IupPopup(image_selector,IUP_CENTER,IUP_CENTER);
    int selected=IupGetInt(image_selector,"STATUS");
    if(selected!=-1){
      char *file_name=IupGetAttribute(image_selector,"VALUE");
    
    FILE *bmp_reader;
    int padding=parse_image(file_name,bmp_reader,data->selected_image_data,data->edit_list,data->dim_list);
    if(padding==-1){
      IupMessage("Error","The selected file is not a valid 24-bit BMP image.\nPlease choose a different file.");
      return;
    }
    data->temp=realloc(data->temp,sizeof(struct pixel_data)*data->dim_list[0].hight*data->dim_list[0].width);
      
    v_flip(data->edit_list,data->dim_list[0].width,data->dim_list[0].hight,0,data->dim_list);
    colour_correction(data->edit_list,data->dim_list[0].width,data->dim_list[0].hight);
    flaten(data->edit_list,0,data->temp,data->dim_list[0].width,data->dim_list[0].hight);
    
    
    
    Ihandle *old_image=(Ihandle *)IupGetAttributeHandle(button,"stored_image");
    Ihandle *img_window=(Ihandle *)IupGetAttributeHandle(button,"img_window");
    Ihandle *window=(Ihandle *)IupGetAttributeHandle(button,"window");
    Ihandle *control_panel=(Ihandle *)IupGetAttributeHandle(button,"control_panel");
    Ihandle *img_scroll = (Ihandle *)IupGetAttributeHandle(button, "img_scroll");
    Ihandle *ui = (Ihandle *)IupGetAttributeHandle(img_scroll, "PARENT");
    Ihandle *image=IupImageRGB(data->dim_list[data->history].width,data->dim_list[data->history].hight,data->temp);
    
    IupSetAttributeHandle(img_window,"IMAGE",image);
    
    printf("\n %p---%p---%p",img_window,window,image);
    //IupSetfAttribute(img_window, "RASTERSIZE", "%dx%d",dim_list[0].width,dim_list[0].hight);
    printf("===============================");
    IupSetAttribute(window,"SIZE",NULL);
    IupSetAttribute(img_window,"SIZE",NULL);
    IupSetAttribute(control_panel,"SIZE",NULL);
    IupSetAttribute(ui,"SIZE",NULL);
    printf("-------------------------------");
    IupRefresh(window);
    {
      int _w = IupGetInt(window, "NATURALWIDTH");
      int _h = IupGetInt(window, "NATURALHEIGHT");
      IupSetfAttribute(window, "RASTERSIZE", "%dx%d", _w + 15, _h + 10);
      IupRefresh(window);
    }
    IupRedraw(window, 1);
    return;
}}
