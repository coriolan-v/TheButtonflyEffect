
#define button_pin 10


String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

unsigned long stamp;

bool stampON = false;

unsigned int OffDelayMs = 2000;

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(button_pin, INPUT);

    inputString.reserve(200);
}

// the loop function runs over and over again forever
void loop() {

  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  // wait for a second


  if(millis() - stamp >= OffDelayMs && stampON == true){

    stampON = false;

    turnOff();

  }

  // delay(5000);

  // wait for a second
}

void turnOnSolid() {
  for (int i = 0; i < 4; i++) {
    pinMode(button_pin, OUTPUT);
    digitalWrite(button_pin, LOW);
    delay(40);
    pinMode(button_pin, INPUT);
    delay(40);
  }

  stamp = millis();
}

void turnOff(){
  pinMode(button_pin, OUTPUT);
    digitalWrite(button_pin, LOW);
    delay(100);
    pinMode(button_pin, INPUT);
    delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;

      Serial.print("input: ");  Serial.print(inputString);

      if(inputString.equals("A\n")){
        Serial.println("ON");

        stampON = true;

        turnOnSolid();
      }

      if(inputString.equals("B\n")){
        Serial.println("OFF");

        turnOff();
      }

      inputString == "";
    }
  }
}
