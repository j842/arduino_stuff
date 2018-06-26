#ifndef __JSCROLL__H
#define __JSCROLL__H

#include <MD_MAX72xx.h>
#include <SPI.h>
#include <string.h>

// Define the number of devices we have in the chain and the hardware interface
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

#define CLK_PIN     10  // or SCK
#define DATA_PIN    12  // or MOSI
#define CS_PIN      11  // or SS
#define SPEED_IN    A5
#define MAX_DEVICES  2

namespace jscroll
{
  void set(const char * s);
  void reset();
  void loop();
  void setup();
  bool isDone();
} // namespace


#endif
