#include <Keyboard.h>
#include "FastDebouncer.hpp"

#define ON LOW
#define OFF HIGH

const int btn_pin_begin = 5;
const int led_pin_begin = 9;

const int num_btn = 3; // number of the buttons
enum button { BLUE, YELLOW, GREEN };
int FIRST = BLUE; // assign this to the first element of the enum above.
int LAST = GREEN; // assign this to the last element of the enum above.
int *keys[num_btn] = { // assign the keyboard keys to each buttons
  new int[2]{1, 'a'},
  new int[3]{2, ' ', 'w'},
  new int[2]{1, 'd'}
};

const int brightness_off = 0;       // brightness level at "off" state
const int brightness_dim = 50;      // brightness level at "dim" state
const int brightness_active = 255;  // brightness level at "active" state


// NON_USER_CONFIGURABLE AREA BELOW =============================
FastDebouncer *debouncers[num_btn];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Init the controller...");

  // put your setup code here, to run once:
  for (int i = FIRST; i <= LAST; i++) {
    int current_btn = btn_pin_begin + i;
    int current_led = led_pin_begin + i;

    pinMode(current_btn, INPUT_PULLUP); // internal pullup -> default HIGH, active at LOW (=short to GND)
    pinMode(current_led, OUTPUT);
    analogWrite(current_led, brightness_dim); // light on with DIM level
    debouncers[i] = new FastDebouncer(digitalRead(current_btn), 20); // fast debouncer: fast detecting change of the pin state
  }

  Keyboard.begin();

  Serial.println("Controller running.");
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = FIRST; i <= LAST; i++) {
    int current_btn = btn_pin_begin + i;
    int current_led = led_pin_begin + i;

    int btn_read = digitalRead(current_btn);
    bool event = debouncers[i] -> feed(btn_read);

    switch (event) {
      case false:
        break;
      case true:
        Serial.print(i);
        Serial.print('\t');
        Serial.println(btn_read);

        int* _k = keys[i];
        
        if(btn_read == ON) {
          for(int j=1;j<_k[0]+1;j++)
          {
            Keyboard.press(_k[j]);
          }
        }
        else { // btn_read == OFF
          for(int j=1;j<_k[0]+1;j++)
          {
            Keyboard.release(_k[j]);
          }          
        }
        break;
    }
  }
  //(btn_stat == LOW) ? analogWrite(led_pin_begin+i, brightness_active) : analogWrite(led_pin_begin+i, brightness_dim);
  delay(1);
}
