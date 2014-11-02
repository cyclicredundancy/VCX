// electric-imp device code for uart/serial communication with dfrobot PN532 nfc module to read mifare tags

server.log("Device starting.........................................");
 
// create a global variable called led, and assign pin9 to it
led <- hardware.pin9;
// configure led to be a digital output
led.configure(DIGITAL_OUT);
// blink the led continuously if thats what we are being told to do
blink <- 1;
function blink_handler(state) {
  if (blink) led.write(state);  
  else led.write(0);
  imp.wakeup(0.6, function() { blink_handler(1-state); });
} blink_handler(1);

// setup serial port to connect to NFC board
nfc_uart <- hardware.uart57;
// pin 5 is TX
// pin 7 is RX
nfc_uart.flush();

rxbuff <- [];

function nfc_rd() {
  local idx = 0;
  while(1) {
    local val = nfc_uart.read();
    if (val != -1) {
      //server.log("nfc_rd: " + format("%02d = 0x%02X", idx++, val) );
      rxbuff.push(val);
    } else {
      break;
    }
  }
}

nfc_uart.configure( 115200, 8, PARITY_NONE, 1, NO_CTSRTS);

function nfc_wr(cmd) {
  local msg = "";
  foreach(idx,val in cmd) {
    msg += format("0x%02X ", val);
    nfc_uart.write(val);
  }
  nfc_uart.flush();
  //server.log("init: writing " +format("%02d = 0x%02X", idx,val) ); 
  server.log("nfc_wr: "+msg);
}

function print_rxbuff() {
  local msg = "";
  foreach(idx,val in rxbuff) {
    msg += format("0x%02X ", val);
  }
  //server.log("init: writing " +format("%02d = 0x%02X", idx,val) ); 
  server.log("rxbuff: "+msg);
  rxbuff.clear();
}

function wait_ms(ms) {
  local endMillis = hardware.millis()+ms;
  while(hardware.millis() < endMillis);
}

const wakeup_cmd = "\x00\x00\x00\x00\x00\x00\xff\x03\xfd\xd4\x14\x01\x17\x00";
const firmware_cmd = "\x00\x00\xFF\x02\xFE\xD4\x02\x2A\x00";
const tag_cmd = "\x00\x00\xFF\x04\xFC\xD4\x4A\x01\x00\xE1\x00";

wait_ms(500);
nfc_wr(wakeup_cmd);
wait_ms(10);
nfc_rd();
print_rxbuff();

wait_ms(500);
nfc_wr(firmware_cmd);
wait_ms(10);
nfc_rd();
print_rxbuff();

wait_ms(500);
nfc_wr(tag_cmd);
wait_ms(40);
nfc_rd();
print_rxbuff();

//imp.wakeup(0.1, function() { nfc_read(); print_rxbuff(); });  
//imp.sleep(2);

