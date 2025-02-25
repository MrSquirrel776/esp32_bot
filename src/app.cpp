char* GetHtml() {return R"rawliteral(<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
    overflow: hidden;
    height: 100%;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
    touch-action: none;
    webkit-touch-action: none;
    moz-touch-action: none;
    ms-touch-action: none;
    o-touch-action: none;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
   .joystick {
    margin: none;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="content">
      <p><button id="button" class="button">Toggle</button></p>
      <canvas id="canvas" name="game" class="joystick"></canvas>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var xInput;
  var yInput;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
  }
  function onOpen(event) {
    log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
    log("page loaded");
  }
  function initButton() {
    console.log("button initialised");
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    console.log("Websocket message 'toggle' sent");
    websocket.send("toggle");
  }

  //---------------------------------------------------------------
  //                        JOYSTICK
  //---------------------------------------------------------------
</script>
<script>
  var canvas, ctx;

  window.addEventListener('load', () => {

      canvas = document.getElementById('canvas');
      ctx = canvas.getContext('2d');          
      resize(); 

      document.addEventListener('mousedown', startDrawing);
      document.addEventListener('mouseup', stopDrawing);
      document.addEventListener('mousemove', Draw);

      document.addEventListener('touchstart', startDrawing);
      document.addEventListener('touchend', stopDrawing);
      document.addEventListener('touchcancel', stopDrawing);
      document.addEventListener('touchmove', Draw);
      window.addEventListener('resize', resize);
  });

  var width, height, radius, x_orig, y_orig;

  function resize() {
      width = window.innerWidth;
      radius = 100;
      height = radius * 6.5;
      ctx.canvas.width = width;
      ctx.canvas.height = height;
      background();
      joystick(width / 2, height / 3);
  }

  function background() {
      x_orig = width / 2;
      y_orig = height / 3;

      ctx.beginPath();
      ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
      ctx.fillStyle = '#ECE5E5';
      ctx.fill();
  }

  function joystick(width, height) {
      ctx.beginPath();
      ctx.arc(width, height, radius, 0, Math.PI * 2, true);
      ctx.fillStyle = '#F08080';
      ctx.fill();
      ctx.strokeStyle = '#F6ABAB';
      ctx.lineWidth = 8;
      ctx.stroke();
  }

  let coord = { x: 0, y: 0 };
  let paint = false;

  function getPosition(event) {
      var mouse_x = event.clientX || event.touches[0].clientX;
      var mouse_y = event.clientY || event.touches[0].clientY;
      coord.x = mouse_x - canvas.offsetLeft;
      coord.y = mouse_y - canvas.offsetTop;
  }

  function is_it_in_the_circle() {
      var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
      if (radius >= current_radius) return true
      else return false
  }


  function startDrawing(event) {
      paint = true;
      getPosition(event);
      if (is_it_in_the_circle()) {
          ctx.clearRect(0, 0, canvas.width, canvas.height);
          background();
          joystick(coord.x, coord.y);
          Draw();
      }
  }


  function stopDrawing() {
      paint = false;
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      background();
      joystick(width / 2, height / 3);
  }

  function Draw(event) {

      if (paint) {
          ctx.clearRect(0, 0, canvas.width, canvas.height);
          background();
          var angle_in_degrees,x, y, speed;
          var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));

          if (Math.sign(angle) == -1) {
              angle_in_degrees = Math.round(-angle * 180 / Math.PI);
          }
          else {
              angle_in_degrees =Math.round( 360 - angle * 180 / Math.PI);
          }


          if (is_it_in_the_circle()) {
              joystick(coord.x, coord.y);
              x = coord.x;
              y = coord.y;
          }
          else {
              x = radius * Math.cos(angle) + x_orig;
              y = radius * Math.sin(angle) + y_orig;
              joystick(x, y);
          }

      
          getPosition(event);

          var speed =  Math.round(100 * Math.sqrt(Math.pow(x - x_orig, 2) + Math.pow(y - y_orig, 2)) / radius);

          var x_relative = Math.round(x - x_orig);
          var y_relative = Math.round(y - y_orig);

          send( x_relative,y_relative);
      }
  } 
  
  function send(x, y) {
    var data = {"x":Math.abs(x),"y":Math.abs(y), "xpos":(x > 0), "ypos":(y > 0)};
    data = JSON.stringify(data);
    console.log(typeof(data));
    console.log(data);
    websocket.send(data);
  }
</script>
</body>
</html>)rawliteral";}