# Linux_Rep

At Frist open and terminal open one more ternimal in linux
opren Arduino ide and connect the esp32 and open Serial monitor
In enter command in Terminal 

mosquitto_sub -h 192.168.108.55 -t test/topic

in enter command in Ternima2

mosquitto_pub -h 192.168.108.55  -t test/topic -m "Hello, Mosquitto this is Shankar World"

It print the mosquitto_sub working terminal is running and esp32 send the Defaut Data for mosquitto_sub

change the msg in Arduino Serial monitor it updata msg send the mosquitto_sub start print 
do same in mosquitto_pub change the msg it updata in esp32 Serial monitor
