# esphome-espnow-gateway2
How to communicate with multiple boards from esphome to espnow
An espnow gateway for receiving data from boards that transmit different structures!

![504-5_bb](https://user-images.githubusercontent.com/106683637/177980656-49b24f9a-d4ea-4a2b-97e5-bbb7e07e3da9.jpg)

Below is an explanation video! Sorry, it's in Korean!

[![Video Label](http://img.youtube.com/vi/0DWio4fnpP0/0.jpg)](https://youtu.be/0DWio4fnpP0?t=7935)

1. Upload nockanda_espnow.h to ESPHOME!
2. Register the board in ESPHOME, edit the contents of the 3.espnow_gateway.yaml file to suit yourself, and upload it!
4. Prepare the first board, connect dht11, and upload espnow_node1_dht11.ino file!
5. dht11 has 2 float data!
6. Prepare the second board, connect mhz19, and upload espnow_node2_mhz19.ino file!
7. mhz19 has 2 int data!
8. Prepare the third board, connect pms7003, and upload espnow_node3_pms7003.ino file!
9. pms7003 has 3 int data!
10. At this time, you must change the mac address of the gateway in the file to your own!
11. Upload sensor data with different data formats from 3 boards to Home Assistant!
12. Press a button in Home Assistant to turn the board's built-in LED on or off!
