#!/bin/bash

echo this script should first update my installed packed using apt-get
echo and then install the packages that I need and are not installed by default
echo NOTE: assuming YES to all apt-get questions

sleep 3

#   update - Retrieve new lists of packages
sudo apt-get update  -y
#   upgrade - Perform an upgrade
sudo apt-get upgrade -y

sudo apt-get install -y \
        vim vim-gtk ctags git tcsh \
        python-setuptools python-dev python-rpi.gpio python-smbus \
        screen tightvncserver minicom \
        cpufrequtils i2c-tools \
        netatalk avahi-daemon nmap

sudo apt-get autoremove

# install python modules
sudo easy_install pyserial

# edit your hostname
#hostname

#$ sudo vim /etc/avahi/services/afpd.service
#<?xml version="1.0" standalone='no'?><!--*-nxml-*-->
# <!DOCTYPE service-group SYSTEM "avahi-service.dtd">
# <service-group>
# <name replace-wildcards="yes">%h</name>
# <service>
# <type>_afpovertcp._tcp</type>
# <port>548</port>
# </service>
# </service-group>
#
#$ sudo vim /etc/avahi/services/rfb.service
#<?xml version="1.0" standalone='no'?>
# <!DOCTYPE service-group SYSTEM "avahi-service.dtd">
# <service-group>
# <name replace-wildcards="yes">%h</name>
# <service>
# <type>_rfb._tcp</type>
# <port>5901</port>
# </service>
# </service-group>
#
#Note that rfb port 5901 implies :1 display.
#
#$ sudo /etc/init.d/avahi-daemon restart
