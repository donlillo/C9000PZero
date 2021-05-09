#include "ads1292R.h"

uint8_t data[10];
uint8_t reset = 0;

#define RESET 4U
#define START 16U
#define LED 2U



void setup()
{
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(START, OUTPUT);
  pinMode(SS, OUTPUT);
  
  Serial.begin(115200);
  delay(100);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  ads1292Init(SS,RESET,START);
}


void loop()
{
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(SS, LOW);
  delay(1);
  SPI.transfer(0x20);
  SPI.transfer(0x09);

  for (int i = 0; i < sizeof(data); i++)
  {
    data[i] = SPI.transfer(0xff);
  }
  delay(1);
  digitalWrite(SS, HIGH);

  for (int i = 0; i < sizeof(data); i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  digitalWrite(LED, LOW);
  delay(500);
}