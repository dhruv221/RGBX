int incomingByte = 0;            // Received data byte
String inputString = "";         // Used to store received content
boolean newLineReceived = false; // Previous data end flag
boolean startBit  = false;       //Acceptance Agreement Start Sign
int num_reveice = 0;
String received_DATA = "";
int mode, intensity, wheel_rotation = 0;
int hue, i;
int R, G, B;
bool runfade = false;

void setup() {
  Serial.begin(9600); //  set the baud rate to 9600
  pinMode(9,  OUTPUT); //red
  pinMode(10, OUTPUT); //blue
  pinMode(11, OUTPUT); //green
  hue = 270;
  i = 100;
  getRGB();
  outRGB();
}

void loop() {
  readSerial();
  if (runfade == true) {
    hue += 1;
    if (hue > 360) {
      hue = 0;
    }
    getRGB();
    outRGB();
    delay(25);
  }
}

void getRGB() {
  if ((hue >= 0) && (hue < 60)) {
    R = 255;
    G = 0;
    B = 4.25 * hue;
  }
  else if ((hue >= 60) && (hue < 120)) {
    R = 510 - (4.25 * hue);
    G = 0;
    B = 255;
  }
  else if ((hue >= 120) && (hue < 180)) {
    R = 0;
    G = (4.25 * hue) - 510;
    B = 255;
  }
  else if ((hue >= 180) && (hue < 240)) {
    R = 0;
    G = 255;
    B = 1020 - (4.25 * hue);
  }
  else if ((hue >= 240) && (hue < 300)) {
    R = (4.25 * hue) - 1020;
    G = 255;
    B = 0;
  }
  else if ((hue >= 300) && (hue <= 360)) {
    R = 255;
    G = 1530 - (4.25 * hue);
    B = 0;
  }
}

void outRGB() {
  R = (i / 100.0) * R;
  G = (i / 100.0) * G;
  B = (i / 100.0) * B;
  analogWrite(9, R);
  analogWrite(11, G);
  analogWrite(10, B);
}


void on_receive() {
  mode           = received_DATA.substring(0, 1).toInt();
  intensity      = received_DATA.substring(2, received_DATA.lastIndexOf(":")).toInt();
  wheel_rotation = received_DATA.substring(received_DATA.lastIndexOf(":") + 1, received_DATA.length()).toInt();

  if (mode == 0) { //solid color
    runfade = false;
    hue = wheel_rotation;
    i   = intensity;
    getRGB();
    outRGB();
  }

  if (mode == 1) { //white
    runfade = false;
    R = 255;
    G = 255;
    B = 255;
    i = intensity;
    outRGB();
  }

  if (mode == 2) { //fade
    runfade = true;
    i = intensity;
    if (i < 10) {
      i = 10;
    }
  }
} // on_receive()

void readSerial() {
  while (Serial.available())
  {
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
      received_DATA = inputString.substring(1, (inputString.length() - 1));
      //Serial.println(received_DATA);
      inputString = "";   // clear the string
      newLineReceived = false;
      on_receive();
    }
  }
}
