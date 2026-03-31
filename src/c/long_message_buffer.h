#pragma once
#include <pebble.h>

typedef struct LongMessageBuffer LongMessageBuffer;
struct LongMessageBuffer
{
    uint32_t size;
    uint32_t capacity;
    uint8_t* data;
};

void lmb_prepare( LongMessageBuffer* lmb, uint32_t message_size );
void lmb_append_message_part( LongMessageBuffer* lmb, const uint8_t* data, uint32_t size );
bool lmb_is_complete( const LongMessageBuffer* lmb );
void lmb_reset( LongMessageBuffer* lmb );