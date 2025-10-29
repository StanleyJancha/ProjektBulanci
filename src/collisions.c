//
// Created by standa on 10/29/25.
//

#include "collisions.h"

bool Collsions_areColliding(const struct Object *object1, const struct Object *object2) {
    if (object1 == NULL || object2 == NULL){return false;}

    struct Vector2 startBoundObject1 = {object1->position.x, object1->position.y}; // top left
    struct Vector2 endBoundObject1 = {object1->position.x + object1->size.x, object1->position.y + object1->size.y}; // bottom right

    struct Vector2 startBoundObject2 = {object2->position.x, object2->position.y}; // top left
    struct Vector2 endBoundObject2 = {object2->position.x + object2->size.x, object2->position.y + object2->size.y}; // bottom right

    if (endBoundObject1.x <= startBoundObject2.x) return false; // Object1 is left of Object2
    if (startBoundObject1.x >= endBoundObject2.x) return false; // Object1 is right of Object2
    if (endBoundObject1.y <= startBoundObject2.y) return false; // Object1 is above Object2
    if (startBoundObject1.y >= endBoundObject2.y) return false; // Object1 is below Object2

    return true;
}
