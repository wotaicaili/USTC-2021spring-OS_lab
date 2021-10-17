#include "tick.h"
#include "vga.h"

int hh, mm, ss;

// display the wall clock at the right bottom of vga
void display_wall_clock(void) {
    char str[12];
    
    str[0] = '0' + hh / 10;
    str[1] = '0' + hh % 10;
    str[2] = ' ';
    str[3] = ':';
    str[4] = ' ';
    str[5] = '0' + mm / 10;
    str[6] = '0' + mm % 10;
    str[7] = ' ';
    str[8] = ':';
    str[9] = ' ';
    str[10] = '0' + ss / 10;
    str[11] = '0' + ss % 10;
    
    put_chars(str, 0x3, VGA_SCREEN_HEIGHT - 1, VGA_SCREEN_WIDTH - 12);
}

// when a tick takes place, this function is called by hook
void update_wall_clock(void) {

    if (kernel_tick_times % TICK_FREQ != 0)
        return;
        
    ss = (ss + 1) % 60;
        
    if (ss == 0)
        mm = (mm + 1) % 60;

    if (mm == 0 && ss == 0)
        hh = (hh + 1) % 24;
    
    display_wall_clock();
}

// create the wall clock and set it to a specific value
void setWallClock(int h, int m, int s) {

    // hh: [0, 24)
    if (h < 0) hh = 0;
    else if (h > 23) hh = 23;
    else hh = h;
    // mm: [0, 60)
    if (m < 0) mm = 0;
    else if (m > 59) mm = 59;
    else mm = m;
    // ss: [0, 60)
    if (s < 0) ss = 0;
    else if (s > 59) ss = 59;
    else ss = s;

    // add the updata function to hook
    setWallClockHook(update_wall_clock);

    display_wall_clock();
}

// read wall clock value
void getWallClock(int *h, int *m, int *s) {
    *h = hh;
    *m = mm;
    *s = ss;
}
