#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
 float x, y, z;
} vec3;

typedef struct {
  vec3 domain_max, domain_min; 
  int lut_size;
  vec3 *color_grade;
} lut_t;

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
          lut->color_grade = malloc(sz*sz*sz*sizeof(vec3));
          lut->lut_size = sz;
          if(!lut->color_grade) return 0;
          continue;
       }
       sscanf(line, "%f %f %f", &x, &y, &z);
       vec3 v = {x, y, z};
       lut->color_grade[i++] = v;
  }

  if(line) free(line);

  fclose(fp);



  return 1;
}


int main(int argc, char **argv)
{
   lut_t lut;
   read_lut(argv[1], &lut);

   printf("Lut Size: %d\n", lut.lut_size);
   printf("Domain min: %f %f %f\n", lut.domain_min.x, lut.domain_min.y, lut.domain_min.z);
   printf("Domain max: %f %f %f\n", lut.domain_max.x, lut.domain_max.y, lut.domain_max.z);
   printf("[%f %f %f]\n", lut.color_grade[4096].x, lut.color_grade[4096].y, lut.color_grade[4096].z);
   return 0;
}
