/*
   FastDebouncer.hpp - Library for debouncing key event fast
   Create: Sunjun Kim, Nov 24, 2020
   License: GPL
*/

#ifndef FAST_DEBOUNCER_H
#define FAST_DEBOUNCER_H
#include "limits.h"
#include "Arduino.h"


class FastDebouncer
{
  private:
    uint32_t history; // buffer space for saving history of the values, 32-bit
    uint32_t mask;
    int last;

  public:
    FastDebouncer(int init_value, int counter_length)
    {
      // state history assignment
      (init_value == HIGH) ? history = 0xFFFFFFFF : history = 0;

      mask = 0;
      for (int i = 0; i < counter_length; i++)
      {
        mask = (mask << 1) + 1;
      }

      last = init_value;
    }

//   debugging purpose
//    void print_32_bin(uint32_t value)
//    {
//      for (int b = 32 - 1; b >= 0; b--)
//      {
//        Serial.print(bitRead(value, b));
//      }
//      Serial.println();
//    }
    
    // return true when a event is triggered.
    bool feed(int new_value)
    {
      bool stabilized = ((history & mask) == 0) || ((history & mask) == mask);
      bool last_bit_different = (history & 1) != (new_value & 1);
      bool report = false;


      if (
        // history is stabilized, and new value is incoming -> immediately raise a trigger event.
        (stabilized && last_bit_different)
        // when stabilized, last value should be same to the new value.
        // Otherwise, a button state is changed again within the debouncing period.
        // raise a trigger here also.
        || (stabilized && last != new_value)
      )
      {
        report = true;
        last = new_value;
      }

      history = (history << 1) + (new_value & 1); // make sure only the rightmost bit is added to the history

      return report;
    }
};

#endif
