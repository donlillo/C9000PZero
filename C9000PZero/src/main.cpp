#include "ads1292R.h"

// GPIO define
#define RESETPIN 4U
#define STARTPIN 16U
#define LED 2U

const int sampleRate = SPS_125;
volatile byte RX_Buff[15];
volatile static int RX_Buff_Count = 0;
volatile char *RX_Buff_Ptr;

const boolean DebugEnable = true;

void ads1292Acquisition(const int SS, const int acquistionTime, const int sampleRate, const boolean DebugEnable)
{
  uint16_t acquisitionCount = acquistionTime * sampleRate;

  uint32_t delayTime = 100;
  if (sampleRate != 0)
  {
    delayTime = (1000 / sampleRate);
  }

  if (DebugEnable)
  {
    for (int j = 0; j < acquisitionCount; j++)
    {

      RX_Buff_Ptr = ads1292ReadData(SS);
      // Serial.print("*");
      // Serial.print("-");
      // Serial.print(millis());
      // Serial.print("-");
      for (int i = 0; i < 9; i++)
      {
        RX_Buff[RX_Buff_Count++] = *(RX_Buff_Ptr + i);
        char out[18];
        sprintf(out, "%02X", RX_Buff[i]);
        Serial.print(out);
      }

      RX_Buff_Count = 0;
      Serial.println();
      delay(delayTime);
    }
  }
}

void setup()
{
  // GPIO INIT:
  pinMode(LED, OUTPUT);
  pinMode(RESETPIN, OUTPUT);
  pinMode(STARTPIN, OUTPUT);
  pinMode(SS, OUTPUT);

  // Serial Init
  Serial.begin(115200);
  delay(100);

  // SPI Init
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  // ADS1292R Init
  if (ads1292Init(SS, RESETPIN, STARTPIN, sampleRate))
  {
    Serial.println("ADS1292 Initialization DONE");
    ads1292StartConversion(SS, STARTPIN);
  }
  else
  {
    Serial.println("ADS1292 Initialization FAILED");
    ads1292HardStop(STARTPIN);
  }
}

void loop()
{
  digitalWrite(LED, HIGH);
  ads1292Acquisition(SS, 1, sampleRate, DebugEnable);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);

  for(;;)
  {

  }
}
