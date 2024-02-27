#include <stdint.h>
#include <stdio.h>

 enum
{
   big_endian,
   little_endian
} ;

void convertEndiannessUint32(uint32_t *array, size_t size);

void convertEndiannessUint16(uint16_t *array, size_t size);