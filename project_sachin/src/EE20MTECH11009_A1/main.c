#include <stdio.h>
#include "common_functions.h"

int main(int argc, char* argv[])
{
    //Convolution : Signal 1
    float x1[] = { 0.5377, 1.8339, -2.2588, 0.8622, 0.3188, -1.3077, -0.4336, 0.3426, 3.5784, 2.7694, -1.3499, 3.0349, 0.7254, -0.0631, 0.7147, -0.2050, -0.1241, 1.4897, 1.4090, 1.4172 }; 
    //Convolution : Signal 2
    float h1[] = { 0.6715, -1.2075, 0.7172, 1.6302, 0.4889, 1.0347, 0.7269, -0.3034, 0.2939, -0.7873, 0.8884, -1.1471, -1.0689, -0.8095, -2.9443 };
    float* y1 = convo(x1, h1, 20, 15, 34); //Calling convolving function. 
    printf("Convolution:\n y[n] = {");
    for (int i = 0;i < 34;i++)
    {
        printf("%0.4lf ", y1[i]);
    }
    printf("}\n \n");


    //correlation : Signal 1
    float x2[] = { 0.5377, 1.8339, -2.2588, 0.8622, 0.3188, -1.3077, -0.4336, 0.3426, 3.5784, 2.7694, -1.3499, 3.0349, 0.7254, -0.0631, 0.7147, -0.2050, -0.1241, 1.4897, 1.4090, 1.4172 };
    //Correlation : Signal 2
    float y2[] = { 0.6715, -1.2075, 0.7172, 1.6302, 0.4889, 1.0347, 0.7269, -0.3034, 0.2939, -0.7873, 0.8884, -1.1471, -1.0689, -0.8095, -2.9443 };
    float* r2 = correlate(x2, y2, 20, 15, 34); //Calling correlating function.  
    printf("Correlation:\n Rxy[k] = {");
    for (int i = 0;i < 34;i++)
    {
        printf(" %0.4lf ", r2[i]);      
    }
    printf("}\n \n");
  

    //DownSampling : Given signal
    float x3[] = { 0.3252, -0.7549, 1.3703, -1.7115, -0.1022, -0.2414, 0.3192, 0.3129, -0.8649, -0.0301, -0.1649, 0.6277, 1.0933, 1.1093, -0.8637, 0.0774, -1.2141, -1.1135, -0.0068, 1.5326, -0.7697, 0.3714, -0.2256, 1.1174, -1.0891, 0.0326, 0.5525, 1.1006, 1.5442, 0.0859, -1.4916, -0.7423, -1.0616, 2.3505, -0.6156, 0.7481 };
    int length_x3 = 36;
    int m = 2;  //DownSampling factor : m=2
    int length_y3 = (int)( length_x3 / m)+1;
    float* y3 = downsample(x3, length_x3, length_y3, m);
    printf("Down Sampled signal:\n y[n] = {");
    for (int i = 0;i < length_y3;i++)
    {
        printf(" %0.4lf ", y3[i]);
    }
    printf("}\n\n");


    //UpSampling : Given signal
    float x4[] = { 0.3252, 1.3703, -0.1022, 0.3192, -0.8649, -0.1649, 1.0933, -0.8637, -1.2141, -0.0068, -0.7697, -0.2256, -1.0891, 0.5525, 1.5442, -1.4916, -1.0616, -0.6156 };
    int length_x4 = 18;
    int l = 2;  //Upsampling factor : L=2
    int length_y4 = length_x4 * l - 1;
    float* y4 = upsample(x4, length_x4, length_y4, l);
    printf("Up Sampled signal:\n y[n] = {");
    for (int i = 0;i < length_y4;i++)
    {
        printf(" %0.4lf ", y4[i]);
    }
    printf("}\n");
    return 0;
}