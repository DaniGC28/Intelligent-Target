#include "web.h"

char js[] = R"rawliteral(

async function loadPositions() {
    let response = await fetch("/getPos");
    let positions = await response.json();

    document.getElementById("leftPos").value = positions.left;
    document.getElementById("rightPos").value = positions.right;
}
loadPositions();
async function loadTogglePiezo() {
    let response = await fetch("/getTogglePiezo");
    let toggle = await response.text();

    document.getElementById("piezoSwitch").checked = toggle == "true";
}
loadTogglePiezo();
async function loadToggleRadar() {
    let response = await fetch("/getToggleRadar");
    let toggle = await response.text();

    document.getElementById("radarSwitch").checked = toggle == "true";
}
loadToggleRadar();
async function loadToggleBLE() {
    let response = await fetch("/getToggleBLE");
    let toggle = await response.text();

    document.getElementById("bleSwitch").checked = toggle == "true";
}
loadToggleBLE();

function updateLeftPos(step){
    let pos = document.getElementById("leftPos").value;
    pos = Number(pos)

    if (pos + step > 90 || pos + step < 0){
        if (step > 0){pos = 90;}
        else {pos = 0;}
    } else {pos = pos + step;}

    document.getElementById("leftPos").value = pos;
    posChange("left");
}
function updateRightPos(step){
    let pos = document.getElementById("rightPos").value;
    pos = Number(pos)

    if (pos + step > 90 || pos + step < 0){
        if (step > 0){pos = 90;}
        else {pos = 0;}
    } else {pos = pos + step;}

    document.getElementById("rightPos").value = pos;
    posChange("right");
}
function posChange(side){

    if (side == "right"){
        let pos = document.getElementById("rightPos").value;
        fetch("/updateRightPos?pos=" + pos);
    } else {
        let pos = document.getElementById("leftPos").value;
        fetch("/updateLeftPos?pos=" + pos);
    }

}

function armLeft(){
    let pos = document.getElementById("leftPos").value;
    fetch("/arm?pos=-" + pos);
}
function armRight(){
    let pos = document.getElementById("rightPos").value;
    fetch("/arm?pos=" + pos);
}



async function timed(side){
    let bval = document.getElementById("before").value;
    let wval = document.getElementById("while").value;
    let pos;

    if (side == "right"){
        pos = document.getElementById("rightPos").value;
    } else {
        pos = "-" + document.getElementById("leftPos").value;
    }

    let buttons = document.getElementsByClassName("dButton");

    for (let button of buttons) {
        button.disabled = true;
    }

    fetch("/timed?bval=" + bval + "&wval=" + wval + "&pos=" + pos);

    while (!done){
        let response = await fetch("/TimedStatus");
        let running = await response.text();

        if (running == 0){
            break;
        }

        await new Promise(resolve => setTimeout(resolve, 250));
    }

    for (let button of buttons) {
        button.disabled = false;
    }
}

function togglePiezo(){
    let val = document.getElementById("piezoSwitch").checked;
    
    fetch("/togglePiezo?value=" + val);
}
function updateThresholdPiezo(){
    let val = document.getElementById("thresholdPiezo").value;
    fetch("/updateThresholdPiezo?value=" + val);
}

function toggleRadar(){
    let val = document.getElementById("radarSwitch").checked;
    
    fetch("/toggleRadar?value=" + val);
}

function toggleBLE(){
    let val = document.getElementById("bleSwitch").checked;
    
    fetch("/toggleBLE?value=" + val);
}
function updateThresholdBLE(){
    let val = document.getElementById("thresholdBLE").value;
    fetch("/updateThresholdBLE?value=" + val);
}


let updating = false;
let interval;

async function startSearch(){

    await fetch("/startSearch");

    document.getElementById("popup").style.display = "block";

    updating = true;

    interval = setInterval(updateBeacons, 1000); 
}


async function updateBeacons(){

    if (!updating) return;

    let resposta = await fetch("/getBeacons");
    let beacons = await resposta.json();

    let list = document.getElementById("beaconList");
    list.innerHTML = "";

    beacons.forEach(b => {
        let li = document.createElement("li");
        let bu = document.createElement("button");

        bu.textContent = b.name + " | " + b.mac;
        bu.onclick = async function(){
            await fetch("/BLEselected?mac=" + b.mac);
            closePopup();
        }

        li.appendChild(bu);
        list.appendChild(li);
    });
}


async function closePopup(){

    updating = false;
    clearInterval(interval);

    document.getElementById("popup").style.display = "none";

    let resposta = await fetch("/stopSearch");
    let dades = await resposta.text();

    document.getElementById("selectedBLE").textContent = dades;

}

)rawliteral";