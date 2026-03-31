#pragma once
#include <pebble.h>

typedef struct App App;

App* app_create();
void app_destroy( App* app );
