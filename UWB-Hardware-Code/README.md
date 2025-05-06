## Using hardware
> [!CAUTION]
> Besure to remove all of Anchor and Step down converter before wiring Controller to a computer.

This hardware does not require server connection to opreate.

Use ArduinoIDE to install this sofeware to microcontroller.
The list of microcontroller to config is
Controller: ESP32C6 Dev Module
Anchor and Tag: ESP32 Dev Module

## Server connection
This program require https connection and a domain name.

Follow those step to config server connection.
1. Download certificate chain of domain and paste its content to ```Controller/Certificate.h```

2. Open ```Controller/Secret.h``` and config SSID and PASSWORD of WIFI access point.

3. Open ```Controller/Controller.ino``` and change ```"https://project.your_domain.xyz/api/"``` to domain of Laravel server