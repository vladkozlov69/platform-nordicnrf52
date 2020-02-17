..  Copyright 2014-present PlatformIO <contact@platformio.org>
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

How to build PlatformIO based project
=====================================

1. `Install PlatformIO Core <http://docs.platformio.org/page/core.html>`

## Build without SoftDevice & BLEPeripheral

I2C Master, SPI Master and UART have been tested working with a BME680 sensor.

.. code-block:: bash

    # Change directory to example
    > cd ../examples/arduino-pca10059

    # Build project
    > platformio run -e dongle

    # Upload firmware
    > platformio run -e dongle --target upload

    # Clean build files
    > platformio run -e dongle --target clean

## Build with SoftDevice S140 & BLEPeripheral

Only BLE Advertisement, Connect and Read/Write characteristic have been tested.

.. code-block:: bash

    # Change directory to example
    > cd ../examples/arduino-pca10059

    # Build project
    > platformio run -e ble

    # Upload firmware
    > platformio run -e ble --target upload

    # Clean build files
    > platformio run -e ble --target clean
