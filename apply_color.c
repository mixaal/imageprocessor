#include "apply_color.h"
#include "color_balance.h"
#include "histogram.h"
#include "layer.h"
#include <float.h>
#include <string.h>


static void print_params(apply_color_params_t params)
{
 printf("HIGHLIGHTS=[%.3f %.3f %.3f]\nMIDTONES=[%.3f %.3f %.3f]\nSHADOWS=[%.3f %.3f %.3f]\nERROR=%f\n-------\n", 
      params.cyan_red_coef[2], params.magenta_green_coef[2], params.yellow_blue_coef[2],
      params.cyan_red_coef[1], params.magenta_green_coef[1], params.yellow_blue_coef[1],
      params.cyan_red_coef[0], params.magenta_green_coef[0], params.yellow_blue_coef[0],params.error);
}


typedef struct  {
   apply_color_params_t best, second_best;
} two_best_t;



static two_best_t apply_range(
   histogram_t source_histogram, 
   layer_t destination_layer, 
   float step, 
   vec3 start, vec3 end, 
   int level_index, 
   apply_color_params_t params,
   _Bool preserve_luminosity
)
{
    apply_color_params_t saved_params = params;
    apply_color_params_t second_best = params;


    float dx = (end.x - start.x) / step;
    float dy = (end.y - start.y) / step;
    float dz = (end.z - start.z) / step;
    printf("[%.3f %.3f %.3f] ----> [%.3f %.3f %.3f] by (%.3f, %3f, %.3f)\n", start.x, start.y, start.z, end.x, end.y, end.z, dx, dy, dz);
    if (dx==0.0f) dx = 0.0001f;
    if (dy==0.0f) dy = 0.0001f;
    if (dz==0.0f) dz = 0.0001f;

    for (float cr=start.x;cr<=end.x;cr+=dx) 
    for (float mg=start.y;mg<=end.y;mg+=dy) 
    for (float yb=start.z;yb<=end.z;yb+=dz)  {
       params.cyan_red_coef[level_index] = cr;
       params.magenta_green_coef[level_index] = mg;
       params.yellow_blue_coef[level_index] = yb;
       //print_params(params);
       layer_t adjust_layer = layer_copy(destination_layer);
       adjust_color_balance(adjust_layer, params.cyan_red_coef, params.magenta_green_coef, params.yellow_blue_coef, preserve_luminosity, destination_layer.zone);
       histogram_t destination_histogram = histogram_from_layer(adjust_layer, RGB, destination_layer.zone);
       layer_free(adjust_layer);
       float error = histogram_difference(&destination_histogram, &source_histogram);
       if (error < saved_params.error) {
          second_best = saved_params;
          //save params
          saved_params = params;
          saved_params.error=error;
       }
    }
    two_best_t r = {saved_params, second_best};
    return r;
}

apply_color_params_t apply_color(layer_t source_layer, rect_t source_zone, layer_t destination_layer, float step, _Bool preserve_luminosity) 
{
    histogram_t source_histogram = histogram_from_layer(source_layer, RGB, source_zone);
    two_best_t highlights, midtones, shadows;

    apply_color_params_t params, saved_params, second_best_midtones, second_best_highlights, second_best_shadows;
    memset(&params, 0, sizeof(params));
    params.error = FLT_MAX;
    saved_params = second_best_shadows = second_best_highlights = second_best_midtones = params;
    /**
     * Start with midtones, find the best match.
     */

    vec3 start_vec = vec3_init(-1.0f, -1.0f, -1.0f);
    vec3 end_vec = vec3_init(1.0f, 1.0f, 1.0f);
    float last_error = params.error;
   
    for(int i=0; i< 10 ; i++ ) {
       if(i!=0) {
         start_vec = vec3_init(saved_params.cyan_red_coef[1], saved_params.magenta_green_coef[1], saved_params.yellow_blue_coef[1]);
         end_vec   = vec3_init(second_best_midtones.cyan_red_coef[1], second_best_midtones.magenta_green_coef[1], second_best_midtones.yellow_blue_coef[1]);
       }
       midtones   = apply_range(source_histogram, destination_layer, step, start_vec, end_vec, 1, saved_params, preserve_luminosity);

       if(i!=0) {
         start_vec = vec3_init(saved_params.cyan_red_coef[2], saved_params.magenta_green_coef[2], saved_params.yellow_blue_coef[2]);
         end_vec   = vec3_init(second_best_midtones.cyan_red_coef[2], second_best_midtones.magenta_green_coef[2], second_best_midtones.yellow_blue_coef[2]);
       }
       highlights = apply_range(source_histogram, destination_layer, step, start_vec, end_vec, 2, midtones.best, preserve_luminosity);


       if(i!=0) {
         start_vec = vec3_init(saved_params.cyan_red_coef[2], saved_params.magenta_green_coef[0], saved_params.yellow_blue_coef[0]);
         end_vec   = vec3_init(second_best_midtones.cyan_red_coef[2], second_best_midtones.magenta_green_coef[0], second_best_midtones.yellow_blue_coef[0]);
       }
       shadows    = apply_range(source_histogram, destination_layer, step, start_vec, end_vec, 0, highlights.best, preserve_luminosity);
 
       saved_params = shadows.best;
       print_params(saved_params);
       if (last_error == saved_params.error) {
          return saved_params;
       }
       last_error = saved_params.error;

       second_best_highlights = highlights.second_best;  
       second_best_midtones   = midtones.second_best;  
       second_best_shadows    = shadows.second_best;  
    }

    return saved_params;
}
