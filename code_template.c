/*

Written by Montek Singh
Copyright and all rights reserved by Montek Singh
Last Updated:  April 4, 2025

Permission granted to use this only for students, teaching/learning assistants
and instructors of the COMP 541 course at UNC Chapel Hill.
For any other use, contact Montek Singh first.

*/


/*

This is a C template for initial development
of your demo app for COMP541 find projects!

You must compile and run this code in an ANSI
compatible terminal.  You can use the terminal app
in the course VM.  For macOS and Linux users, the
standard terminal/shell on your laptop is also ANSI
compatible.

Open a terminal and compile and run the code as follows:

	gcc code.c
	./a.out

*/



/* Specify the keys here that get_key() will look for,
returning 1 if the first key was found, 2, for the second key, etc.,
and returning 0 if none of these keys was found.
In the actual board-level implementation, you will define
scancodes instead of characters, and you can use specify 
key releases as well.
*/

int key_array[] = {'a', 'd', 'w', 's'}; 	// define as many as you need

/* Specify the keys here that get_key2() will look for. */

int key_array2[] = {'j', 'l', 'i', 'k'}; 	// define as many as you need


/* Let us define our sprites.  These will be text approximations
of the actual sprites used in the board implementation.
Here, each sprite is specified by:
	{ text character, foreground color, background color }

For example, specifying a sprite as
	{'.', white, red},
means it is drawn as a white dot over a red background.

Specify the number of sprites first (Nchars), and then the
attributes of each sprite.
*/

// type definition for emulating sprites (see below)
typedef struct {
	char char_to_display;
	int fg_color;
	int bg_color;
} sprite_attr;


#define Nchars 7

enum colors {black, red, green, yellow, blue, magenta, cyan, white};

sprite_attr sprite_attributes[Nchars] = {
	{' ', black, black},
	{'-', black, white},
	{' ', black, red},
	{' ', white, blue},
	{' ', magenta, magenta},
	{'0', white, blue},
	{'0', white, red}
};


//===============================================================
// Here are the functions available for I/O.  These correspond
// one-to-one to functions available in MIPS assembly in the
// helper files provided.
//
// NOTE:  There is one function specific to the C implementation
// that is not needed in the assembly implementation:
//     void initialize_IO(char* smem_initfile);
//===============================================================

void my_pause(int N);  	// N is hundredths of a second

void putChar_atXY(int charcode, int col, int row);
	// puts a character at screen location (X, Y)

int getChar_atXY(int col, int row);
	// gets the character from screen location (X, Y)

int get_key();
	// if a key has been pressed and it matches one of the
	// characters specified in key_array[], return the
	// index of the key in that array (starting with 1),
	// else return 0 if no valid key was pressed.

int get_key2();
	// similar to get_key(), but looks for key in
	// key_array2[].

int pause_and_getkey(int N);
	// RECOMMENDED!
	// combines pause(N) with get_key() to produce a 
	// *responsive* version of reading keyboard input

void pause_and_getkey_2player(int N, int* key1, int* key2);
	// 2-player version of pause_and_getkey().

int get_accel();
	// returns the accelerometer value:  accelX in bits [31:16], accelY in bits [15:0]
	// to emulate accelerometer, use the four arrow keys

int get_accelX();
	// returns X tilt value (increases back-to-front)

int get_accelY();
	// returns Y tilt value (increases right-to-left)

void put_sound(int period);
	// visually shows approximate sound tone generated
	// you will not hear a sound, but see the tone highlighted on a sound bar

void sound_off();
	// turns sound off

void put_leds(int pattern);
	// put_leds: set the LED lights to a specified pattern
	//   displays on row #31 (below the screen display)

void initialize_IO(char* smem_initfile);


//===============================================================
// This is the code for your demo app!
//===============================================================

int note_things[] = { // These are *halfnotes* above the actual notes, i.e., midpoints between consecutive notes
	935713, 883196, 833626, 786838, 742676, 700993, 661649, 624514, 589463, 556379, 525151, 495677, 	//A2-A#2 to G#3-A3
	467857, 441598, 416813, 393419, 371338, 350497, 330825, 312257, 294731, 278189, 262576, 247838, 	//A3-A#3 to G#4-A4
	233928, 220799, 208406, 196710, 185669, 175248, 165412, 156128 										//A4-A#4 to E5-F5
};

int game_song [] = {7, 7, 19, 19, 7, 17, 17, 7, 19, 19, 7, 21, 21, 7, 22, 22, 7, 7, 19, 19, 7, 17, 17, 7, 15, 14, 12, 10, 8, 7, 20, 19};



void countdown(){
	//3
	putChar_atXY(1, 20, 12); putChar_atXY(1, 21, 12);
								putChar_atXY(1, 21, 13);
	putChar_atXY(1, 20, 14); putChar_atXY(1, 21, 14);
								putChar_atXY(1, 21, 15);
	putChar_atXY(1, 20, 16); putChar_atXY(1, 21, 16);
	my_pause(100);

	//2
	putChar_atXY(0, 21, 15);
	putChar_atXY(1, 20, 15);
	my_pause(100);
	//1
	putChar_atXY(0, 20, 12);
	putChar_atXY(0, 20, 14);
	putChar_atXY(1, 21, 15);
	putChar_atXY(0, 20, 15);
	putChar_atXY(0, 20, 16);
	my_pause(100);

	//Ready for gameplay
	putChar_atXY(0, 21, 12);
	putChar_atXY(0, 21, 13);
	putChar_atXY(0, 21, 14);
	putChar_atXY(0, 21, 15);
	putChar_atXY(0, 21, 16);
}

