//
// Created by standa on 10/28/25.
//

#ifndef PROJEKT_WORKINGWITHFILES_H
#define PROJEKT_WORKINGWITHFILES_H
#include <stdbool.h>
#include "object.h"

#define MAP_OBJECTS_FILE "configs/mapObjects.txt"

bool GetMapObjects(struct Object *objects, int *length);

bool GetObject(struct Object *object);



#endif //PROJEKT_WORKINGWITHFILES_H