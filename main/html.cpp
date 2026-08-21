#include "web.h"

char html[] = R"rawliteral(
<!DOCTYPE html>
<html>

<link rel="stylesheet" type="text/css" href="/style.css">


<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Target</title>
</head>

<body>

<h1>Target</h1>


<div class="row">
<div class="column">
    <h2>Izquierda</h2>

    <button onclick="updateLeftPos(-10)">&#8810;</button>
    <button onclick="updateLeftPos(-1)"><</button>
    <input type="text" value="45" size="1" 
    id="leftPos" onchange="posChange('left')">
    <button onclick="updateLeftPos(1)">></button>
    <button onclick="updateLeftPos(10)">&#8811;</button>
    
    <br><br>

    <button onclick="armLeft()" class="dButton">Armar</button>
    <button onclick="fetch('/disarm')" class="dButton">Desarmar</button>

</div>
<div class="column">
    <h2>Derecha</h2>

    <button onclick="updateRightPos(-10)">&#8810;</button>
    <button onclick="updateRightPos(-1)"><</button>
    <input type="text" value="45" size="1"
    id="rightPos" onchange="posChange('right')">
    <button onclick="updateRightPos(1)">></button>
    <button onclick="updateRightPos(10)">&#8811;</button>

    <br><br>

    <button onclick="armRight()" class="dButton">Armar</button>
    <button onclick="fetch('/disarm')" class="dButton">Desarmar</button>

</div>
</div>

<hr>

<h2>Temporizado</h2>

    Esperar a armar <input type="text" id="before" value="5" size="3">s<br>
    Armar durante &nbsp; <input type="text" id="while" value="5" size="3">s<br>

    <br>

    <button onclick="timed('left')" class="dButton">
        Ejecutar Izq.
    </button>

    <button onclick="fetch('/stopTimed')">
        Parar
    </button>

    <button onclick="timed('right')" class="dButton">
        Ejecutar Der.
    </button>

<hr>

<h2>Ajustes piezo</h2>

    Piezo activo
    <label class="switch">
        <input type="checkbox" id="piezoSwitch" onchange="togglePiezo()">
        <span class="slider round"></span>
    </label>

    <br><br>

    Threshold: <input type="text" id="thresholdPiezo" value="100">

    <button onclick="updateThresholdPiezo()">
    Update
    </button>

<hr>

<h2>Ajustes sensor microondas</h2>

    Sensor activo
    <label class="switch">
        <input type="checkbox" id="radarSwitch" onchange="toggleRadar()">
        <span class="slider round"></span>
    </label>

<hr>

<h2>Ajustes BLE</h2>

    BLE activo
    <label class="switch">
        <input type="checkbox" id="bleSwitch" onchange="toggleBLE()">
        <span class="slider round"></span>
    </label>

    <br><br>

    Threshold: <input type="text" id="thresholdBLE" value="80">

    <button onclick="updateThresholdBLE()">
    Update
    </button>

<h3>Buscar BLEs</h3>

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