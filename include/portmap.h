#include <stdint.h>

#ifndef MYOS_INCLUDE_PORTMAP_H
#define MYOS_INCLUDE_PORTMAP_H

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif