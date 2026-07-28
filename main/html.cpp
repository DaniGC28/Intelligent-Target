#include "web.h"

char html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Target</title>
</head>

<body>

<h1>Target</h1>

<button onclick="fetch('/arm')">
Armar
</button>
<button onclick="fetch('/disarm')">
Desarmar
</button>

<hr>

<h2>Temporizar</h2>

Esperar <input type="text" id="before" value="5">s<br>
Armar durante <input type="text" id="while" value="5">s<br>

<button onclick="timed()">
Run
</button>

<hr>

<h2>Variar sensibilidad piezo</h2>

Threshold: <input type="text" id="thresholdPiezo" value="100">

<button onclick="updateThresholdPiezo()">
Update
</button>

<hr>

<h2>Variar sensibilidad BLE</h2>

Threshold: <input type="text" id="thresholdBLE" value="80">

<button onclick="updateThresholdBLE()">
Update
</button>

<h2>Buscar BLEs</h2>

<p>
BLE seleccionado: <span id="selectedBLE">---</span>
</p>

<button onclick="startSearch()">
Buscar
</button>

<div id="popup" style="display:none;">
    <h3>Beacons trobats</h3>
    <ul id="beaconList"></ul>
    <button onclick="closePopup()">Tancar</button>
</div>

<scipt src="/script.js"></script>

</body>
</html>
)rawliteral";