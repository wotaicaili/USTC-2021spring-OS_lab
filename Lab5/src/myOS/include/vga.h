#ifndef __VGA_H__
#define __VGA_H__

#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25

void scroll_screen(void);
void append2screen(char *str, int color);
void put_chars(char *str, int color, int line, int col);
void clear_screen(void);

#endif
