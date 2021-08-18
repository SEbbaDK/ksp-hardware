#define RCS_SWITCH 8
#define RCS_LED 9

byte control_state = 0;
byte last_control_state = 0;

void setAllLEDs(bool to)
{
  digitalWrite(RCS_LED, to);
}

void writeControls()
{
  last_control_state = control_state;
  control_state = 0;
  if(!digitalRead(RCS_SWITCH))
    control_state |= 1;
  if (control_state != last_control_state)
    Serial.write(control_state);
}

void setup() {
  pinMode(RCS_SWITCH, INPUT_PULLUP);
  pinMode(RCS_LED, OUTPUT);
  digitalWrite(RCS_LED, HIGH);

  Serial.begin(115200);
  //Serial.print("K\n");

  setAllLEDs(HIGH);
  delay(1000);
  setAllLEDs(LOW);
}

void readSerial()
{
  if (Serial.read() == 1)
    digitalWrite(RCS_LED, HIGH);
  else
    digitalWrite(RCS_LED, LOW);
}

void loop()
{
  if (Serial.available())
    readSerial();
  writeControls();
}
