#include <pebble.h>
#include "src/c/app.h"
#include "src/c/macros.h"

int main( void )
{
    APP_LOG( APP_LOG_LEVEL_INFO, "Starting up" );
    
    App* const app = app_create();
    GUARD( app, goto give_up );
    
    app_event_loop();
    
give_up:
    app_destroy( app );
    return 0;
}