void wipe_board(){
	for(int i = 1; i < 39; i++){
		for(int j = 1; j < 29; j++){
			putChar_atXY(0, i, j);
		}
	}
};

//p1 win wipe board
//p2 win wipe board

int p1_score = 0;
int p2_score = 0;

int p1_win_board[5][6] = {
	{0, 0, 0, 3, 0, 3},
	{0, 3, 0, 3, 0, 3},
	{0, 0, 0, 3, 0, 3},
	{0, 3, 3, 3, 0, 3},
	{0, 3, 3, 3, 0, 3}

};
int p2_win_board[5][6] = {
	{0, 0, 0, 2, 0, 0},
	{0, 2, 0, 2, 2, 0},
	{0, 0, 0, 2, 0, 0},
	{0, 2, 2, 2, 0, 2},
	{0, 2, 2, 2, 0, 0}
};

void p1_win_screen(){
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 30; j++){
			if(i > 15 && i < 22 && j > 11 && j < 17){
				putChar_atXY(p1_win_board[j - 12][i - 16], i, j);
			} else{
				putChar_atXY(3, i, j);
			}
		}
	}
	my_pause(100);
}

void p2_win_screen(){
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 30; j++){
			if(i > 15 && i < 22 && j > 11 && j < 17){
				putChar_atXY(p2_win_board[j - 12][i - 16], i, j);
			} else{
				putChar_atXY(2, i, j);
			}
		}
	}
	my_pause(100);
}

int score[4][5][2] = {
	{
		{0, 0},
		{0, 0},
		{1, 1},
		{0, 0},
		{0, 0}
	},
	{
		{0, 1},
		{0, 1},
		{0, 1},
		{0, 1},
		{0, 1}
	},
	{
		{1, 1},
		{0, 1},
		{1, 1},
		{1, 0},
		{1, 1}
	},
	{
		{1, 1},
		{0, 1},
		{1, 1},
		{0, 1},
		{1, 1}
	},

};

void score_display(){
	int col1 = 16;
	int col2 = 24;
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 5; j++){
			putChar_atXY(score[p1_score][j][i], i + col1, j + 12);
		}
	}
	putChar_atXY(1, 20, 14);
	putChar_atXY(1, 21, 14);
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 5; j++){
			putChar_atXY(score[p2_score][j][i], i + col2, j + 12);
		}
	}
	my_pause(150);
	wipe_board();

}

int p1_led_vals[] = {0, 32768, 16384, 8192, 4096};
int p2_led_vals[] = {0, 1, 2, 4, 8};
int led_val = 0;

int wincheck(int p1, int p2){
	sound_off();
		if(p1 & p2){
			return(1);
		} else if(p2){
			p1_score++;
			led_val = led_val + p1_led_vals[p1_score];
			return(1);
		}else{
		p2_score++; 
		led_val = led_val + p2_led_vals[p2_score];
		return(1);}	
};


void update_key(int *key, int newkey) {
	if(newkey != 0) {
		if(*key == 1 || *key == 2) {
			if(newkey == 3 || newkey == 4) *key = newkey;
		} else {
			if(newkey == 1 || newkey == 2) *key = newkey;
		}
	}
}



int game_loop(){
	int row1 = 15, row2 = 15;
	int col1 = 7, col2 = 33;

	int row1prev = 15, row2prev = 15;
	int col1prev = 7, col2prev = 33;

	int key1 = 2, key2 = 1;
	int key1new, key2new;

	int p1loss = 0;
	int p2loss = 0;
	int song_length = 64;
	int song_indexer = 0;
	
	score_display();
	countdown(); 

	while(1){
		song_indexer = (song_indexer + 1) & (song_length -1);
		put_sound(note_things[game_song[song_indexer >> 1]]);

		p1loss = getChar_atXY(col1, row1) != 0;
		p2loss = getChar_atXY(col2, row2) != 0;

		if(p1loss | p2loss){
			if(wincheck(p1loss, p2loss)){
				my_pause(100);
				wipe_board();
				return(1);
			}
		}

		
		
		putChar_atXY(3, col1prev, row1prev);
		putChar_atXY(2, col2prev, row2prev);

		putChar_atXY(5, col1, row1);
		putChar_atXY(6, col2, row2);

		if(col1 == col2 && row1 == row2){
			sound_off();
			my_pause(100);
			wipe_board();
			return(1);
		}
		
		pause_and_getkey_2player(10, &key1new, &key2new);
		update_key(&key1, key1new);
		update_key(&key2, key2new);

		col1prev = col1;
		col2prev = col2;
		row1prev = row1;
		row2prev = row2;
		
		switch(key1){
		case 1:
			col1--; 
			if(col1<0) col1=0; 
			break;
		case 2: 
			col1++; 
			if(col1>39) col1=39; 
			break;
		case 3: 
			row1--; 
			if(row1<0) row1=0;
			break;
		case 4: 
			row1++; 
			if(row1>29) row1=29; 
			break;
		}

		switch(key2){
			case 1: 
				col2--; 
				if(col2<0) col2=0; 
					break;
			case 2: 
				col2++; 
				if(col2>39) col2=39; 
				break;
			case 3: 
				row2--; 
				if(row2<0) row2=0;
				 break;
			case 4: 
				row2++; 
				if(row2>29) row2=29; 
				break;

		}
	}
}


int main() {
	initialize_IO("tron_board.mem");
	while(1){
	
	if(p1_score ==1){
		p1_win_screen();
		return(1);
	}
	if(p2_score == 1){
		p2_win_screen();
		return(1);
	}
	put_leds(led_val);
	game_loop();
}

}

// The file below has the implementation of all of the helper functions
#include "procs.c"
