#include "console.h"
#include "portmap.h"
#include <stdint.h>

static int term_pos = 0;
static char* VGA_MEMORY = (char*) 0xb8000;

static VGA_Color terminal_font_color = LIGHT_GRAY; // Default font color will be light gray
static VGA_Color terminal_background_color = BLACK; // Default background color is black

void print_character(char c) {
	print_character_with_color(c, terminal_background_color, terminal_font_color);
}

void print_character_with_color(char c, VGA_Color bg_color, VGA_Color font_color){
	if (c == '\n'){
		term_pos = (term_pos / VGA_WIDTH + 1) * VGA_WIDTH;
		update_cursor();
		return;
	}

	if (c == '\b'){
		if (term_pos < 1) return;
		VGA_MEMORY[(term_pos - 1) * 2] = ' ';
        VGA_MEMORY[(term_pos - 1) * 2 + 1] = terminal_background_color;
		term_pos--;
		update_cursor();
		return;
	}

	VGA_MEMORY[term_pos * 2] = c;
	VGA_MEMORY[term_pos * 2 + 1] = (bg_color << 4) | font_color;;
	term_pos++;
	update_cursor();
	return;
}

void print_string(char* str){
	while(*str){
		print_character(*str++);
	}
}

void print_string_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {
	for (int i=0; str[i] != '\0'; i++) {
		print_character_with_color(str[i], bg_color, font_color);
	}
}

void print_line(char* line) {
	print_string(line);
	print_character('\n');
}

void print_line_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {
    print_string_with_color(str, bg_color, font_color);
    print_character_with_color('\n', bg_color, font_color);
}


void clear_terminal(){
    int cells = 80 * 25;
    for (int i = 0; i < cells; i++){
        VGA_MEMORY[i * 2] = ' ';       // Set character to space
        VGA_MEMORY[i * 2 + 1] = terminal_background_color; // Set attribute
    }
	term_pos = 0;
	update_cursor();
	return;
}

void set_terminal_font_color(VGA_Color col) {
	terminal_font_color = col;
}

void set_terminal_background_color(VGA_Color col) {
	terminal_background_color = col;
}

void update_cursor() {
	uint16_t cursor_position = term_pos;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (cursor_position));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) (cursor_position >> 8));
	return;
}

