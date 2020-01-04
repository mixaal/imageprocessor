# Levels Visualization

Shadows, mid-tones and highlights visualization.

## Credits

* Junktown

## Functions Used

- [levels()](../../../levels.c)

## Original 
![Original Image](../laplace/IMG_1612.JPG)

## Result
![Comics Result](levels-result.jpg)

## Implementation

[examples/levels.c](../../../examples/levels.c)

## Example
```c
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  rect_t adjust_zone = { 0, 0, source.width, source.height };
  levels(source, adjust_zone, false);
  write_JPEG_file("levels-result.jpg", source, 90);
```

