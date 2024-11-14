#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc; // TinyRTC uses the DS1307 RTC chip


const int dataPins[4] = {6, 5, 4, 3}; // Data bus pins D0, D1, D2, D3
const int selectorPins[6] = { 7, 8, 9, 10, 11, 12}; // Selector pins for 6 flip-flops
const int ups = 100;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Initialize data bus pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(dataPins[i], OUTPUT);
  }
  randomSeed(analogRead(0));
  // Initialize selector pins as outputs
  for (int i = 0; i < 6; i++) {
    pinMode(selectorPins[i], OUTPUT);
  }

  // pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // sets the digital pin 13 on
  // digitalWrite(13, HIGH);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  DateTime now = rtc.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    rtc.adjust(DateTime(__DATE__, __TIME__));
}
  // startup();
}

void loop() {

  DateTime now = rtc.now(); // Get the current time

  // Extract hours, minutes, and seconds
  int hours = now.hour();
  int minutes = now.minute();
  int seconds = now.second();

  writeToNixie(hours / 10, 0);
  writeToNixie(hours % 10, 1);
  writeToNixie(minutes / 10, 2);
  writeToNixie(minutes % 10, 3);
  writeToNixie(seconds / 10, 4);
  writeToNixie(seconds % 10, 5);

  delay(1000/ups);
}



void writeToNixie(int num, int nixie){
  if (nixie != -1){
    int binaryArray[4];
    for (int i = 0; i < 4; i++) {
        // Shift the bits of the number to the right, then mask with 1 to get each bit
        binaryArray[3 - i] = (num >> i) & 1;
      }
    for (int i = 0; i < 4; i++) {
      if (binaryArray[i] == 1) {
        digitalWrite(dataPins[i], HIGH);
      } else {
        digitalWrite(dataPins[i], LOW);
      }
    }
    digitalWrite(selectorPins[nixie], HIGH); 
    digitalWrite(selectorPins[nixie], LOW);
    // Serial.println(selectorPins[nixie]);
    // Serial.println(num);
  }
}
void randomizeNixie(int nixies[]){
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 10; i++) {
      writeToNixie(i , nixies[1]);
      writeToNixie(i , nixies[2]);
      writeToNixie(i , nixies[3]);
      writeToNixie(i , nixies[4]);
      writeToNixie(i , nixies[5]);
      writeToNixie(i , nixies[0]);
      delay(15); 
    } 
  
  }
}

void shuffleArray(int array[], int size) {
  for (int i = size - 1; i > 0; i--) {
    // Generate a random index between 0 and i
    int j = random(0, i + 1);

    // Swap array[i] with array[j]
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void startup(){
  int nixs[6] = {0,1,2,3,4,5};
  int order[6] = {0,1,2,3,4,5};
  int counter = 0;
  shuffleArray(order, 6);
  bool done = false;

  while (done != true){
    DateTime now = rtc.now(); // Get the current time
    int rnd = random(0,2);
    if (rnd == 0){
      nixs[order[counter]] = -1;
      counter = counter + 1;
      
    }
    
    // Extract hours, minutes, and seconds
    int hours = now.hour();
    int minutes = now.minute();
    int seconds = now.second();

    // Display the hours, minutes, and seconds

    if (nixs[0] == -1){writeToNixie(hours / 10, 0);}
    if (nixs[1] == -1){writeToNixie(hours % 10, 1);}
    if (nixs[2] == -1){writeToNixie(minutes / 10, 2);}
    if (nixs[3] == -1){writeToNixie(minutes % 10, 3);}
    if (nixs[4] == -1){writeToNixie(seconds / 10, 4);}
    if (nixs[5] == -1){writeToNixie(seconds % 10, 5);}

    randomizeNixie(nixs);

    if (nixs[0] == -1){
      if (nixs[1] == -1){
        if (nixs[2] == -1){
          if (nixs[3] == -1){
            if (nixs[4] == -1){
              if (nixs[5] == -1){
                done = true;
                Serial.println("DONE");
              }
            }
          }
        }
      }
    }
  }
  
}