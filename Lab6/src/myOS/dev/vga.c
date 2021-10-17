#include "io.h"
#include "vga.h"

#define VGA_BASE 0xB8000
#define VGA_SIZE 0x1000

#define CURSOR_LINE_REG 0xE
#define CURSOR_COL_REG 0xF
#define CURSOR_INDEX_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

int cursor_pos; // the current position of the cursor, equals current column number + current line number * VGA_SCREEN_WIDTH

// set the cursor to a sepecific position
void set_cursor_pos(int pos) {
    // set cursor line position
    outb(CURSOR_INDEX_PORT, CURSOR_LINE_REG);
    outb(CURSOR_DATA_PORT, (pos >> 8) & 0xFF);
    // set cursor column position
    outb(CURSOR_INDEX_PORT, CURSOR_COL_REG);
    outb(CURSOR_DATA_PORT, pos & 0xFF);
}

// scroll the screen
void scroll_screen(void) {
    int *address_reader32, *address_writer32;
    int i = 0;
    while (i < cursor_pos) {
        address_writer32 = (int*)(VGA_BASE + i * 2);
        if (i < cursor_pos - VGA_SCREEN_WIDTH) {
            // copy the data from the next line
            address_reader32 = (int*)(VGA_BASE + i * 2 + VGA_SCREEN_WIDTH * 2);
            *(address_writer32) = *(address_reader32);
        }
        else
            // clear the last line
            *(address_writer32) = 0x0F200F20;
        i += 2;
    }
    cursor_pos -= VGA_SCREEN_WIDTH;
    set_cursor_pos(cursor_pos);
}

// put a single char to vga
void put_char2pos(unsigned char c, int color, int pos) {
    // put the char to cursor_pos
    unsigned char *address_writer8;
    address_writer8 = (unsigned char*)(VGA_BASE + pos * 2);
    *address_writer8 = c;
    address_writer8 = (unsigned char*)(VGA_BASE + pos * 2 + 1);
    *address_writer8 = (unsigned char)color;
}

// clear the screen
void clear_screen(void) {
    int *address_writer32;
    int i = 0;
    while (i < VGA_SIZE) {
        // fill the screen with spaces
        address_writer32 = (int*)(VGA_BASE + i);
        *address_writer32 = 0x0F200F20;
        i += 4;
    }
    cursor_pos = 0;
    set_cursor_pos(cursor_pos);
}

// append a string to vga, move the cursor
void append2screen(char *str, int color) { 
    for (int i = 0; str[i] != '\0'; i++) {
        switch (str[i]) {
            case '\t':
                // move the cursor forward for 4 chars' length
                cursor_pos += 4;
                set_cursor_pos(cursor_pos);
                while (cursor_pos / VGA_SCREEN_WIDTH >= VGA_SCREEN_HEIGHT - 1)
                    scroll_screen();
                break;
                
            case '\n':
                // move the cursor to the start of next line
                cursor_pos = cursor_pos + VGA_SCREEN_WIDTH - cursor_pos % VGA_SCREEN_WIDTH;     
                set_cursor_pos(cursor_pos);
                while (cursor_pos / VGA_SCREEN_WIDTH >= VGA_SCREEN_HEIGHT - 1)
                    scroll_screen();
                break;
                
            case '\f':
                // a new page = clear the screen
                clear_screen();
                break;
                
            case '\r':
                // set the cursor to the start of this line
                cursor_pos = cursor_pos - cursor_pos % VGA_SCREEN_WIDTH;
                set_cursor_pos(cursor_pos);
                break;
                
            case '\b':
                // set the cursor to the pervious line if it is not at the start of a line
                if (cursor_pos % VGA_SCREEN_WIDTH != 0) {
                    cursor_pos--;
                    set_cursor_pos(cursor_pos);
                }
                break;
                
            case '\v':
                // put spaces until the next line's this column
                cursor_pos += VGA_SCREEN_WIDTH;
                set_cursor_pos(cursor_pos);
                while (cursor_pos / VGA_SCREEN_WIDTH >= VGA_SCREEN_HEIGHT - 1)
                    scroll_screen();
                break;
                
            default:
                // put a char to the current cursor position
                while (cursor_pos / VGA_SCREEN_WIDTH >= VGA_SCREEN_HEIGHT - 1)
                // whether need to scroll the screen
                    scroll_screen();

                put_char2pos(str[i], color, cursor_pos);
                cursor_pos++;
                set_cursor_pos(cursor_pos);
                break;
        }
    }
}

// just put a string to a specific position, don't change cursor pos, ignore escape char like '\n'
void put_chars(char *str, int color, int line, int col) {
    int pos = line * VGA_SCREEN_WIDTH + col;
    for (int i = 0; str[i] != '\0'; i++) {
        put_char2pos(str[i], color, pos);
        pos++;
    }
}
