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

void median_filter(layer_t layer, rect_t zone) 
{
   layer_t layer_cp = layer_copy(layer);
   color_t *image = layer.image;
   color_t *copy= layer_cp.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
   color_t color_values_r[9];
   color_t color_values_g[9];
   color_t color_values_b[9];
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  for(int y=zone.miny+1; y<zone.maxy-1; y++)  {
    for(int x=zone.minx+1; x<zone.maxx-1; x++) {
       color_t r, g, b;
       int idx = y*width*color_components + x*color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[0] = r; color_values_g[0] = g; color_values_b[0] = b;
       
       idx+=color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[1] = r; color_values_g[1] = g; color_values_b[1] = b;
       
       idx-=2*color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[2] = r; color_values_g[2] = g; color_values_b[2] = b;

       idx-=width*color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[3] = r; color_values_g[3] = g; color_values_b[3] = b;

       idx+=color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[4] = r; color_values_g[4] = g; color_values_b[4] = b;

       idx+=color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[5] = r; color_values_g[5] = g; color_values_b[5] = b;

       idx+=width*color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[6] = r; color_values_g[6] = g; color_values_b[6] = b;

       idx-=color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[7] = r; color_values_g[7] = g; color_values_b[7] = b;

       idx-=color_components;
       r = copy[idx]; g = copy[idx+1]; b = copy[idx+2];
       color_values_r[8] = r; color_values_g[8] = g; color_values_b[8] = b;

       r = get_median(color_values_r, 9);
       g = get_median(color_values_g, 9);
       b = get_median(color_values_b, 9);

       image[idx] = r;
       image[idx+1] = g;
       image[idx+2] = b;
    }
  }
 layer_free(layer_cp);
}
