#include <sprite.h>

void sprite::set_origin(const pointf& new_origin) {
    if (my_bounds.contains(new_origin)) {
        my_origin = new_origin;    
    }
    else {
        my_origin = (new_origin - my_bounds.my_top_left);
    }
}