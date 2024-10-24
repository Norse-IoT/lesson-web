#pragma once

const char rawWebpage[] = R"=====(<!DOCTYPE html>
<html>
  <head>
    <title>Page Title</title>
  </head>

  <body style="background-color: #eeeeee">
    <span style="color: #003366">
      <h1>LED Controller</h1>
      <form>
        <p>Select LED:</p>
        <div>
          <input type="radio" id="ID_LED_0" name="operation_mode" />
          <label for="ID_LED_0">LED 0</label>
          <input type="radio" id="ID_LED_1" name="operation_mode" />
          <label for="ID_LED_1">LED 1</label>
          <input type="radio" id="ID_LED_2" name="operation_mode" />
          <label for="ID_LED_2">LED 2</label>
        </div>
      </form>
      <br />Set intensity level: <br /><input
        type="range"
        min="1"
        max="100"
        value="50"
        class="slider"
        id="LED_INTENSITY"
      />Value: <span id="LED_VALUE">-</span><br />
    </span>
    <script>
      document
        .getElementById("ID_LED_0")
        .addEventListener("click", led_changed);
      document
        .getElementById("ID_LED_1")
        .addEventListener("click", led_changed);
      document
        .getElementById("ID_LED_2")
        .addEventListener("click", led_changed);
      var slider = document.getElementById("LED_INTENSITY");
      var output = document.getElementById("LED_VALUE");
      slider.addEventListener("change", slider_changed);
      var Socket;
      function init() {
        Socket = new WebSocket("ws://" + window.location.hostname + ":81/");
        Socket.onmessage = function (event) {
          processCommand(event);
        };
      }
      function led_changed() {
        var l_LED_selected = 0;
        if (document.getElementById("ID_LED_1").checked == true) {
          l_LED_selected = 1;
        } else if (document.getElementById("ID_LED_2").checked == true) {
          l_LED_selected = 2;
        }
        console.log(l_LED_selected);
        var msg = { type: "LED_selected", value: l_LED_selected };
        Socket.send(JSON.stringify(msg));
      }
      function slider_changed() {
        var l_LED_intensity = slider.value;
        console.log(l_LED_intensity);
        var msg = { type: "LED_intensity", value: l_LED_intensity };
        Socket.send(JSON.stringify(msg));
      }
      function processCommand(event) {
        var obj = JSON.parse(event.data);
        var type = obj.type;
        if (type.localeCompare("LED_intensity") == 0) {
          var l_LED_intensity = parseInt(obj.value);
          console.log(l_LED_intensity);
          slider.value = l_LED_intensity;
          output.innerHTML = l_LED_intensity;
        } else if (type.localeCompare("LED_selected") == 0) {
          var l_LED_selected = parseInt(obj.value);
          console.log(l_LED_selected);
          if (l_LED_selected == 0) {
            document.getElementById("ID_LED_0").checked = true;
          } else if (l_LED_selected == 1) {
            document.getElementById("ID_LED_1").checked = true;
          } else if (l_LED_selected == 2) {
            document.getElementById("ID_LED_2").checked = true;
          }
        }
      }
      window.onload = function (event) {
        init();
      };
    </script>
  </body>
</html>)=====";
