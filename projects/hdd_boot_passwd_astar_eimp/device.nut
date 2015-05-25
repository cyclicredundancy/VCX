server.flush(60.0);
//server.disconnect();
// Change the WiFI config
//imp.setwificonfiguration(ssid, password);
// Attempt to reconnect synchronously
//server.connect();
server.log("Device starting.........................................\n");
server.log("Device's unique id: " + hardware.getdeviceid());
server.log("IMP-SW-Version: " + imp.getsoftwareversion());
server.log("Current ambient light level is: " + hardware.lightlevel());
server.log("The voltage on Pin 9 is: " + hardware.voltage());
// uart rx event handler
function tnsy_rx() {
  local dat = tnsy_uart.read();
  while(dat != -1) 
  {
    server.log("tnsy_rx: " + dat);
    dat = tnsy_uart.read();
  }
}
// get uart obj instance and configure it
tnsy_uart <- hardware.uart57;
tnsy_uart.configure(9600, 8, PARITY_NONE, 1, NO_CTSRTS, tnsy_rx);
// pin 5 is TX
// pin 7 is RX
tnsy_uart.flush();
// agent key message handler
function sendKey(keyVal) {
  server.log("sendKey : " + keyVal);
  //tnsy_uart.write(keyVal);
  tnsy_uart.write(keyVal);
}
// agent key message handler
function sendEnter(keyVal) {
  server.log("sendEnter");
  tnsy_uart.write(0xB0);
}
// register a handler for "key" messages from the agent
agent.on("key", sendKey);
agent.on("enter", sendEnter);
/*
// agent/server alive trace
function alive() {
  server.log("alive");
  imp.wakeup(10, alive);
}
// start the loop
alive();
*/
