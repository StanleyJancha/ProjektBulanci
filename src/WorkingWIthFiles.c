//
// Created by standa on 10/28/25.
//
#include "WorkingWithFiles.h"



bool GetMapObjects(struct Object *objects, int *length) {

    FILE *file = fopen(MAP_OBJECTS_FILE, "r");
    if (!file) {return false;}

    char line[512];
    int i = 0;
    char tmp;
    while((tmp = (char)fgetc(file)) != EOF) {
        line[i] = tmp;
        i++;
    }



    printf("%s",line);



}

bool GetObject(struct Object *object);
