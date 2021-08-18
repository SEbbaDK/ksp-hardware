#include <font6x8.h>
#include <font4x6.h>
#include <fontALL.h>
#include <font8x8ext.h>
#include <font8x8.h>

#include <TVout.h>
#include <video_gen.h>

#include "logo.cpp"

#define WIDTH 128
#define HEIGHT 96
#define TV_SYNC 7
#define TV_SIGNAL 9

#define SPEAKER 11

// Selector on 2 to 5
#define SELECTOR 2
int selector = 0;

char vessel_name[16] = "No Vessel";

#define RESOURCE_COUNT 2
#define RESOURCE_NAME_LENGTH 16
char resource_names[RESOURCE_COUNT][RESOURCE_NAME_LENGTH];
float resource_values[RESOURCE_COUNT];

TVout TV;

int length(char* string)
{
	int i = 0;
	while (string[i] != '\0')
		i++;
	return i + 1;
}

void pin_setup()
{
	for(int i = 0; i < 4; i++)
		pinMode(SELECTOR + i, INPUT_PULLUP);
}

void old_startup_sequence()
{
	TV.print("KSP ");
	TV.tone(392,200);
	TV.delay(250);
	TV.print("Console ");
	TV.tone(440,200);
	TV.delay(250);
	TV.println("Mk1");
	TV.tone(523,200);
	TV.delay(250);
	TV.draw_line(0,10,128,10,WHITE);
	TV.set_cursor(0,16);
	TV.delay(250);
}

#define TONE_E 330
#define TONE_G 392
#define TONE_A 440
#define TONE_B 494
#define TONE_C 523
void play(int tone, int length)
{
	TV.tone(tone, length - 25);
	TV.delay(length);
}

void startup_sequence()
{
	int tempo = 1000;
	TV.bitmap(0,16,logo);
	play(TONE_G, tempo/4);
	play(TONE_C, tempo/2);
	play(TONE_G, tempo/4 + tempo/8);
	play(TONE_A, tempo/8);
	play(TONE_B, tempo/2);
	play(TONE_E, tempo/4);
}

void read_selector()
{
	int previous = selector;
	for(int i = 0; i < 4; i++)
		if(!digitalRead(SELECTOR + i))
			selector = i;
	if(selector != previous)
		TV.clear_screen();
}

void draw_bar(byte y, const char *text, float v)
{
	TV.set_cursor(0,y);
	TV.print(text);
	y += 9;
	TV.draw_rect(0,y, WIDTH,10, WHITE, WHITE);
	if (v != 1.0)
	{
		int sm_width = WIDTH - 6;
		float iv = 1.0 - v;
		TV.draw_rect(3 + sm_width - sm_width*iv,y+1, sm_width*iv,8, BLACK, BLACK);
	}
	if (v == 0.0)
	{
		TV.set_cursor(WIDTH/2-20,y+1);
		TV.print("EMPTY");
	}
	TV.set_cursor(0,y+16);
}

void display_status()
{
	TV.draw_rect(0,0,WIDTH-1,HEIGHT-1,WHITE);
	int offset = 8*length(vessel_name)/2;
	TV.set_cursor(2+(WIDTH/2-offset),2);
	TV.print(vessel_name);
	TV.draw_line(0,11,WIDTH,11,WHITE);
	TV.set_cursor(2,15);
	TV.println("Status: Nominal");
}

void display_resources()
{
    TV.println(resource_names[0]);
    for (int i = 0; i < RESOURCE_COUNT; i++)
        if (resource_names[i][0] != '\0')
        	draw_bar(25 * i, /*resource_names[i]*/"tom", resource_values[i]);
}

byte current_message = 0;
byte receive_index = 0; //Sent ID
byte receive_count = 0; //Amount of bytes (not counting header)
byte receive_float[4];
byte receive_byte() { receive_count += 1; return Serial.read(); }
void done_receiving() { current_message = 0; receive_count = 0; }
void receive()
{
	if (current_message == 0)
	{
		current_message = Serial.read();
	}
	else if (current_message == 1)
	{
		if (receive_count == 0)
		{
			receive_index = receive_byte();
			for (int i = 0; i < RESOURCE_NAME_LENGTH; i++)
					resource_names[receive_index][i] = '\0';
		}
		else
			resource_names[receive_index][receive_count - 1] = receive_byte();

    	// Stop once we've read the id + all the bytes in the name
		if (receive_count == RESOURCE_NAME_LENGTH)
    		done_receiving();
	}
	else if (current_message == 2)
	{
		if (receive_count == 0)
			receive_index = receive_byte();
		else
    		receive_float[receive_count - 1] = receive_byte();

    	// Stop once we've received the id + the 4 bytes of float
		if (receive_count == 4)
		{
    		done_receiving();
			resource_values[receive_index] = *((float*)receive_float);
		}
	}
	else if (current_message == 3)
	{
		for (int i = 0; i < RESOURCE_COUNT; i++)
		{
			resource_values[i] = 0.0;
			for (int c = 0; c < RESOURCE_NAME_LENGTH; c++)
    			resource_names[i][c] = '\0';
		}
		done_receiving();
	}
}

void setup()
{
	pin_setup();
	Serial.begin(115200);
	// put your setup code here, to run once:
	TV.begin(PAL, WIDTH, HEIGHT);
	TV.select_font(font8x8ext);
	startup_sequence();
}

void loop() {
	TV.delay(250);

	Serial.write('a');
	while (Serial.available())
		receive();
	if (((millis() / 100) % 50) == 0)
		TV.clear_screen();

	read_selector();
	if(selector==1)
		display_status();
	if(selector==2)
		display_resources();
}
