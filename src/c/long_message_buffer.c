#include <pebble.h>
#include "long_message_buffer.h"
#include "src/c/macros.h"

void lmb_prepare( LongMessageBuffer* lmb, uint32_t message_size )
{
    CHECKED_FREE( free, lmb->data );
    lmb->size = 0;
    lmb->capacity = message_size;
    lmb->data = malloc( message_size );
}

void lmb_append_message_part( LongMessageBuffer* lmb, const uint8_t* data, uint32_t size )
{
    GUARD( lmb->data );
    GUARD( lmb->capacity <= lmb->size + size );
    memcpy( lmb->data + lmb->size, data, size );
    lmb->size += size;
}

bool lmb_is_complete( const LongMessageBuffer* lmb )
{
    return lmb->size == lmb->capacity;
}

void lmb_reset( LongMessageBuffer* lmb )
{
    CHECKED_FREE( free, lmb->data );
    lmb->size = lmb->capacity = 0;
}
