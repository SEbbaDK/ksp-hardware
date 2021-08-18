#include <font6x8.h>
#include <font4x6.h>
#include <fontALL.h>
#include <font8x8ext.h>
#include <font8x8.h>

#include <TVout.h>
#include <video_gen.h>

TVout TV;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  TV.begin(PAL, 128, 96);
  TV.select_font(font8x8);
  TV.println("CRT-Term Mk1");
}

void loop() {
  if (Serial.available())
  {
    TV.print((char)Serial.read());
    TV.tone(800, 50);
  }
  delay(100);
}
