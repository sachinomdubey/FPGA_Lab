#include <stdlib.h>
#include <stdio.h>

// these are the  functions to get the min and max of two numbers
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

float* convo(float x[], float h[], int length_x, int length_h, int length_y); 
//this fiunction performs convolution between signal h and x.

float* correlate(float x[], float y[], int length_x, int length_y, int length_r);
//this fiunction performs correlation between signal x and y.

float* downsample(float x[], int length_x, int length_y, int factor);
//this fiunction performs downsampling of signal x.

float* upsample(float x[], int length_x, int length_y, int factor);
//this fiunction performs upsampling of signal x.
