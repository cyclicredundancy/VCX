// configure the imp (best practice)
imp.configure("LED Api", [], []);
 
// create a global variabled called led, 
// and assign pin9 to it
led <- hardware.pin9;
 
// configure led to be a digital output
led.configure(DIGITAL_OUT);
 
// create a global variable to store delay
// betweem led flashes
ledelay <- 0;

// function to specify delay between led flashes in ms
function setLed(ledState) {
  server.log("Previous delay in seconds" + ledelay);
  ledelay <- (ledState.tofloat()/1000);
  server.log("Delay in milliseconds " + ledState);
  server.log("Delay in seconds " + ledelay);
}

// create a global variable to store current
// state of the LED
state <- 0;
 
function blink() {
  // invert the value of state:
  // when state = 1, 1-1 = 0
  // when state = 0, 1-0 = 1
  state = 1-state;  
 
  // write current state to led pin
  led.write(state);

  if (ledelay == 0) {
    rdelay <- 1;
    rdelay = math.rand().tofloat()/RAND_MAX/10;
    //server.log("picked rand delay in seconds " + rdelay);    
    // schedule imp to wakeup
    imp.wakeup(rdelay, blink);
  } else {
    // schedule imp to wakeup
    imp.wakeup(ledelay, blink);
  }
}
 
// start the loop
blink();

// register a handler for "led" messages from the agent
agent.on("led", setLed);

