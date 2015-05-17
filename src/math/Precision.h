#ifndef MATH_PRECISION_H
#define MATH_PRECISION_H

#include <float.h>

#define SCALAR_MAX_VAL FLT_MAX
#define SCALAR_MIN_VAL -FLT_MAX

/**
 *  Used in math classes, allows us to easily change the precision
 *  of our physics calculations.
 */
typedef float scalar;

#endif
