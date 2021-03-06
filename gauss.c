#include <math.h>
#include <strings.h>
#include "gauss.h"
#include "layer.h"


void create_gauss(double **GKernel, int N, double sigma, _Bool normalize)
{
    int N2 = N/2;
    // intialising standard deviation to 1.0
    if (sigma < 0.00001) 
      sigma = (double)N/5.0;
    double r, s = 2.0 * sigma * sigma;
 
    // sum is for normalization
    double sum = 0.0;
 
    // generating 5x5 kernel
    for (int x = -N2; x <= N2; x++)
    {
        for(int y = -N2; y <= N2; y++)
        {
            r = sqrt(x*x + y*y);
            GKernel[x + N2][y + N2] =
                     (exp(-(r*r)/s))/(M_PI * s);
            sum += GKernel[x + N2][y + N2];
        }
    }
    if (!normalize) return;
    // normalising the Kernel
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            GKernel[i][j] /= sum;
}

void gauss(layer_t layer, int radius, double sigma, rect_t zone, _Bool mask) 
{
   color_t *image = (mask) ? layer.mask : layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
   if ((radius&1)==0) radius++; // make it odd number
   int N2 = radius / 2;
   double **kernel = xmalloc(sizeof(double)*radius);
   for (int i=0; i<radius; i++) {
     kernel[i] = xmalloc(sizeof(double)*radius);
   }

   /**
    * Create a copy of the source image.
    */
   layer_t layer_cp = layer_copy(layer);
   color_t *image_copy = (mask) ? layer_cp.mask : layer_cp.image;

   create_gauss(kernel, radius, sigma, True);

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

   for (int y=zone.miny; y<zone.maxy; y++) {
     for (int x=zone.minx; x<zone.maxx; x++) {
     int idx = color_components * (y*width + x);
        double sr, sg, sb;
        color_t r, g, b;
        //printf("x=%d y=%d\n", x, y);
        sr = sg = sb = 0;
        for (int ky = -N2; ky <= N2; ky++)
          for (int kx = -N2; kx <= N2; kx++) {
             int xx = x + kx;
             int yy = y + ky;
             if (xx<0) xx=0;
             if (yy<0) yy=0;
             if (xx>=width) xx=width-1;
             if (yy>=height) yy=height-1;
             int src_idx = color_components * (yy*width + xx);
             r = image_copy[src_idx];
             g = image_copy[src_idx+1];
             b = image_copy[src_idx+2];
             sr += r * kernel[ky+N2][kx+N2];
             sg += g * kernel[ky+N2][kx+N2];
             sb += b * kernel[ky+N2][kx+N2];
          }
  
       //sr /= radius*radius;
       //sg /= radius*radius;
       //sb /= radius*radius;
       image[idx] = (color_t) sr;
       image[idx+1] = (color_t) sg;
       image[idx+2] = (color_t) sb;
     }
   }

   layer_free(layer_cp);
   for (int i=0; i<radius; i++) {
     free(kernel[i]);
   }
   free(kernel);


}
