#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"


int print_char(char c, int col, int row, char attr);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/************************************************
 *  Public API: kprint, kprint_at, clearscreen  *
 ************************************************/

/** Prints message at specified location.
 * */
void kprint_at(char* message, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

/** Prints message on cursor. Alias for kprint_at(message, -1, -1). */
void kprint(char* message) { kprint_at(message, -1, -1); }

/** Clears screen memory of any text. */
void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    unsigned char *screen = (unsigned char*) VIDEO_ADDRESS;
    int i;
    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset( get_offset(0, 0) );
}

void kprint_backspace() {
    int offset = get_cursor_offset()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}

/************************************************
 *                Private functions             *
 ************************************************/

/** base char print function */
int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = '!';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else if (c == 0x08) {
        vidmem[offset] = ' ';
        vidmem[offset+1] = attr;
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    /* Implement Scrolling:
        Check max size, copy every line upwards, clear last line
    */

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        for (int i = 1; i < MAX_ROWS; i++) 
            memory_copy((u8*) get_offset(0, i) + VIDEO_ADDRESS,
                (u8*) get_offset(0, i-1) + VIDEO_ADDRESS,
                MAX_COLS * 2);
        char* last_line = (char*) VIDEO_ADDRESS + get_offset(0, MAX_ROWS-1);
        for (int i = 0; i < MAX_COLS*2; i++) last_line[i] = 0;
        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

/** Reads CRTC port for data about the cursor. */
int get_cursor_offset() {
    /* Control Port: Data needed
    DATA 0xE - 14 (CURSOR POSITION - HIGH BYTE)
    DATA 0xF - 15 (CURSOR POSITION - LOW BYTE)
    */
    port_byte_out(CRTC_CTRL, 14);
    int offset = port_byte_in(CRTC_DATA) << 8;
    port_byte_out(CRTC_CTRL, 15);
    offset += port_byte_in(CRTC_DATA);
    // Position in memory multiplied by two because VGA data blocks are two bytes wide
    // One for the character, another for the attribute
    return offset * 2; 
}

/** Writes to CRTC port to change cursor position. */
void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(CRTC_CTRL, 14);
    port_byte_out(CRTC_DATA, (unsigned char)(offset >> 8) );
    port_byte_out(CRTC_CTRL, 15);
    port_byte_out(CRTC_DATA, (unsigned char)(offset & 0xff) );
}

int get_offset(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}

int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS); 
}

int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS) ) / 2; 
}
