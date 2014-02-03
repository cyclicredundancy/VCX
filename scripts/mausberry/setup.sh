echo '#!/bin/bash


# Vikram:
# changed pin values to use gpio pins from P5 header 

# Detailed instructions:
# http://mausberry-circuits.myshopify.com/pages/setup

# original script:
# http://files.mausberrycircuits.com/setup.sh

# P5 header Pinouts:
# http://elinux.org/Rpi_Low-level_peripherals
#
# 3.3V <- 2 1 -> 5V  
# G29  <- 4 3 -> G28 
# G31  <- 6 5 -> G30 
# GND  <- 8 7 -> GND 

# P5 header pin positions, top side view, board edge:
#  ____________.
#        P1    |
#          1 2 |
#   P5     3 4 |
#     2 1  . . |
#     4 3  . . |
#     6 5  . . |
#     8 7  . . |
#          . . |
#
#################

#this is the GPIO pin connected to the lead on switch labeled OUT
#GPIOpin1=23
GPIOpin1=31

#this is the GPIO pin connected to the lead on switch labeled IN
#GPIOpin2=24
GPIOpin2=29

echo "$GPIOpin1" > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio$GPIOpin1/direction
echo "$GPIOpin2" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio$GPIOpin2/direction
echo "1" > /sys/class/gpio/gpio$GPIOpin2/value
while [ 1 = 1 ]; do
power=$(cat /sys/class/gpio/gpio$GPIOpin1/value)
if [ $power = 0 ]; then
sleep 1
else
sudo poweroff
fi
done' > /etc/switch.sh
sudo chmod 777 /etc/switch.sh
sudo sed -i '$ i /etc/switch.sh &' /etc/rc.local
