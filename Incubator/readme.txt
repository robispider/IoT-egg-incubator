embeded device codes here

This new update integrating conveying data collected by arduino due to esp32 which make them available for visualization via wifi on thingspeak
Upload 'IoT_Smart_Poultry_Arduino_V2.ino' on the Arduino Due, Choose for Board 'Arduino Due'
Upload 'IoT_Smart_Poultry_Esp_V2' on Esp32NodeMCU. Choose for Board: 'DOIT ESP32 DEVKIT V1'

FOR MQTT

mqtt_server "broker.hivemq.com"

SUSBCRIBE TO ALL TOPIC : smartpoultry/#

Specific TOPIC subscribe

      smartpoultry/temperatureF
      smartpoultry/temperatureC
      smartpoultry/humidity
      smartpoultry/fanState
      smartpoultry/eggRotatorState
     smartpoultry/windowsState
     smartpoultry/humidifierState
    smartpoultry/flameOdor
