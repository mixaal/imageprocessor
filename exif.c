#include <stdio.h>
#include <string.h>
#include <libexif/exif-data.h>
#include <layer.h>

// See: https://github.com/libexif/libexif/blob/master/contrib/examples/photographer.c

/* Remove spaces on the right of the string */
static void trim_spaces(char *buf)
{
    char *s = buf-1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0; /* nul terminate the string on the first of the final spaces */
}

/* Show the tag name and contents if the tag exists */
static void show_tag(ExifData *d, ExifIfd ifd, ExifTag tag)
{
    /* See if this tag exists */
    ExifEntry *entry = exif_content_get_entry(d->ifd[ifd],tag);
    if (entry) {
        char buf[1024];

        /* Get the contents of the tag in human-readable form */
        exif_entry_get_value(entry, buf, sizeof(buf));

        /* Don't bother printing it if it's entirely blank */
        trim_spaces(buf);
        if (*buf) {
            printf("%s: %s\n", exif_tag_get_name_in_ifd(tag,ifd), buf);
        }
    }
}

void get_color_space(ExifData *ed)
{
   show_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_COLOR_SPACE);
}

ExifData *load_exif(const char *filename)
{

  ExifData *ed;

  ed = exif_data_new_from_file(filename);
  if (!ed) {
    fprintf(stderr, "File not readable or no EXIF data in file %s\n", filename);
    return NULL;
  }

  return ed;
}


