int incomingByte = 0;            // Received data byte
String inputString = "";         // Used to store received content
boolean newLineReceived = false; // Previous data end flag
boolean startBit  = false;       //Acceptance Agreement Start Sign
int num_reveice = 0;
int value = 270;
int fadeValue = 0;

bool fade = false;
bool state = false;



void setup() {
  Serial.begin(9600); //  set the baud rate to 9600
  pinMode(9,  OUTPUT); //red
  pinMode(10, OUTPUT); //blue
  pinMode(11, OUTPUT); //green
  digitalWrite(9, HIGH);
}




void loop() {
  readSerial();
  if (value == 0) {
    analogWrite(9, 255);
    analogWrite(10, 0);
    analogWrite(11, 0);
  }
  else if ((value > 0) && (value < 60)) {
    value = (value - 0) * 4;
    analogWrite(9, 255);
    analogWrite(10, value);
    analogWrite(11, 0);
  }
  else if (value == 60) {
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 0);
  }
  else if ((value > 60) && (value < 120)) {
    value = (value - 60) * 4;
    analogWrite(9, (255 - value));
    analogWrite(10, 255);
    analogWrite(11, 0);
  }
  else if (value == 120) {
    analogWrite(9, 0);
    analogWrite(10, 255);
    analogWrite(11, 0);
  }
  else if ((value > 120) && (value < 180)) {
    value = (value - 120) * 4;
    analogWrite(9, 0);
    analogWrite(10, 255);
    analogWrite(11, value);
  }
  else if (value == 180) {
    analogWrite(9, 0);
    analogWrite(10, 255);
    analogWrite(11, 255);
  }
  else if ((value > 180) && (value < 240)) {
    value = (value - 180) * 4;
    analogWrite(9, 0);
    analogWrite(10, (255 - value));
    analogWrite(11, 255);
  }
  else if (value == 240) {
    analogWrite(9, 0);
    analogWrite(10, 0);
    analogWrite(11, 255);
  }
  else if ((value > 240) && (value < 300)) {
    value = (value - 240) * 4;
    analogWrite(9, value);
    analogWrite(10, 0);
    analogWrite(11, 255);
  }
  else if (value == 300) {
    analogWrite(9, 255);
    analogWrite(10, 0);
    analogWrite(11, 255);
  }
  else if ((value > 300) && (value < 360)) {
    value = (value - 300) * 4;
    analogWrite(9, 255);
    analogWrite(10, 0);
    analogWrite(11, (255 - value));
  }
  if (value == 400) {
    fade = !fade;
    state = true;
    fadeValue = 0;
  }
  if ((state == true) && (fade == false)) {
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 255);
  }
  value = 999;
  if ((state == true) && (fade == true)) {
    fadeValue++;
    delay(15);
    if (fadeValue > 360){
      fadeValue = 0;
    }
    value = fadeValue;
  }
  
}




void readSerial() {
  while (Serial.available())
  {
    state = false;
    incomingByte = Serial.read();              //One byte by byte, the next sentence is read into a string array to form a completed packet
    if (incomingByte == '%') {
      num_reveice = 0;
      startBit = true;
    }
    if (startBit == true) {
      num_reveice++;
      inputString += (char) incomingByte;
    }
    if (startBit == true && incomingByte == '#') {
      newLineReceived = true;
      startBit = false;
    }
    if (newLineReceived) {
      inputString = inputString.substring(2, (inputString.length() - 1));
      value = inputString.toInt();
      inputString = "0";   // clear the string
      newLineReceived = false;
    }
  }
}
