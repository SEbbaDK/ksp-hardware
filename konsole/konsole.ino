#include <font6x8.h>
#include <font4x6.h>
#include <fontALL.h>
#include <font8x8ext.h>
#include <font8x8.h>

#include <TVout.h>
#include <video_gen.h>

#define WIDTH 128
#define HEIGHT 96
#define TV_SYNC 7
#define TV_SIGNAL 9

#define SPEAKER 11

TVout TV;

void startup_sequence()
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

void setup()
{
  Serial.begin(115200);
  // put your setup code here, to run once:
  TV.begin(PAL, WIDTH, HEIGHT);
  TV.select_font(font8x8);
  startup_sequence();
}

void draw_bar(byte y, float v)
{
  TV.draw_rect(0,y, v * WIDTH,10, WHITE, WHITE);
  TV.draw_line(0,y,WIDTH,y,WHITE);
  TV.draw_line(0,y+10,WIDTH,y+10,WHITE);
  TV.draw_rect(WIDTH-2,y, 2,10, WHITE, WHITE);
}

void display_fuel()
{
  int roller = (millis() / 100) % 100;
  float fuel = (float)(roller) / 100.0;
  TV.set_cursor(0,0);
  TV.print("Fuel");
  draw_bar(10, fuel);
}

void loop() {
  if (Serial.available())
  {
  }
  display_fuel();
  delay(100);
}
