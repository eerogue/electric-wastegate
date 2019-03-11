#include <Stepper.h>
#define STEPS 200

Stepper Mystepper(STEPS, 8, 9, 10, 11);

// the previous reading from the analog input
int previous = 0;
const int BP_SIGNAL = A0;
const int B_SIGNAL = A1;

float BACK_PRESSURE = 0.0;
float BACK_PRESSURE_OUT = 0.0;

float BOOST = 0.0;
float BOOST_OUT = 0.0;

float kohdearvo_BP = 0.0;
unsigned int muutettu_viimeksi_BP = 0;

float kohdearvo_B = 0.0;
unsigned int muutettu_viimeksi_B = 0;

// Funktio arvon siirtämiseen lähemmäs kohdetta, palauttaa muutetun tai annetun arvon
float ramp(float arvo, float kohde, float maxmuutos = 0.5) {
  // Jos erotus on riittävän suuri
  if(abs(arvo - kohde) > 1.0) {
    float erotus = arvo - kohde;
    // Siirrä arvoa vähän lähemäs kohdetta ja palauta se
    return arvo - (erotus * maxmuutos);  
  } else {
    // Arvo on hyvä, anna takaisin
    return arvo;
  }
}


void setup() {
    Serial.begin(9600);
  // set the speed of the motor (laskettu) käytännön kokeissa pienemmällä nopeudella
   Mystepper.setSpeed(288);

  muutettu_viimeksi_BP = millis();
  kohdearvo_BP = 4.5; //tavoitepaine bar

  muutettu_viimeksi_B = millis();
  kohdearvo_B = 4.0; //tavoitepaine bar

pinMode(BP_SIGNAL, INPUT);
pinMode(B_SIGNAL, INPUT);
}

void loop() {

  // Nykyisen arvon lukeminen sensoreilta
  BACK_PRESSURE = analogRead(BP_SIGNAL);
   float BP_VOLTAGE = BACK_PRESSURE * (5.0 / 1023.0);
   float BP_PRESS = map(BACK_PRESSURE, 102.3, 920.7, 0.0, 10.2);

  BOOST = analogRead(B_SIGNAL);
   float B_VOLTAGE = BOOST * (5.0 / 1023.0);
   float B_PRESS = map(BOOST, 102.3, 920.7, 0.0, 10.2);
  

  if (B_PRESS < 3.5){
    if(millis() > muutettu_viimeksi_BP + 20) {
      BACK_PRESSURE_OUT = ramp(BP_PRESS, kohdearvo_BP, 0.2);
      muutettu_viimeksi_BP = millis();
       Mystepper.step(-10); //hyppää 10 steppiä vastapäivään
    }
  
     if(millis() < muutettu_viimeksi_BP + 20) {
      BACK_PRESSURE_OUT = ramp(BP_PRESS, kohdearvo_BP, 0.2);
      muutettu_viimeksi_BP = millis();
       Mystepper.step(10); //hyppää 10 steppiä myötäpäivään
    }
  }
  
  if (BP_PRESS > 4.5){
    if(millis() > muutettu_viimeksi_BP + 20) {
      BACK_PRESSURE_OUT = ramp(BP_PRESS, kohdearvo_BP, 0.2);
      muutettu_viimeksi_BP = millis();
       Mystepper.step(-10); //hyppää 10 steppiä vastapäivään
    }
  
     if(millis() < muutettu_viimeksi_BP + 20) {
      BACK_PRESSURE_OUT = ramp(BP_PRESS, kohdearvo_BP, 0.2);
      muutettu_viimeksi_BP = millis();
       Mystepper.step(10); //hyppää 10 steppiä myötäpäivään
    }
  }
  
}
