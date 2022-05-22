#ifndef CWINDOW_TYPES_H
#define CWINDOW_TYPES_H

#include "dc_utils.h"

#define CW_WHITE                (color) { 255, 255, 255, 255 }
#define CW_BLACK                (color) {   0,   0,   0, 255 }
#define CW_RED                  (color) { 255,   0,   0, 255 }
#define CW_GREEN                (color) {   0, 255,   0, 255 }
#define CW_BLUE                 (color) {   0,   0, 255, 255 }
#define CW_SLATEBLUE            (color) { 106,  90, 205, 255 }
#define CW_PURPLE               (color) { 128,   0, 128, 255 }
#define CW_MEDIUMPURPLE         (color) { 147, 112, 219, 255 }

typedef struct color
{
    u8 r, g, b, a;
} color;

#endif