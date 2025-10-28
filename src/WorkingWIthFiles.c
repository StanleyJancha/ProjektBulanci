//
// Created by standa on 10/28/25.
//
#include "WorkingWithFiles.h"



bool GetMapObjects(struct Object *objects, int *length) {

    FILE *file = fopen(MAP_OBJECTS_FILE, "r");
    if (!file) {return false;}

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        strcmp("Objects", line);
    }



    printf("\n\n\n\n");



}

bool GetObject(struct Object *object);
