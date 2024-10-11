var webSocket;

        // Connect to WebSocket when the page loads
        function openWebSocket()
        {
            webSocket = new WebSocket('ws://' + '10.1.224.85'+ ':80/');

            // Handle incoming messages (plain text potentiometer data)
            webSocket.onmessage = function(event) {
                document.getElementById('potValue').innerText = event.data;
            };

            // Log WebSocket status
            webSocket.onopen = function() {
                console.log('WebSocket connection opened.');
            };
            webSocket.onclose = function() {
                console.log('WebSocket connection closed.');
            };
        }

        