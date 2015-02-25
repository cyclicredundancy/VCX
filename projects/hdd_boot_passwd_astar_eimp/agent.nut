// Log the URLs we need
server.log("send key: " + http.agenturl() + "?key=<val>");
// http request handler
function requestHandler(request, response) {
  try {
    result <- "";
    // check if the user sent led as a query parameter
    if ("key" in request.query)
    {
        local keyVal = request.query.key;
        // send "led" message to device, and send ledState as the data
        device.send("key", keyVal); 
        result = "key value sent\n";
    } else {
      result = "did you forget to add ?key=<val> to your url?\n";
    }
    // send a response back saying everything was OK.
    response.send(200, result + "OK");
  } catch (ex) {
    response.send(500, "Internal Server Error: " + ex);
  }
}
// register the HTTP handler
http.onrequest(requestHandler);
