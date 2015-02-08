#!/bin/bash

ssh vdhar@192.168.1.64 -t -L 5900:localhost:5900 'sudo x11vnc -ncache 10 -nevershared -localhost -usepw -forever -rfbauth ~/.vnc/passwd -auth /var/run/lightdm/root/:0 -display :0'

