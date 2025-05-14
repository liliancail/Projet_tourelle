
#include <Bluepad32.h>
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 21, 19);//Pin Step en premier puis 
AccelStepper stepper1(AccelStepper::DRIVER, 2, 4);
//AccelStepper stepper2(AccelStepper::DRIVER, 17, 5);
int in4Pin = 12;
int in3Pin = 14;
int enaPinB = 13;
int in1Pin = 26;
int in2Pin = 27;
int enaPinA = 25;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            myControllers[i] = ctl;
            Serial.printf("Controller connected at index %d\n", i);
            break;
        }
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            myControllers[i] = nullptr;
            Serial.printf("Controller disconnected from index %d\n", i);
            break;
        }
    }
}

void processGamepad(ControllerPtr ctl) {
    int rawJoyY = ctl->axisY();
    int rawJoyX = ctl->axisX();
    int Buttons = ctl->buttons();
    int joyY_offset = 0;
    int joyY = rawJoyY - joyY_offset;
    int joyX_offset = 0;
    int joyX = rawJoyX - joyX_offset;
  
    //Déplacement moteur plateau
    Serial.printf("Joystick Y brut: %d | calibré: %d\n", rawJoyY, joyY);
    if (joyY > 20) {
        stepper.setSpeed(500);
        while (joyY > 20) {
            stepper.runSpeed();
            Serial.printf("Running forward | Joystick Y: %d\n", joyY);
            
            BP32.update(); // actualise les manettes !
            rawJoyY = ctl->axisY();
            joyY = rawJoyY - joyY_offset;
        }
    }
    else if (joyY < -20) {
        stepper.setSpeed(-500);
        while (joyY < -20) {
            stepper.runSpeed();
            Serial.printf("Running backward | Joystick Y: %d\n", joyY);
            
            BP32.update(); // actualise les manettes !
            rawJoyY = ctl->axisY();
            joyY = rawJoyY - joyY_offset;
        }
    }
        stepper.stop();
        Serial.println("Joystick neutre, moteur arrêté");

    //Déplacement canon
    Serial.printf("Joystick X brut: %d | calibré: %d\n", rawJoyX, joyX);
    if (joyX > 20) {
        stepper1.setSpeed(500);
        while (joyX > 20) {
            stepper1.runSpeed();
            Serial.printf("Running forward | Joystick X: %d\n", joyX);
            
            BP32.update(); // actualise les manettes !
            rawJoyX = ctl->axisX();
            joyX = rawJoyX - joyX_offset;
        }
    }
    else if (joyX < -20) {
        stepper1.setSpeed(-500);
        while (joyX < -20) {
            stepper1.runSpeed();
            Serial.printf("Running backward | Joystick X: %d\n", joyX);
            
            BP32.update(); // actualise les manettes !
            rawJoyX = ctl->axisX();
            joyX = rawJoyX - joyX_offset;
        }
    }
        stepper1.stop();
        Serial.println("Joystick neutre, moteur arrêté");


    //Gestion du tir
    if (Buttons != 0){

    // Déclenchement des 2 moteurs CC
  Serial.printf("Tir !\n");
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enaPinB, 200);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPinA, 200);
  delay(2000);

  // Arrêt des 2 moteurs CC
  Serial.printf("Arrêt du tir\n");
  digitalWrite(enaPinB, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  digitalWrite(enaPinA, LOW);
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  
    }

}


void processControllers() {
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected() && ctl->hasData()) {
            if (ctl->isGamepad()) {
                processGamepad(ctl);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 + NEMA17 + PS4 Controller");

    stepper.setMaxSpeed(1500);     // max vitesse en pas/seconde
    stepper.setAcceleration(500); // accélération en pas/s²
    stepper1.setMaxSpeed(1500);     // max vitesse en pas/seconde
    stepper1.setAcceleration(500); // accélération en pas/s²

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();

    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    pinMode(enaPinB, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(enaPinA, OUTPUT);


}

void loop() {
    BP32.update();
    processControllers();
    delay(50);  // un petit délai pour éviter de surcharger le port série
}
Manette_Mot.ino
5 Ko