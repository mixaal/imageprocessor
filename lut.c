#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <lut.h>
#include <layer.h>

static vec3 trilinear_interpolation(lut_t lut, vec3 color);
static void base_cube(lut_t lut, vec3 rgb, int *i, int *j, int *k);


#define IDX(lut, i, j, k) ((i)+lut.lut_size*(j)+lut.lut_size*lut.lut_size*(k))


void lut_translate(layer_t layer, lut_t lut, rect_t zone) 
{
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       vec3 color = vec3_init(r/(float)COLOR_MAX, g/(float)COLOR_MAX, b/(float)COLOR_MAX);
       vec3 new = trilinear_interpolation(lut, color);

       int nr = COLOR_MAX*new.x;
       int ng = COLOR_MAX*new.y;
       int nb = COLOR_MAX*new.z;
       if(nr<0) nr = 0;
       if(ng<0) ng = 0;
       if(nb<0) nb = 0;

       if(nr>COLOR_MAX) nr = COLOR_MAX;
       if(ng>COLOR_MAX) ng = COLOR_MAX;
       if(nb>COLOR_MAX) nb = COLOR_MAX;

       image[idx] = nr;
       image[idx+1] = ng;
       image[idx+2] = nb;
    }
  }

}

static void base_cube(lut_t lut, vec3 rgb, int *i, int *j, int *k)
{
 int s = lut.lut_size;
 float delta = 1.0f / (s-1);
 *i = (int) floor(rgb.x / delta);
 *j = (int) floor(rgb.y / delta);
 *k = (int) floor(rgb.z / delta);
 if(*i==lut.lut_size-1) --*i;
 if(*j==lut.lut_size-1) --*j;
 if(*k==lut.lut_size-1) --*k;
}

_Bool read_lut(const char *filename, lut_t *lut)
{
  FILE *fp = fopen(filename, "rb");

  if(!fp) {
    perror("fopen()");
    return 0;
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  lut->domain_min.x = 0.0f;
  lut->domain_min.y = 0.0f;
  lut->domain_min.z = 0.0f;


  lut->domain_max.x = 1.0f;
  lut->domain_max.y = 1.0f;
  lut->domain_max.z = 1.0f;
  int i = 0;
  float x, y, z;
  while ((read = getline(&line, &len, fp)) != -1) {
       //printf("Retrieved line of length %zu:\n", read);
       //printf("\n|%s|\n", line);
       if(!line) continue;
       if(0==read) continue;
       if(line[0]=='#') continue; // skip comments
       if(line[0]=='\n') continue; // skip newlines
       if(!strncmp(line, "TITLE", 5)) continue;
       if(!strncmp(line, "DOMAIN_MIN", 10)) {
          sscanf(line, "DOMAIN_MIN %f %f %f\n", &x, &y, &z);
          lut->domain_min.x = x;
          lut->domain_min.y = y;
          lut->domain_min.z = z;
          continue;
       }
       if(!strncmp(line, "DOMAIN_MAX", 10)) {
          sscanf(line, "DOMAIN_MAX %f %f %f\n", &x, &y, &z);
          lut->domain_max.x = x;
          lut->domain_max.y = y;
          lut->domain_max.z = z;
          continue;
       }

       if(!strncmp(line, "LUT_3D_SIZE", 11)) {
          int sz;
          sscanf(line, "LUT_3D_SIZE %d\n", &sz);
          printf("total_size=%d\n", sz*sz*sz);
          lut->color_grade = malloc(sz*sz*sz*sizeof(vec3));
          lut->lut_size = sz;
          if(!lut->color_grade) return 0;
          continue;
       }
       sscanf(line, "%f %f %f", &x, &y, &z);
       vec3 v = {x, y, z};
       lut->color_grade[i++] = v;
  }
  printf("i=%d\n", i);

  if(line) free(line);

  fclose(fp);



  return 1;
}

static vec3 trilinear_interpolation(lut_t lut, vec3 color)
{
   int i, j, k;
   base_cube(lut, color, &i, &j, &k);

   int idx0 = IDX(lut, i, j, k);
   int idx1 = IDX(lut, i+1, j+1, k+1);
   //printf("XXX: idx0=%d idx1=%d\n", idx0, idx1);
   vec3 C000 = lut.color_grade[idx0];
   vec3 C111 = lut.color_grade[idx1];
   vec3 C100 = lut.color_grade[IDX(lut, i+1, j, k)];
   vec3 C001 = lut.color_grade[IDX(lut, i, j, k+1)];
   vec3 C010 = lut.color_grade[IDX(lut, i, j+1, k)];
   vec3 C101 = lut.color_grade[IDX(lut, i+1, j, k+1)];
   vec3 C110 = lut.color_grade[IDX(lut, i+1, j+1, k)];
   vec3 C011 = lut.color_grade[IDX(lut, i, j+1, k+1)];
   
   float dCx = C111.x - C000.x;
   float dCy = C111.y - C000.y;
   float dCz = C111.z - C000.z;
   float xd = (color.x - C000.x)/dCx;
   float yd = (color.y - C000.y)/dCy;
   float zd = (color.z - C000.z)/dCz;
   if(dCx<0.0001f) xd = 1.0f;
   if(dCy<0.0001f) yd = 1.0f;
   if(dCz<0.0001f) zd = 1.0f;

   //printf("delta=[%f %f %f] C111.x=%f C000.x=%f\n", xd, yd, zd, C111.x, C000.x);

   vec3 C00 = vec3_add(vec3_multiply(C000, 1-xd), vec3_multiply(C100, xd));
   vec3 C01 = vec3_add(vec3_multiply(C001, 1-xd), vec3_multiply(C101, xd));
   vec3 C10 = vec3_add(vec3_multiply(C010, 1-xd), vec3_multiply(C110, xd));
   vec3 C11 = vec3_add(vec3_multiply(C011, 1-xd), vec3_multiply(C111, xd));
   
   vec3 C0 = vec3_add(vec3_multiply(C00, 1-yd), vec3_multiply(C10, yd));
   vec3 C1 = vec3_add(vec3_multiply(C01, 1-yd), vec3_multiply(C11, yd));

   vec3 C = vec3_add(vec3_multiply(C0, 1-zd), vec3_multiply(C1, zd));
   return C;
}

#ifdef __TEST__
int main(int argc, char **argv)
{
   lut_t lut;
   read_lut(argv[1], &lut);

   printf("Lut Size: %d\n", lut.lut_size);
   printf("Domain min: %f %f %f\n", lut.domain_min.x, lut.domain_min.y, lut.domain_min.z);
   printf("Domain max: %f %f %f\n", lut.domain_max.x, lut.domain_max.y, lut.domain_max.z);
   printf("[%f %f %f]\n", lut.color_grade[31710].x, lut.color_grade[31710].y, lut.color_grade[31710].z);
   printf("[%f %f %f]\n", lut.color_grade[32767].x, lut.color_grade[32767].y, lut.color_grade[32767].z);
   int i, j, k;
   vec3 color = {0.1f, 0.67f, 0.67f};
   base_cube(lut, color, &i, &j, &k);
   printf("i=%d j=%d k=%d\n", i, j, k);  
   vec3 C = trilinear_interpolation(lut, color);
   printf("LUT=[%f %f %f]\n", C.x, C.y, C.z);
   return 0;
}
#endif
