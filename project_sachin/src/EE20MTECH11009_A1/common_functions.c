#include "common_functions.h"

float* convo(float x[], float h[], int length_x, int length_h, int length_y)
{
    int i, j, start_h, start_x, end_x;
    float* y = (float*) calloc(length_y, sizeof(float));
    for (i = 0; i < length_y; i++)
    {
        start_x = MAX(0, i - length_h + 1);
        end_x = MIN(i + 1, length_x);
        start_h = MIN(i, length_h - 1);
        for (j = start_x; j < end_x; j++)
        {
            y[i] += h[start_h--] * x[j];
        }
    }
    return y;
}

float* correlate(float x[], float y[], int length_x, int length_y, int length_r)
{
    int i, j, start_y, start_x, end_x;
    float* r = (float*) calloc(length_r, sizeof(float));
    for (i = 0; i < length_r; i++)
    {
        start_x = MAX(0, i - length_y + 1);
        end_x = MIN(i + 1, length_x);
        start_y = MAX(0,length_y - 1 -i);
        for (j = start_x; j < end_x; j++)
        {
            r[i] += y[start_y++] * x[j];
        }
    }
    return r;
}

float* downsample(float x[], int length_x, int length_y, int factor)
{
    int i;
    float* y = (float*) calloc(length_y, sizeof(float));
    for (i = 0; i < length_x; i++)
    {
        if (i % factor == 0)
            y[i / factor] = x[i];
        else;           
    }
    return y;
}

float* upsample(float x[], int length_x, int length_y, int factor)
{
    int i;
    float* y = (float*) calloc(length_y, sizeof(float));
    for (i = 0; i < length_y; i++)
    {
        if (i % factor == 0)
            y[i] = x[i / factor];
        else
            y[i] = 0;
    }
    return y;
}