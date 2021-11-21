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
int blob_array[size[0]+2][size[1]+2];
int labeled_pixels[size[0]][size[1]];
int i,j;
int count=0;
int image_int[DIM][DIM];

for (int i=0; i < DIM; i++){
	for (int j=0; j < DIM; j++){
		image_int[i][j] = (int)image[i][j];
	}
}
// initialize the blob array
for ( i = 0; i < size[0]+2; i++ ) {
      for ( j = 0; j < size[1]+2; j++ ) {
		if (i == 0 || j==0 || i == size[0] + 1 || j == size[1] + 1){ // it helps with initial label
			blob_array[i][j] = 800;
		}
		else{
			blob_array[i][j] = image_int[i-1][j-1];
		}

      }
}

for ( i = 1; i < size[0]+1; i++ ) {
      for ( j = 1; j < size[1]+1; j++ ) {
    	  // check the difference with the top and left pixels to be in a threshold
    	  if( abs( blob_array[i][j]-blob_array[i-1][j]) <= threshold && abs( blob_array[i][j] -blob_array[i][j-1]) <= threshold){
    		  labeled_pixels[i-1][j-1] = labeled_pixels[i-1][j-2];
    	  }
    	  // one of the two (top or left) pixels are outside the threshold
    	  else{
    		  // top pixel is not the same, but the left is
    		  if( abs( blob_array[i][j]-blob_array[i-1][j])>threshold && abs(blob_array[i][j] -blob_array[i][j-1]) <= threshold){
    			  labeled_pixels[i-1][j-1] = labeled_pixels[i-1][j-2];
    		  }
    		  else{
    			  // top pixel is the same, but the left is not
    			  if( abs( blob_array[i][j] - blob_array[i-1][j]) <= threshold && abs(blob_array[i][j]- blob_array[i][j-1]) > threshold){
    				  labeled_pixels[i-1][j-1] = labeled_pixels[i-2][j-1];
    			  }
    			  else{
    				  // check if the intensity difference is greater than the threshold and assigns a new label
    				  if( abs( blob_array[i][j] -blob_array[i-1][j]) > threshold && abs(blob_array[i][j]- blob_array[i][j-1]) > threshold){
    					  //add a new label
    					  count++;
    					  labeled_pixels[i-1][j-1] = count;
    				  }

    			  }
    		  }
    	  }
      }
}

// initialize a label array
int labels[count];
for ( i = 0; i < count; i++ ) {
	labels[i]=i;
}
// second pass to substitute equal labels with the smalles one
for ( i = 1; i < size[0]+1; i++ ) {
      for ( j = 1; j < size[1]+1; j++ ) {
    	  if(abs(blob_array[i][j] - blob_array[i-1][j]) <= threshold && abs( blob_array[i][j] - blob_array[i][j-1]) <= threshold){
    		  // the label of the left neighbor is bigger
    		  if ( labeled_pixels[i-2][j-1] <= labeled_pixels[i-1][j-2]){
    			  int min = labels[labeled_pixels[i-2][j-1]-1];
    			  int temp = labeled_pixels[i-2][j-1]-1;
    			  // substitute with the smallest equal label
    			  while( labels[temp] != temp){
    				  temp=labels[temp];
    			  }

    			  min=labels[temp];
    			  labels[labeled_pixels[i-1][j-2]-1]=min;
    		  }
    		  // the label of the top neighbor is bigger
    		  else{

    			  int min=labels[labeled_pixels[i-1][j-2]-1];
    			  int temp=labeled_pixels[i-1][j-2]-1;
    			  // substitute with the smallest equal label
    			  while(labels[temp]!=temp){
    				  temp=labels[temp];
    			  }
    			  min=labels[temp];
    			  labels[labeled_pixels[i-2][j-1]-1]=min;
    		  }
    	  }
      }
}
// calculate the different regions
int n_regions = labels[0];

for ( i = 0; i < count; i++ ) {
	if ( labels[i] > n_regions)
		n_regions = labels[i];
}

// substitute the labels in the original image pixel labels
for ( i = 0; i < size[0]; i++ ) {
      for ( j = 0; j < size[1]; j++ ) {
    	  labeled_pixels[i][j]=labels[labeled_pixels[i][j]];

      }
}

for ( i = 0; i < size[0]; i++ ) {
      for ( j = 0; j < size[1]; j++ ) {
    	  proc_img[i][j]=(unsigned char) (int) ( ( (float)labeled_pixels[i][j] / (float) n_regions) * 255);
	  }
}

}
