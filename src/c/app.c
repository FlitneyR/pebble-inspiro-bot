#include <pebble.h>
#include "src/c/app_prv.h"
#include "src/c/macros.h"

App* app_create()
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Creating app" );
    
    App* app = (App*)malloc( sizeof *app );
    GUARD( app, return NULL );
    *app = (App){};
    
    if ( !app_init( app ) )
        CHECKED_FREE( app_destroy, app );
    
    return app;
}

void app_destroy( App* app )
{
    if ( !app )
        return;
    
    app_deinit( app );
    free( app );
}

void message_received_callback( DictionaryIterator* iterator, void* context ) { app_on_receive_message( (App*)context, iterator ); }
void on_window_load( Window* window )      { app_init_layout( (App*)window_get_user_data( window ) ); }
void on_window_appear( Window* window )    { app_start( (App*)window_get_user_data( window ) ); }
void on_window_disappear( Window* window ) { app_stop( (App*)window_get_user_data( window ) ); }
void on_window_unload( Window* window )    { app_destroy_layout( (App*)window_get_user_data( window ) ); }

bool app_init( App* app )
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Initilising app" );
    
    app->imageWidth = UINT32_MAX;
    app->imageHeight = UINT32_MAX;
    
    GUARD( app->window = window_create(), return false );
    window_set_user_data( app->window, app );
    window_set_window_handlers( app->window, (WindowHandlers){
        .load=on_window_load,
        .appear=on_window_appear,
        .disappear=on_window_disappear,
        .unload=on_window_unload,
    });
    
    window_stack_push( app->window, false );
    
    app_message_set_context( app );
    app_message_register_inbox_received( message_received_callback );
    
    app_message_open(
        app_message_inbox_size_maximum(),
        app_message_outbox_size_maximum()
    );
    
    return true;
}

void app_deinit( App* app )
{
    lmb_reset( &app->imageDataBuffer );
    window_destroy( app->window );
}

void app_init_layout( App* app )
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Initialising app layout" );
    
    Layer* const root_layer = window_get_root_layer( app->window );
    app->bitmapLayer = bitmap_layer_create( layer_get_bounds( root_layer ) );
    layer_add_child( root_layer, bitmap_layer_get_layer( app->bitmapLayer ) );
}

void app_start( App* app ) { /* nothing to do here yet */ }
void app_stop( App* app )  { /* nothing to do here yet */ }

void app_destroy_layout( App* app )
{
    bitmap_layer_destroy( app->bitmapLayer );
}

void app_on_receive_message( App* app, DictionaryIterator* message )
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Received message of %d bytes", dict_size( message ) );
    GUARD( app && message, return );
    
    if ( dict_find( message, MESSAGE_KEY_JSReady ) )
        app_request_new_image( app );

    APP_LOG( APP_LOG_LEVEL_INFO, "" );

    const Tuple* const long_message_size = dict_find( message, MESSAGE_KEY_ImageDataSize );
    if ( long_message_size )
        lmb_prepare( &app->imageDataBuffer, long_message_size->value->uint32 );

    APP_LOG( APP_LOG_LEVEL_INFO, "" );

    const Tuple* const long_message_part = dict_find( message, MESSAGE_KEY_ImageDataPart );
    if ( long_message_part )
        lmb_append_message_part( &app->imageDataBuffer, long_message_part->value->data, long_message_part->length );

    APP_LOG( APP_LOG_LEVEL_INFO, "" );

    const Tuple* const bitmap_width = dict_find( message, MESSAGE_KEY_BitmapWidth );
    const Tuple* const bitmap_height = dict_find( message, MESSAGE_KEY_BitmapHeight );

    APP_LOG( APP_LOG_LEVEL_INFO, "" );

    if ( bitmap_width || bitmap_height )
    {
        GUARD( bitmap_width );
        GUARD( bitmap_height );
        app_set_bitmap_dimensions( app, bitmap_width->value->uint32, bitmap_height->value->uint32 );
    }

    APP_LOG( APP_LOG_LEVEL_INFO, "" );

    if ( app_is_new_image_ready( app ) )
    {
        app_update_image( app );
    }
}

void app_set_bitmap_dimensions( App* app, uint32_t width, uint32_t height )
{
    app->imageWidth = width;
    app->imageHeight = height;
}

bool app_is_new_image_ready( const App* app )
{
    return lmb_is_complete( &app->imageDataBuffer )
        && app->imageWidth != UINT32_MAX
        && app->imageHeight != UINT32_MAX;
}

void app_update_image( App* app )
{
    CHECKED_FREE( gbitmap_destroy, app->bitmap );
    app->bitmap = gbitmap_create_blank( (GSize){ .w=app->imageWidth, .h=app->imageHeight }, GBitmapFormat8Bit );
    
    uint8_t* data_copy = malloc( app->imageDataBuffer.size );
    memcpy( data_copy, app->imageDataBuffer.data, app->imageDataBuffer.size );
    gbitmap_set_data( app->bitmap, data_copy, GBitmapFormat8Bit, app->imageWidth, true );
    
    bitmap_layer_set_bitmap( app->bitmapLayer, app->bitmap );
    
    lmb_reset( &app->imageDataBuffer );
    app->imageWidth = UINT32_MAX;
    app->imageHeight = UINT32_MAX;
}

void app_request_new_image( App* app )
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Requesting new image" );
    
    DictionaryIterator* message;
    
    {
        AppMessageResult result = app_message_outbox_begin( &message );
        if ( result != APP_MSG_OK )
            APP_LOG( APP_LOG_LEVEL_ERROR, "result = %d", (int)result );
        GUARD( result == APP_MSG_OK );
    }
    
    {
        DictionaryResult result = dict_write_data( message, MESSAGE_KEY_RequestNewImage, NULL, 0 );
        if ( result != DICT_OK )
            APP_LOG( APP_LOG_LEVEL_ERROR, "result = %d", (int)result );
        GUARD( result == DICT_OK );
    }
    
    {
        AppMessageResult result = app_message_outbox_send();
        if ( result != APP_MSG_OK )
            APP_LOG( APP_LOG_LEVEL_ERROR, "result = %d", (int)result );
        GUARD( result == APP_MSG_OK );
    }   
}
