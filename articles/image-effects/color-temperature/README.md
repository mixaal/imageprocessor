# Temperature Manipulation

## Credits

Inspired by photodemon software:
  http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/

## Functions Used

- [kelvin_temperature()](../../../kelvin_temp.c)

## Result

![Temperature Manipulation Result](../../../data/final_true_blood_poster.jpg)

## Implementation

* [examples/true-blood.c](../../../examples/true-blood.c)

## Example

```c
  /**
   * Read the "True Blood" image.
   */
  layer_t true_blood = read_JPEG_file(argv[1]);
  rect_t adjustment_zone = {true_blood.width>>1, 0, true_blood.width, true_blood.height};
  layer_info(true_blood);

  /**
   * Prepare layers and setup masks.
   */
  layer_t true_blood_adjustment_layer = layer_copy(true_blood);
 

  /**
   * Adjust temperature in adjustment layer.
   */
  kelvin_temperature(true_blood_adjustment_layer, 1600.0f, 0.58f, 0.9999f, adjustment_zone);
  write_JPEG_file("true_blood_adjustment_layer.jpg", true_blood_adjustment_layer, 90);

  /**
   * Setup opacity of adjustment layer and merge down.
   */
  true_blood_adjustment_layer.opacity = 0.5f;
  true_blood_adjustment_layer.blend_func = blend_normal;
  layer_t layers[2] =  { true_blood, true_blood_adjustment_layer};
  layer_t output = layer_merge_down( 2, layers );
  write_JPEG_file("final_true_blood_poster.jpg", output, 90);


```


