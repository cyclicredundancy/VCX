// Log the URLs we need
server.log("send key: " + http.agenturl() + "?key=<val>");
// http request handler
function requestHandler(request, response) {
  try {
    result <- "";
    flag <- 0;
    if ("key" in request.query) {
        local keyVal = request.query.key;
        server.log("send key: " + keyVal);
        device.send("key", keyVal); 
        result += "key value sent\n";
        flag = 1;
    }
    if ("enter" in request.query) {
        //passwordlocal keyVal = request.query.enter;
        device.send("enter", 0); 
        result += "enter value sent\n";
        server.log("send enter");
        flag = 1;
    }
    if (flag == 0) {
        result = "did you forget to add ?key=<val> or ?enter to your url?\n";
    }
    // send a response back saying everything was OK.
    response.send(200, result + "OK\n");
  } catch (ex) {
    response.send(500, "Internal Server Error: " + ex + "\n");
  }
}
// register the HTTP handler
http.onrequest(requestHandler);
