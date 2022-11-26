#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

typedef struct Buffer Buffer;

Buffer * bufferCreate(uint16_t buffSize);
uint8_t bufferPush(Buffer *buffer, uint8_t value);
uint16_t bufferPushValues(Buffer *buffer, const uint8_t *values, uint16_t size);
uint8_t bufferPop(Buffer *buffer);
uint8_t bufferPeek(const Buffer *buffer);
void bufferClear(Buffer *buffer);
uint16_t bufferGetSize(const Buffer *buffer);
uint16_t bufferPopValues(Buffer *buffer, uint8_t *buf, uint16_t bufSize);
uint8_t bufferHasValues(const Buffer *buffer);
uint16_t bufferGetFreeSize(const Buffer *buffer);
uint16_t bufferPeekValues(const Buffer * buffer, uint8_t **values);
uint16_t bufferPopToBuffer(Buffer * const from, Buffer * const to, uint16_t size);

static struct {
    Buffer * (*create)(uint16_t buffSize);
    uint8_t (*push)(Buffer *buffer, uint8_t value);
    uint16_t (*pushValues)(Buffer *buffer, const uint8_t *values, uint16_t size);
    uint8_t (*pop)(Buffer *buffer);
    uint8_t (*peek)(const Buffer *buffer);
    void (*clear)(Buffer *buffer);
    uint16_t (*size)(const Buffer *buffer);
    uint16_t (*popValues)(Buffer *buffer, uint8_t *buf, uint16_t bufSize);
    uint8_t (*hasValues)(const Buffer *buffer);
    uint16_t (*freeSize)(const Buffer *buffer);
    uint16_t (*peekValues)(const Buffer *buffer, uint8_t **values);
    uint16_t (*popToBuffer)(Buffer *from, Buffer *to, uint16_t size);
} const buffer = {
        bufferCreate,
        bufferPush,
        bufferPushValues,
        bufferPop,
        bufferPeek,
        bufferClear,
        bufferGetSize,
        bufferPopValues,
        bufferHasValues,
        bufferGetFreeSize,
        bufferPeekValues,
        bufferPopToBuffer

};
#endif