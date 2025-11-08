//
// Created by standa on 11/4/25.
//

#include "basicStructs.h"

struct Vector2 Vector2_Subtract(struct Vector2 u,struct Vector2 v) {
    struct Vector2 newVector = {
        u.x-v.x,
        u.y-v.y
    };
    return newVector;
}