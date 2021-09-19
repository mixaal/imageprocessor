#include <math.h>
#include <strings.h>
#include "gauss.h"
#include "layer.h"

// Fast Gauss blur filter from photopea
// http://blog.ivank.net/fastest-gaussian-blur.html
//
//
//

static int *boxesForGauss(float sigma, int n)  // standard deviation, number of boxes
{
    int *sizes = xmalloc(sizeof(int) * n);    
    float wIdeal = sqrtf((12*sigma*sigma/n)+1);  // Ideal averaging filter width
    int wl = (int)floor(wIdeal);  if(wl%2==0) wl--;
    int wu = wl+2;

    float mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n)/(-4*wl - 4);
    float m = round(mIdeal);
    // var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

    for(int i=0; i<n; i++) sizes[i] = (i<m) ?wl:wu;
    return sizes;
}


static void boxBlurH_4 (color_t *scl, color_t *tcl, int w, int h, int r) {
    float iarr = 1.0f / (r+r+1);
    for(int i=0; i<h; i++) {
        int ti = i*w, li = ti, ri = ti+r;
        color_t  fv = scl[ti], lv = scl[ti+w-1];
	int val = (r+1)*fv;
        for(int j=0; j<r; j++) val += scl[ti+j];
        for(int j=0  ; j<=r ; j++) { val += scl[ri++] - fv       ;   tcl[ti++] = (color_t)round(val*iarr); }
        for(int j=r+1; j<w-r; j++) { val += scl[ri++] - scl[li++];   tcl[ti++] = (color_t)round(val*iarr); }
        for(int j=w-r; j<w  ; j++) { val += lv        - scl[li++];   tcl[ti++] = (color_t)round(val*iarr); }
    }
}

static void boxBlurT_4 (color_t *scl, color_t *tcl, int w, int h, int r) {
    float iarr = 1.0f / (r+r+1);
    for(int i=0; i<w; i++) {
        int ti = i, li = ti, ri = ti+r*w;
        color_t fv = scl[ti], lv = scl[ti+w*(h-1)];
        int val = (r+1)*fv;
        for(int j=0; j<r; j++) val += scl[ti+j*w];
        for(int j=0  ; j<=r ; j++) { val += scl[ri] - fv     ;  tcl[ti] = (color_t)round(val*iarr);  ri+=w; ti+=w; }
        for(int j=r+1; j<h-r; j++) { val += scl[ri] - scl[li];  tcl[ti] = (color_t)round(val*iarr);  li+=w; ri+=w; ti+=w; }
        for(int j=h-r; j<h  ; j++) { val += lv      - scl[li];  tcl[ti] = (color_t)round(val*iarr);  li+=w; ti+=w; }
    }
}


static void boxBlur_4 (color_t *scl, color_t *tcl, int w, int h, int r) {
    for(int i=0; i<w*h; i++) tcl[i] = scl[i];
    boxBlurH_4(tcl, scl, w, h, r);
    boxBlurT_4(scl, tcl, w, h, r);
}

void gauss(layer_t layer, int r, double sigma, rect_t zone, _Bool mask) 
{
    int * bxs = boxesForGauss(r, 3);

    int width = layer.width;
    int height = layer.height;
    int color_components = layer.color_components;

    if (zone.minx<0) zone.minx=0;
    if (zone.miny<0) zone.miny=0;
    if (zone.maxx>=width) zone.maxx=width;
    if (zone.maxy>=height) zone.maxy=height;

    int w = zone.maxx - zone.minx;
    int h = zone.maxy - zone.miny;

    color_t *scl_red = xmalloc(w * h * sizeof(color_t));
    color_t *scl_green = xmalloc(w * h * sizeof(color_t));
    color_t *scl_blue = xmalloc(w * h * sizeof(color_t));


    color_t *tcl_red = xmalloc(w * h * sizeof(color_t));
    color_t *tcl_green = xmalloc(w * h * sizeof(color_t));
    color_t *tcl_blue = xmalloc(w * h * sizeof(color_t));

   int idx=0;
   for (int y=zone.miny; y<zone.maxy; y++) {
     for (int x=zone.minx; x<zone.maxx; x++) {
             int src_idx = color_components * (y*width + x);
             color_t r = layer.image[src_idx];
             color_t g = layer.image[src_idx+1];
             color_t b = layer.image[src_idx+2];
	     scl_red[idx] = r;
	     scl_green[idx] = g;
	     scl_blue[idx] = b;
	     idx++;
     }
   }
 
   
    

    boxBlur_4 (scl_red, tcl_red, w, h, (bxs[0]-1)/2);
    boxBlur_4 (tcl_red, scl_red, w, h, (bxs[1]-1)/2);
    boxBlur_4 (scl_red, tcl_red, w, h, (bxs[2]-1)/2);


    boxBlur_4 (scl_green, tcl_green, w, h, (bxs[0]-1)/2);
    boxBlur_4 (tcl_green, scl_green, w, h, (bxs[1]-1)/2);
    boxBlur_4 (scl_green, tcl_green, w, h, (bxs[2]-1)/2);


    boxBlur_4 (scl_blue, tcl_blue, w, h, (bxs[0]-1)/2);
    boxBlur_4 (tcl_blue, scl_blue, w, h, (bxs[1]-1)/2);
    boxBlur_4 (scl_blue, tcl_blue, w, h, (bxs[2]-1)/2);

   idx=0;
   for (int y=zone.miny; y<zone.maxy; y++) {
     for (int x=zone.minx; x<zone.maxx; x++) {
             int src_idx = color_components * (y*width + x);
             layer.image[src_idx] = scl_red[idx];;
             layer.image[src_idx+1] = scl_green[idx];
             layer.image[src_idx+2] = scl_blue[idx];
	     idx++;
     }
   }

   free(scl_red);
   free(scl_green);
   free(scl_blue);
    
   free(tcl_red);
   free(tcl_green);
   free(tcl_blue);
}




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

#if 0
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
#endif
