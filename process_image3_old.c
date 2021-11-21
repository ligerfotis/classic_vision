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
// pictures/chess.lpr
void process_image(image, size, proc_img)
unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{
int image_int[DIM][DIM];
int blob_id[DIM][DIM];

for (int i=0; i < size[0]; i++){
	for (int j=0; j < size[1]; j++){
		image_int[i][j] = (int)image[i][j];
	}
}
for (int i=0; i < size[0]; i++){
	for (int j=0; j < size[1]; j++){
		if (i == 1 || j == 1)
				blob_id[i][j] = 1000;
	}
}

int label = 0;
int thresshold = 20;
int matching_labels[30];
int count = 0;
for (int row = 1; row < size[0]; row++) {
    for (int col = 1; col < size[1]; col++) {

		// look at the top
		if ((abs(image_int[row-1][col] - image_int[row][col]) <= thresshold) && ((abs(image_int[row][co-1l] - image_int[row][col]) <= thresshold))){
				blob_id[row][col] = blob_id[row-1][col];
//			if (((image_int[row][col-1] - thresshold) <= image_int[row][col]) && ((image_int[row][col-1] + thresshold) >= image_int[row][col])){
//				if (blob_id[row][col] != blob_id[row][col-1]){
//					matching_labels[blob_id[row][col-1]] = blob_id[row][col];
//					count++;
//				}
//			}

		}
		//look at the left
		else if (((image_int[row][col-1] - thresshold) <= image_int[row][col]) && ((image_int[row][col-1] + thresshold) >= image_int[row][col])){
			blob_id[row][col] = blob_id[row][col-1];
//			if (((image_int[row-1][col] - thresshold) <= image_int[row][col]) && ((image_int[row-1][col] + thresshold) >= image_int[row][col])){
//							if (blob_id[row][col] != blob_id[row-1][col]){
//								matching_labels[blob_id[row][col]] = blob_id[row-1][col];
//								count++;
//							}
//						}
		}
		else
			label++;
			blob_id[row][col] = label;
   }
}
//for (int row = 1; row < size[0]; row++) {
//    for (int col = 1; col < size[1]; col++) {
//    	for (int k=0; k<count; k++){
//    		if 	blob_id[row][col] == label;
//    	}
//   }
//}



for (int i = 0; i < size[0]; i++) {
    for (int j = 0; j < size[1]; j++) {
//		proc_img[i][j] = (unsigned char) (int)((blob_id[i][j])*(255 - 0)/(25.5));

		proc_img[i][j] = (unsigned char) (int)(blob_id[i][j]);


    	printf("%d ", (int)blob_id[i][j]);
   }
    printf("\n");

}




}


