#include <Arduino.h>

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
  uint32_t lastPressTime;  // Para el debounce
};
Button button1 = {18, 0, false, 0};

void IRAM_ATTR isr() {
  // Comprobamos si han pasado suficientes milisegundos desde la última pulsación registrada.
  uint32_t currentTime = millis();
  
  // Si el tiempo desde la última pulsación es mayor que 200 ms, contamos la pulsación.
  if (currentTime - button1.lastPressTime > 200) {
    button1.numberKeyPresses += 1;
    button1.pressed = true;
    button1.lastPressTime = currentTime;  // Actualizamos el tiempo de la última pulsación
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);  // Configura la interrupción en el flanco de bajada
}

void loop() {
  if (button1.pressed) {
    // Imprimimos el número de pulsaciones
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;
  }

  // Desconectar la interrupción después de 1 minuto
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
    Serial.println("Interrupt Detached!");
  }
}