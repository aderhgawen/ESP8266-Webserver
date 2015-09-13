# ESP8266-Webserver
This Arduino sketch starts the ESP8266 as an access point, and provides a web interface to configure WiFi credentials and control the ESP'S GPIO pins. This code relies on the [ESP8266 Arduino core](https://github.com/esp8266/Arduino), an excellent Arduino library for programming the ESP8266.

Main page:

![](http://s23.postimg.org/6s7t24mrf/esp_main.png)

WiFi settings page:

![](http://s23.postimg.org/h3k5usegr/esp_wlan.png)

### Installing the ESP8266 Arduino Core

Starting with 1.6.4, Arduino allows installation of third-party platform packages using Boards Manager. The ESP8266 Arduino code is available for Windows, Mac OS, and Linux (32 and 64 bit).

- Install Arduino 1.6.5 from the [Arduino website](http://www.arduino.cc/en/main/software).
- Start Arduino and open Preferences window.
- Enter ```http://arduino.esp8266.com/stable/package_esp8266com_index.json``` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).
