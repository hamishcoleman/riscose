/* enum.h -- provide enumeration images. */

/* $Id$ */

/* these macros allow a user to write ENUM_IMAGE(colour, sunken, image)
 * to get a character pointer to a string representing the value sunken
 * in the enumeration colour using the temporary character array image
 * if necessary.
 *
 * the information required by these macros is
 *
 *     the images of the enumeration.
 *
 *         char *colour_image[] = {
 *             "red",
 *             NULL,
 *             "blue",
 *         };
 *
 *     the enumerate value of the first array element.
 *
 *         #define colour_MIN 10
 *
 *     if the DIM macro can't be used to determine the number of
 *     elements in the image array because the array definition isn't
 *     visible then the maximum possible enumerate index.
 *
 *         #define colour_MAX 12
 *
 *     a macro defining either that DIM should be used or that a MAX is
 *     provided by hand.
 *
 *         #define colour_TYPE HAND
 *
 *     if DIM can be used then colour_TYPE would be DIM.
 *
 * if the enumerate is out of range or the corresponding array element is
 * NULL then the temporary character array is used to generate the decimal
 * value of the enumerate.  the array should be ENUM_BUF_SIZE big. */

#undef EXTERN
#ifdef DEFINE_ENUM_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

/* the value of the enum corresponding to the first array element. */
#define ENUM_MIN(id) (id ## _MIN)

/* the maximum index of an enum we can DIM. */
#define ENUM_MAX_DIM(id) (DIM(id ## _image) + ENUM_MIN(id) - 1)

/* the maximum, manually defined, index of an enum. */
#define ENUM_MAX_HAND(id) (id ## _MAX)

/* the token pasting operator has his limitations, adding multiple
 * levels of indirection gets around them.  see K&R2. */
#define ENUM_CAT1(a, b) a ## b
#define ENUM_CAT2(a, b) ENUM_CAT1(a, b)

/* the maximum index of an enum regardless of its type. */
#define ENUM_MAX(id) ENUM_CAT2(ENUM_MAX_, ENUM_CAT2(id, _TYPE))(id)

/* is val within range. */
#define ENUM_IN_RANGE(id, val) \
    ((val) >= ENUM_MIN(id) && (val) <= ENUM_MAX(id))

/* is there a valid image for the id enum with val. */
#define ENUM_HAS_IMAGE(id, val) \
    (ENUM_IN_RANGE(id, val) && id ## _image[(val) - ENUM_MIN(id)])

/* the size of the temporary `no valid image' buffer. */
#define ENUM_BUF_SIZE (sizeof "4294967295")

#define ENUM_IMAGE_FMT(id, val, buf, fmt) \
    (ENUM_HAS_IMAGE(id, val) ? id ## _image[(val) - ENUM_MIN(id)] : \
        (sprintf((buf), (fmt), (val)), (buf)))

/* get string image for val of type enum id using buf as temporary work
 * space. */
#define ENUM_IMAGE(id, val, buf) \
    ENUM_IMAGE_FMT(id, val, buf, "%u")

#ifndef DEFINE_ENUM_GLOBALS
extern char *boolean_image[];
#else
char *boolean_image[] = {
    "false",
    "true"
};
#endif
#define boolean_MIN 0
#define boolean_MAX 1
#define boolean_TYPE HAND
