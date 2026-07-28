#include "web.h"

char js[] = R"rawliteral(

function timed(){
    let bval = document.getElementById("before").value;
    let wval = document.getElementById("while").value;
    fetch("/timed?bval=" + bval + "&wval=" + wval);
}

function updateThresholdPiezo(){
    let val = document.getElementById("thresholdPiezo").value;
    fetch("/updateThresholdPiezo?value=" + val);
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