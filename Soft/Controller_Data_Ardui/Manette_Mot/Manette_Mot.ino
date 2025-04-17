#include <Bluepad32.h>
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 17, 5);

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
    int joyY = rawJoyY - joyY_offset;
    int joyY_offset = 0;

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
    else {
        stepper.stop();
        Serial.println("Joystick neutre, moteur arrêté");
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

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
}

void loop() {
    BP32.update();
    processControllers();
    delay(50);  // un petit délai pour éviter de surcharger le port série
}
