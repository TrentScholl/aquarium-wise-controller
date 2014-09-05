Hardware
=========

Arduino Mega 2560 R3
--------------------

.. image:: /img/ArduinoMega2560_R3_Front_450px.jpg

The `Arduino Mega 2560 <http://arduino.cc/en/Main/ArduinoBoardMega2560>`_ is a microcontroller board based on the ATmega2560. Although it is recommended that you purchase an official board from an Arduino reseller, there are many cheaper clones available that are suitable to use for the Aquarium Wise Controller.

It is important that you purchase the Arduino Mega 2560 R3 or equivalent clone as all of the documentation and software is designed specifically for it.

SainSmart 3.2” SSD1289 TFT Touch Screen
---------------------------------------

.. image:: /img/touch_screen.jpg

The SainSmart 3.2" TFT Touch screen is a LCD screen module with a SSD1289 controller. It also includes and SD card reader - however, this is not currently used by any of the functionality in the Aquarium Wise Controller.

This component can be replaced with any 320 x 240 LCD module that is supported by the UTFT library. The modifications required for using a different screen can be found in the Software section of this documentation.

SainSmart TFT LCD Adjustable Shield 
-----------------------------------

.. image:: /img/tft_shield.jpg

Although not required, the SainSmart TFT LCD Adjustable Shield makes the process of hooking up the 3.2" TFT to the Arduino much simpler. Some modifications are required if you wish to use it as a straight plug-in for the Arduino. These modifications are located further in to the documentation.

Atlas Scientific pH Kit
-------

.. image:: /img/ph_kit.jpg

The `Atlas Scientific pH Kit <https://www.atlas-scientific.com/product_pages/kits/ph_kit.html>`_ comes with everything you need to get your Aquarium Wise controller up and monitoring pH levels in your aquarium.

The software is writen specially to work with the Atlas Scientific pH stamp, but can be easily modified to us a DIY pH solution. However, it is highly recommended to use the Atlas Scientific kit.

`Datasheet <https://www.atlas-scientific.com/_files/_.../_circuit/pH_Circuit_5.0.pdf>`_

SainSmart 8 Channel DC 5V Relay Module
--------------------------------------

.. image:: /img/relay_board.jpg

The SainSmart 8 Channel Relay Module is used to control the power outlets for lights, filters, heaters, etc.. Any relay board can be used in place of the SainSmart board - some software changes may be required depending on what signal the relay board is expecting (Some expect HIGH for off and others expect LOW for on).

DS3231
------

.. image:: /img/DS3231.jpg

The `DS3231 <http://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html>`_ is a low-cost, extremely accurate I²C real-time clock (RTC) with an integrated temperature-compensated crystal oscillator (TCXO) and crystal.

Purchasing a pre-made PCB with components is the simplest and quickest way to get your RTC up and running.

`Datasheet <datasheets.maximintegrated.com/en/ds/DS3231.pdf>`_