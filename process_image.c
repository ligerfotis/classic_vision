#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>

#define DIM 512

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y  - left upper corner's coordinates               */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;


/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image  - the original greyscale image                          */
/* size   - the actual size of the image                          */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/
void process_image(image, size, proc_img)
unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{

int threshold=45;
int b[size[0]+2][size[1]+2];
int region[size[0]][size[1]];
int i,j;
int count=0;
int image_int[DIM][DIM];

for (int i=0; i < DIM; i++){
	for (int j=0; j < DIM; j++){
		image_int[i][j] = (int)image[i][j];
	}
}

for ( i = 0; i < size[0]+2; i++ ) {
      for ( j = 0; j < size[1]+2; j++ ) {
		if(i==0 || j==0 || i==size[0]+1 || j==size[1]+1){
			b[i][j]=1000;
		}
		else{
			b[i][j]=image_int[i-1][j-1];
		}

      }
}

for ( i = 1; i < size[0]+1; i++ ) {
      for ( j = 1; j < size[1]+1; j++ ) {

    	  if( abs( b[i][j]-b[i-1][j]) <= threshold && abs( b[i][j]-b[i][j-1]) <= threshold){
    		  region[i-1][j-1]=region[i-1][j-2];
    	  }

    	  else{
    		  if(abs(b[i][j]-b[i-1][j])>threshold && abs(b[i][j]-b[i][j-1])<=threshold){
    			  region[i-1][j-1]=region[i-1][j-2];
    		  }
    		  else{
    			  if(abs(b[i][j]-b[i-1][j]) <= threshold && abs(b[i][j]-b[i][j-1]) > threshold){
    				  region[i-1][j-1] = region[i-2][j-1];
    			  }
    			  else{
    				  if(abs(b[i][j]-b[i-1][j])>threshold && abs(b[i][j]-b[i][j-1])>threshold){
    					  count++;
    					  region[i-1][j-1] = count;
    				  }

    			  }
    		  }
    	  }
      }
}

int arr[count];
for ( i = 0; i < count; i++ ) {
	arr[i]=i;
}

for ( i = 1; i < size[0]+1; i++ ) {
      for ( j = 1; j < size[1]+1; j++ ) {
    	  if(abs(b[i][j]-b[i-1][j])<=threshold && abs(b[i][j]-b[i][j-1])<=threshold){
    		  if(region[i-2][j-1]<=region[i-1][j-2]){
    			  int min=arr[region[i-2][j-1]-1];
    			  int temp=region[i-2][j-1]-1;

    			  while(arr[temp]!=temp){
    				  temp=arr[temp];
    			  }
    			  min=arr[temp];
    			  arr[region[i-1][j-2]-1]=min;
    		  }

    		  else{
    			  int min=arr[region[i-1][j-2]-1];
    			  int temp=region[i-1][j-2]-1;
    			  while(arr[temp]!=temp){
    				  temp=arr[temp];
    			  }
    			  min=arr[temp];
    			  arr[region[i-2][j-1]-1]=min;
    		  }

    	  }

      }
}


for ( i = 0; i < size[0]; i++ ) {
      for ( j = 0; j < size[1]; j++ ) {
    	  region[i][j]=arr[region[i][j]];

      }
}

int totalregions=arr[0];
for ( i = 0; i < count; i++ ) {
	if(arr[i]>totalregions)
		totalregions=arr[i];
}

printf("total regions: %d", totalregions);

for ( i = 0; i < size[0]; i++ ) {

      for ( j = 0; j < size[1]; j++ ) {
    	  float tempo=((float)region[i][j]/(float)totalregions)*255.0;
    	  proc_img[i][j]=(unsigned char) (int)tempo;
//    	  printf("%d ", (int)tempo);
	  }
//      printf("\n");
}


}
