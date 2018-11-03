#include <stdint.h>
#include "pla.h"

void
setup() {
  // 0 = in, 1 = out
  DDRA = 0xff;
  DDRC = 0xff;
  DDRL = 0x00;
  // PA7 (29) 1------- I7  // PC7 (30) 1------- I15  // PL7 (42) 0------- F7
  // PA6 (28) -1------ I6  // PC6 (31) -1------ I14  // PL6 (43) -0------ F6
  // PA5 (27) --1----- I5  // PC5 (32) --1----- I13  // PL5 (44) --0----- F5
  // PA4 (26) ---1---- I4  // PC4 (33) ---1---- I12  // PL4 (45) ---0---- F4
  // PA3 (25) ----1--- I3  // PC3 (34) ----1--- I11  // PL3 (46) ----0--- F3
  // PA2 (24) -----1-- I2  // PC2 (35) -----1-- I10  // PL2 (47) -----0-- F2
  // PA1 (23) ------1- I1  // PC1 (36) ------1- I9   // PL1 (48) ------0- F1
  // PA0 (22) -------1 I0  // PC0 (37) -------1 I8   // PL0 (49) -------0 F0

  // if DDRD == 0 (in)  -> 0 = no pull-up, 1 = pull-up
  // if DDRD == 1 (out) -> 0 = low output, 1 = high output
  PORTA = 0x0;
  PORTC = 0x0;
  PORTL = 0x0;

  Serial.begin(9600);
}

void
loop() {
  Serial.print("start...\n");
  seq_test();
  Serial.print("completed!\n");
  delay(3000);
}

void
seq_test() {
  uint8_t plaValue;
  uint8_t goodValue;

  do {
    plaValue = peek(addr);

    goodValue = F7;
    goodValue = (goodValue << 1) | F6;
    goodValue = (goodValue << 1) | F5;
    goodValue = (goodValue << 1) | F4;
    goodValue = (goodValue << 1) | F3;
    goodValue = (goodValue << 1) | F2;
    goodValue = (goodValue << 1) | F1;
    goodValue = (goodValue << 1) | F0;

    if (plaValue != goodValue) {
      err(plaValue, goodValue);
    }

    //Serial.print(addr, HEX);
    //Serial.print(": ");
    //Serial.print(PINL, HEX);
    //Serial.print("\n");

    ++addr;
  } while (addr);
}

inline uint8_t
peek(uint16_t a) {
  PORTA = a & 0xff;
  PORTC = (a >> 8) & 0xff;

  asm("nop"); // 62.5 ns
//  asm("nop"); // 62.5 ns

  return PINL;
}

void
err(uint8_t plaValue, uint8_t goodValue) {
  Serial.print("ERROR: input: ");
  Serial.print(addr, HEX);
  Serial.print(", output: ");
  Serial.print(plaValue, HEX);
  Serial.print(", should be: ");
  Serial.print(goodValue, HEX);
  Serial.print("\n");

  // flash pin
  digitalWrite(13, LOW);
  delay(100);
  for (int i = 0; i < 10; ++i) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(100);
  }
}
