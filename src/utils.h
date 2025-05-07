#ifndef UTILS_H
#define UTILS_H

#define PI 3.141592653589793

#include "types.h"

float distance(sfVector2f p1, sfVector2f p2);
sfVector2f get_dir(sfVector2f p1, sfVector2f p2);
float rad_to_deg(float angle);

#endif 