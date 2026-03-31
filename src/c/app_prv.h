#include <pebble.h>
#include "src/c/app.h"
#include "src/c/long_message_buffer.h"

struct App
{
    Window* window;
    BitmapLayer* bitmapLayer;
    
    LongMessageBuffer imageDataBuffer;
    uint32_t imageWidth;
    uint32_t imageHeight;
    
    GBitmap* bitmap;
};

bool app_init( App* app );
void app_deinit( App* app );

void app_init_layout( App* app );
void app_start( App* app );
void app_stop( App* app );
void app_destroy_layout( App* app );

void app_on_receive_message( App* app, DictionaryIterator* message );

void app_set_bitmap_dimensions( App* app, uint32_t width, uint32_t height );
bool app_is_new_image_ready( const App* app );
void app_update_image( App* app );

void app_request_new_image( App* app );
