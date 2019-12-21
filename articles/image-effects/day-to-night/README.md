# Day to Night 

## Credits

Inspired by this youtube PS tutorial:
    https://www.youtube.com/watch?v=HXZOKqDbgpY

## Result
![Day to Night Result](../../../data/final_street.jpg)

## Implementation

[examples/day-to-night.c](../../../examples/day-to-night.c)

## Example
```c
  /**
   * Read the street daylight image.
   */
  layer_t street = read_JPEG_file(argv[1]);
  layer_info(street);

  /**
   * Prepare layers and setup masks.
   */
  layer_t street_night = layer_copy(street);
  layer_t street_lights = layer_copy(street);
  layer_t street_lights_mask = layer_new_dim(street.width, street.height, street.color_components, False, False);
  //street_lights.mask = street_lights_mask.image;
 

  /**
   * Convert daily image into night image.
   */
  colorize(street_night, vec3_init(0.0f, 0.0f, COLOR_MAX), 0.8f, 0.0f, ALL_LEVELS, False, street.zone);
  exposure(street_night, -1.9f, street.zone);
  write_JPEG_file("night.jpg", street_night, 90);

  /**
   * Convert daily image into night lamp light, enhance clarity with unsharp mask.
   */
  colorize(street_lights, vec3_init(COLOR_MAX, COLOR_MAX, 0.0f), 0.8f, 0.0f, ALL_LEVELS, False, street.zone);

  float cyan_red[3] = { 0.0f, 0.0f, 0.03f };
  float magenta_green[3] = { 0.05f, 0.08f, 0.07f };
  float yellow_blue[3] = { -0.3f, -0.3f, -0.3f };
  adjust_color_balance(street_lights, cyan_red, magenta_green, yellow_blue, street_lights.zone);

  unsharp(street_lights, 20, 1.05, street_lights.zone);
  write_JPEG_file("lights.jpg", street_lights, 90);
  
  /**
   * Create mask for night light image.
   */
  brush_touch(street_lights_mask, IMAGE, 400, 0.67f, 633, 114, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(street_lights_mask, IMAGE, 600, 0.27f, 633, 220, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(street_lights_mask, IMAGE, 400, 0.27f, 633, 304, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  brush_touch(street_lights_mask, IMAGE, 400, 0.47f, 633, 404, vec3_init(1.0f, 1.0f, 1.0f), blend_normal);
  write_JPEG_file("lights_mask.jpg", street_lights_mask, 90);

  /**
   * Set up lights mask ans merge-down.
   */
  street_lights.mask = street_lights_mask.image;
  //street_lights.blend_func = blend_lighten_only;
  street_lights.blend_func = blend_linear_light;
  layer_t layers[2] =  { street_night, street_lights };
  layer_t output = layer_merge_down( 2, layers );
  write_JPEG_file("final_street.jpg", output, 90);


```

