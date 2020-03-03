// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// This work file is based on the arduino-ble-led example, adapted for PCA10059 with external BME680.

#include <Arduino.h>

// Uncomment if you have a BME680 connected on I2C or SPI.
//define ENABLE_BME

// Uncomment for selecting BME mode
//#define BME_MODE I2C_STANDARD_MODE  // For I2C MODE
#define BME_MODE SS  // SPI MODE, standard Variant Chip Select pin

// LED pin
#define LED_PIN   LED_BUILTIN

#define R_LED_PIN   PIN_LED2_R
#define G_LED_PIN   PIN_LED2_G
#define B_LED_PIN   PIN_LED2_B

#if defined(ENABLE_BME)
#include "Zanshin_BME680.h"
BME680_Class BME680;
int32_t o_temp, temp;
int32_t o_hum, hum;
int32_t o_pres, pres;
int32_t o_gas, gas;
#endif

#if defined(ENABLE_BLE)
#include <BLEPeripheral.h>
#include <SPI.h>
BLEPeripheral blePeripheral = BLEPeripheral();
BLEService ledService = BLEService("19b10000e8f2537e4f6cd104768a1214");
BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("19b10001e8f2537e4f6cd104768a1214", BLERead | BLEWrite);
#endif

void setup() {
  Serial.begin(115200);
  Serial.println(F("Boot"));
  // set LED pins to output open drain mode
  pinMode(LED_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(R_LED_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(G_LED_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(B_LED_PIN, OUTPUT_OPEN_DRAIN);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(R_LED_PIN, LOW);
  digitalWrite(G_LED_PIN, LOW);
  digitalWrite(B_LED_PIN, LOW);

  #if defined(ENABLE_BME)
  Serial.print(F("BME680... "));
  if (BME680.begin(BME_MODE)) {
    Serial.println(F("OK"));
    BME680.setOversampling(TemperatureSensor,Oversample16);
    BME680.setOversampling(HumiditySensor,   Oversample16);
    BME680.setOversampling(PressureSensor,   Oversample16);
    Serial.println(F("- Setting IIR filter to a value of 4 samples"));
    BME680.setIIRFilter(IIR4);
    Serial.println(F("- Setting gas measurement to 320°C for 150ms"));
    BME680.setGas(320,150);
  } else {
    Serial.println(F("FAIL"));
  }
  #endif

  #if defined(ENABLE_BLE)
  // set advertised local name and service UUID
  blePeripheral.setLocalName("PCA10059");
  blePeripheral.setDeviceName("Green LED");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());
  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);
  // begin initialization
  blePeripheral.begin();
  Serial.println(F("BLE Peripheral Started!"));
  digitalWrite(LED_PIN, HIGH);
  #endif
}

uint8_t toggle = 0;

void loop() {

  #if defined(ENABLE_BLE)
  BLECentral central = blePeripheral.central();

  if (central) {
    // central connected to peripheral
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (central.connected()) {
      // central still connected to peripheral
      if (switchCharacteristic.written()) {
        // central wrote new value to characteristic, update LED
        if (switchCharacteristic.value()) {
          Serial.println(F("LED on"));
          digitalWrite(LED_PIN, LOW);
        } else {
          Serial.println(F("LED off"));
          digitalWrite(LED_PIN, HIGH);
        }
      }
    }

    // central disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  #endif

  toggle++;
  delay(250);
  #if !defined(ENABLE_BLE)
  digitalWrite(LED_PIN, toggle % 2);
  #endif
  digitalWrite(R_LED_PIN, toggle % 3);
  digitalWrite(G_LED_PIN, toggle % 4);
  digitalWrite(B_LED_PIN, toggle % 5);
  #if defined(ENABLE_BME)
  if(!(toggle % 4)) {
    BME680.getSensorData(temp, hum, pres, gas);
    if (o_temp != temp) {
      o_temp = temp;
      Serial.print(temp/100.0, 2);
      Serial.print(F("°C "));
      Serial.print(hum/1000.0, 2);
      Serial.print(F("%Hum "));
      Serial.print(pres/100.0, 2);
      Serial.print(F("hPa "));
      Serial.print(gas/100.0, 2);
      Serial.println(F("mOhm"));
    }
  }
  #endif
}