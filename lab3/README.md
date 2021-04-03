# mbed-os-wifi-socket-snake-game #

A classic snake game using Accelerometer and Wi-Fi for Mbed OS

##  Getting started ##

1. Import the example.

   ```
   mbed import lab3
   cd lab3
   ```
   
2. Configure the Wi-Fi shield to use.

   Edit ```mbed_app.json``` to include the correct Wi-Fi shield, SSID and password:

   ```
       "config": {
 	  "wifi-shield": {
               "help": "Options are WIFI_IDW0XX1",
               "value": "WIFI_IDW0XX1"
        	  },
           "wifi-ssid": {
               "help": "WiFi SSID",
               "value": "\"SSID\""
           },
           "wifi-password": {
               "help": "WiFi Password",
               "value": "\"Password\""
           }
       },
   ```

   Sample ```mbed_app.json``` files are provided for X-NUCLEO-IDW04A1 (```mbed_app_idw04a1.json```),  X-NUCLEO-IDW01M1 (```mbed_app_idw01m1```), DISCO_L475VG_IOT01A/ISM43362 (```mbed_app_ism43362.json```) and DISCO_F413ZH/ISM43362 (```mbed_app_ism43362.json```)
   
   For built-in Wi-Fi, ignore the value of `wifi-shield`.

3. Match host and port on server and client

   Edit HOST and PORT in ```main.cpp``` and ```echo_server.py``` to match the environment settings.

## How to play
1. Make sure pygame is installed. If not, open terminal and type in

   ```
   pip install pygame
   ```

2. Execute echo_server.py

   ```
   python echo_server.py
   ```

3. Move and tilt the STM32 board to try it out!!
   Have fun and enjoy :)))
