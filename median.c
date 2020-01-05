#include "median.h"


/*
 * Algorithm from N. Wirth's book, implementation by N. Devillard.
 * This code in public domain.
 */


typedef color_t elem_type ;

#define ELEM_SWAP(a,b) { register elem_type t=(a);(a)=(b);(b)=t; }


/*---------------------------------------------------------------------------
   Function :   kth_smallest()
   In       :   array of elements, # of elements in the array, rank k
   Out      :   one element
   Job      :   find the kth smallest element in the array
   Notice   :   use the median() macro defined below to get the median. 

                Reference:

                  Author: Wirth, Niklaus 
                   Title: Algorithms + data structures = programs 
               Publisher: Englewood Cliffs: Prentice-Hall, 1976 
    Physical description: 366 p. 
                  Series: Prentice-Hall Series in Automatic Computation 

 ---------------------------------------------------------------------------*/

#if 1
static elem_type kth_smallest(elem_type a[], int n, int k)
{
    int i,j,l,m ;
    elem_type x ;

    l=0 ; m=n-1 ;
    while (l<m) {
        x=a[k] ;
        i=l ;
        j=m ;
        do {
            while (a[i]<x) i++ ;
            while (x<a[j]) j-- ;
            if (i<=j) {
                ELEM_SWAP(a[i],a[j]) ;
                i++ ; j-- ;
            }
        } while (i<=j) ;
        if (j<k) l=i ;
        if (k<i) m=j ;
    }
    return a[k] ;
}


#define get_median(a,n) kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)))
#else
static color_t get_median(color_t *data, int size)
{
  int swap = 1;
  while(swap) {
    swap = 0;
    for(int i=0; i<size-1; i++) {
        if(data[i]>data[i+1]) {
           swap=1;
           color_t t = data[i];
           data[i] = data[i+1];
           data[i+1] = t;
        }
    }
  }
  return data[size>>1];
}
#endif

color_t get_color(layer_t layer, int x, int y, int component)
{
  int idx = component + y*layer.width*layer.color_components + x*layer.color_components;
  return layer.image[idx];
}

/**
 * Median filter implementation.
 * 
 * @param layer layer to process.
 * @param N     window size
 * @param zone  zone to filter
 */
void median_filter(layer_t layer, int N, rect_t zone) 
{
   layer_t layer_cp = layer_copy(layer);
   color_t *image = layer.image;
  // color_t *copy= layer_cp.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
   color_t color_values_r[N*N];
   color_t color_values_g[N*N];
   color_t color_values_b[N*N];
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  int N2 = N>>1;
  for(int y=zone.miny+N2+1; y<zone.maxy-N2-1; y++)  {
    for(int x=zone.minx+N2+1; x<zone.maxx-N2-1; x++) {
       color_t r, g, b;
       int idx = y*width*color_components + x*color_components;
       int c = 0;
       for(int i=-N2; i<=N2; i++) {
         for(int j=-N2; j<=N2; j++) {
            int xx = x+i;
            int yy = y+j;
            if(xx<0) xx = 0;
            if(yy<0) yy = 0;
            if(xx>=width) xx = width-1;
            if(yy>=height) yy = height-1;
            r = get_color(layer_cp, xx, yy, 0);
            g = get_color(layer_cp, xx, yy, 1);
            b = get_color(layer_cp, xx, yy, 2);
            color_values_r[c] = r;
            color_values_g[c] = g;
            color_values_b[c] = b;
            c++;
         }
       }
       r = get_median(color_values_r, c);
       g = get_median(color_values_g, c);
       b = get_median(color_values_b, c);

       image[idx] = r;
       image[idx+1] = g;
       image[idx+2] = b;
    }
  }
 layer_free(layer_cp);
}
