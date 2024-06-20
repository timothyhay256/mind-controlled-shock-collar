let myChart; // Make chart global
let maxValues = 50;
const canvas = document.querySelector("canvas"); // Set correct canvas size
canvas.width = window.innerWidth - (window.innerWidth * .4)
canvas.height = window.innerHeight - (window.innerHeight * .6)
Chart.defaults.color = '#232634';

window.onresize = function(){ location.reload(); }
console.log("Current threshold is: "+threshold);
window.addEventListener('resize', function(event) { // TODO: Fix
    console.log("Resize event!");
    canvas.width = window.innerWidth
    canvas.height = window.innerHeight
    // initialRender();
}, true);

async function fetchData() {
        const response = await fetch('/headset-data');
        const data = await response.json();
        return data;
}

async function updateChart() {
    const data = await fetchData();
    const latestData = data.slice(-maxValues);

    // const labels = latestData.map(item => item.index);
    // const strength = latestData.map(item => item.strength);
    // const attention = latestData.map(item => item.attention);
    const meditation = latestData.map(item => item.meditation);
    // const delta = latestData.map(item => item.delta);
    // const theta = latestData.map(item => item.theta);
    // const low_alpha = latestData.map(item => item.low_alpha);
    // const high_alpha = latestData.map(item => item.high_alpha);
    // const low_beta = latestData.map(item => item.low_beta);
    // const high_beta = latestData.map(item => item.high_beta);
    // const low_gamma = latestData.map(item => item.low_gamma);
    // const high_gamma = latestData.map(item => item.high_gamma);

    myChart.data.labels = latestData.map(item => item.index);
    myChart.data.datasets[0].data = latestData.map(item => item.strength);
    myChart.data.datasets[1].data = latestData.map(item => item.attention);
    myChart.data.datasets[2].data = latestData.map(item => item.meditation);
    myChart.data.datasets[3].data = Array.from({ length: meditation.length }, () => threshold);
    // myChart.data.datasets[3].data = latestData.map(item => item.delta);
    // myChart.data.datasets[4].data = latestData.map(item => item.theta);
    // myChart.data.datasets[5].data = latestData.map(item => item.low_alpha);
    // myChart.data.datasets[6].data = latestData.map(item => item.high_alpha);
    // myChart.data.datasets[7].data = latestData.map(item => item.low_beta);
    // myChart.data.datasets[8].data = latestData.map(item => item.high_beta);
    // myChart.data.datasets[9].data = latestData.map(item => item.low_gamma);
    // myChart.data.datasets[10].data = latestData.map(item => item.high_gamma);

    if (latestData.length >= maxValues) {
        myChart.update('none'); // Prevent animations when shifting the whole chart.
    } else {
        myChart.update();
    }
}

async function initialRender() {
    const data = await fetchData();
    const latestData = data.slice(-maxValues);

    const labels = latestData.map(item => item.index); 
    const strength = latestData.map(item => item.strength);
    const attention = latestData.map(item => item.attention);
    const meditation = latestData.map(item => item.meditation);
    const thresholdArr = Array.from({ length: meditation.length }, () => threshold);
    // const delta = latestData.map(item => item.delta);
    // const theta = latestData.map(item => item.theta);
    // const low_alpha = latestData.map(item => item.low_alpha);
    // const high_alpha = latestData.map(item => item.high_alpha);
    // const low_beta = latestData.map(item => item.low_beta);
    // const high_beta = latestData.map(item => item.high_beta);
    // const low_gamma = latestData.map(item => item.low_gamma);
    // const high_gamma = latestData.map(item => item.high_gamma);

    const ctx = document.getElementById('myChart').getContext('2d');
    myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Signal Strength',
                data: strength,
                borderWidth: 1,
                borderColor: 'orange', 
                fill: false
            },{
                label: 'Attention',
                data: attention,
                borderWidth: 1,
                borderColor: 'green', 
                fill: false
            },{
                label: 'Meditation',
                data: meditation,
                borderWidth: 1,
                borderColor: 'blue', 
                fill: false
            },{
                label: 'Threshold',
                data: thresholdArr,
                borderWidth: 1,
                borderColor: 'red', 
                fill: false
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });
}

function settingsPrompt() {
    maxValues = prompt("How many samples should be shown at once?", "50");
    if (maxValues == null || maxValues == "") {
        text = "User cancelled the prompt.";
    }
}

function home() {
    window.location.href = "/";
}

function sendShock(shockLvl) {
    fetch("/shock?power="+shockLvl)
  .then((response) => response.json())
  .then((json) => console.log(json));
}

function restartHeadset() {
    fetch("/restart-headset")
  .then((response) => response.json())
  .then((json) => console.log(json));
}

function eStop() {
    fetch("/stop")
  .then((response) => response.json())
  .then((json) => console.log(json));
}

initialRender();
setInterval(updateChart, 800);

