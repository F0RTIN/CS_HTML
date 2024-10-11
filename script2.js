var webSocket;
    // Connect to WebSocket when the page loads

   
        function openWebSocket()
        {
            webSocket = new WebSocket('ws://' + '10.1.224.85'+ ':80/');

            // Log WebSocket status
            webSocket.onopen = function() {
                console.log('WebSocket connection opened.');
            };
            webSocket.onclose = function() {
                console.log('WebSocket connection closed.');
            };
        }
    
    function sendBuzz(command) {
        webSocket.send(command);
    }

    function sendLed(command)
    {
        webSocket.send(command);
    }

