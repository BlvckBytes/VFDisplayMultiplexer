#include "VFDHandle.h"
#include <inttypes.h>
#include "CharBits.h"
#include <avr/io.h>
#include <limits.h>

#ifndef VFDMultiplex_H_
#define VFDMultiplex_H_

void vfd_mp_set_underline(VFDHandle* handle, uint8_t row, uint8_t index, _Bool state);
void vfd_mp_set_underline_row(VFDHandle* handle, uint8_t row, _Bool state);
void vfd_mp_clear_underline(VFDHandle* handle);
void vfd_mp_put_row_chars(VFDHandle* handle, uint8_t col);
void vfd_mp_handle(VFDHandle* handle);
void vfd_mp_shutdown(VFDHandle* handle);
void vfd_mp_resume(VFDHandle* handle);
void vfd_mp_set_text(VFDHandle* handle, uint8_t row, const char* text);
void vfd_mp_set_char(VFDHandle* handle, uint8_t row, uint8_t col, char value);
void vfd_mp_clear_text(VFDHandle* handle);
void vfd_mp_clear_text_row(VFDHandle* handle, uint8_t row);
void vfd_mp_clear_display(VFDHandle* handle);

void vfd_mp_setup_pins();
void vfd_mp_clear_pixel_registers();

VFDHandle vfd_init_handle();

#endif