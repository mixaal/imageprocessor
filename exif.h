#ifndef __IM_EXIF_H__
#define __IM_EXIF_H__ 1

#include <libexif/exif-data.h>


ExifData *load_exif(const char *filename);
void get_color_space(ExifData *ed);

#endif /* __IM_EXIF_H__ */
