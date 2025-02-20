import { initializeApp } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-app.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-database.js";

const firebaseConfig = {
    apiKey: "AIzaSyA8OW9Svs7nLclPIfFC79k7EUM3tZeoli4",
    authDomain: "temperature-humidity-iot-eb42a.firebaseapp.com",
    databaseURL: "https://temperature-humidity-iot-eb42a-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "temperature-humidity-iot-eb42a",
    storageBucket: "temperature-humidity-iot-eb42a.firebasestorage.app",
    messagingSenderId: "787404772587",
    appId: "1:787404772587:web:ecbb553f26068b4a7e9aae"
};

// Inicijalizacija Firebase-a
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// Reference na Firebase baze
const humidityRef = ref(database, 'DHT_11/Humidity');
const temperatureRef = ref(database, 'DHT_11/Temperature');
const greenLedRef = ref(database, 'DHT_11/Green_LED_State');
const redLedRef = ref(database, 'DHT_11/Red_LED_State');
const buzzerRef = ref(database, 'DHT_11/Buzzer_State');

// Ažuriranje temperature
onValue(temperatureRef, (snapshot) => {
    const temperature = snapshot.val();
    document.getElementById('temp').innerHTML = temperature;
});

// Ažuriranje vlažnosti
onValue(humidityRef, (snapshot) => {
    const humidity = snapshot.val();
    document.getElementById('humidity').innerHTML = humidity;
});

// Ažuriranje zelene LED diode
onValue(greenLedRef, (snapshot) => {
    const greenLedState = snapshot.val();
    const greenLedElement = document.getElementById('light-green');

    if (greenLedState === "ON") {
        greenLedElement.style.backgroundColor = "#4caf50";
    } else {
        greenLedElement.style.backgroundColor = "gray";
    }
});

// Ažuriranje crvene LED diode
onValue(redLedRef, (snapshot) => {
    const redLedState = snapshot.val();
    const redLedElement = document.getElementById('light-red');

    if (redLedState === "ON") {
        redLedElement.style.backgroundColor = "#ff4d4d";
    } else {
        redLedElement.style.backgroundColor = "gray";
    }
});

// Ažuriranje buzzera
onValue(buzzerRef, (snapshot) => {
    const buzzerState = snapshot.val();
    const buzzerElement = document.getElementById('buzzer-indicator');
    const buzzerText = document.getElementById('buzzer-text');

    if (buzzerState === "ON") {
        buzzerElement.style.backgroundColor = "#F8FF7C";
        buzzerText.innerHTML = "ON";
    } else {
        buzzerElement.style.backgroundColor = "gray";
        buzzerText.innerHTML = "OFF";
    }
});
