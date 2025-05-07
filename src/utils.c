#include "utils.h"
#include <math.h>

float distance(sfVector2f p1, sfVector2f p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

sfVector2f get_dir(sfVector2f p1, sfVector2f p2){
    float dist = distance(p1 ,p2);
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return (sfVector2f){dx/dist, dy/dist};
}

float rad_to_deg(float angle){
    return (360*angle)/(2*PI);
}