import { sharedEventDispatcher } from "./event";
import * as ByteBuffer from "bytebuffer";

const NUS_SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const NUS_CHAR_TX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
const NUS_CHAR_RX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";

var nus_service;
var nus_char_rx;
var nus_char_tx;
var bluetoothDevice;

export function connect() {
    console.log("Starting Bluetooth connection...");

    return navigator.bluetooth.requestDevice({
        filters: [
            { services: [NUS_SERVICE_UUID] }
        ],
        optionalServices: [NUS_SERVICE_UUID]
    })
        .then(device => {
            bluetoothDevice = device;
            console.log("Device selected:", device);
            console.log('Connecting to GATT Server...');
            device.addEventListener('gattserverdisconnected', onDeviceDisconnected);
            return device.gatt.connect();
        })
        .then(server => {
            console.log('Getting Services...');
            return server.getPrimaryServices();
        })
        .then(services => {
            console.log('Getting Characteristics...');
            services.forEach(service => {
                if (service.uuid === NUS_SERVICE_UUID) {
                    nus_service = service;
                    console.log('> Found NUS Service:', service.uuid);
                }
            });
            return nus_service.getCharacteristics();
        })
        .then(characteristics => {
            characteristics.forEach(characteristic => {
                console.log('>> Characteristic:', characteristic.uuid, getSupportedProperties(characteristic));
                if (characteristic.uuid === NUS_CHAR_TX_UUID) {
                    nus_char_tx = characteristic;
                } else if (characteristic.uuid === NUS_CHAR_RX_UUID) {
                    nus_char_rx = characteristic;
                    characteristic.addEventListener('characteristicvaluechanged', onRxDataReceived);
                    characteristic.startNotifications();

                    // Request the maximum MTU size
                    console.log("Requesting maximum MTU...");
                    nus_service.getCharacteristics().then(() => {
                        console.log("Maximum MTU negotiation complete.");
                    }).catch(err => {
                        console.error("MTU negotiation failed:", err);
                    });
                }
            });
            console.log("Connected successfully!");
            sharedEventDispatcher().emit("ble_connected");
        })
        .catch(error => {
            console.error("Connection error:", error);
            sharedEventDispatcher().emit("ble_connect_error", error.message || error);
        });
}

export function disconnect() {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        console.log("Disconnecting...");
        bluetoothDevice.gatt.disconnect();
        bluetoothDevice = null;
    }
}

export async function tx_data(buff) {
    console.log("tx data:", ByteBuffer.wrap(buff).toDebug());

    // Throttle data transfer
    await delay(50); // 50ms delay between chunks

    // Retry logic
    return nus_char_tx.writeValue(buff).catch(err => {
        console.error("Transmission failed, retrying...", err);
        return nus_char_tx.writeValue(buff); // Retry once
    });
}

/* Utils */
function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function onRxDataReceived(event) {
    console.log("rx data:", ByteBuffer.wrap(event.target.value.buffer).toDebug());
    sharedEventDispatcher().emit("ble_rx_data", event.target.value.buffer);
}

function onDeviceDisconnected(event) {
    console.log("Device disconnected:", event);
    sharedEventDispatcher().emit("ble_disconnected");

    // Attempt reconnection
    console.log("Attempting to reconnect...");
    connect().then(() => {
        console.log("Reconnected successfully.");
    }).catch(err => {
        console.error("Reconnection failed:", err);
    });
}

function getSupportedProperties(characteristic) {
    let supportedProperties = [];
    for (const p in characteristic.properties) {
        if (characteristic.properties[p] === true) {
            supportedProperties.push(p.toUpperCase());
        }
    }
    return '[' + supportedProperties.join(', ') + ']';
}

function monitorSignalStrength() {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        bluetoothDevice.gatt.getPrimaryService(NUS_SERVICE_UUID)
            .then(service => service.getCharacteristic(NUS_CHAR_RX_UUID))
            .then(characteristic => {
                characteristic.readValue().then(value => {
                    const rssi = value.getInt8(0); // Assuming RSSI value is available
                    console.log("RSSI:", rssi);
                    if (rssi < -80) {
                        console.warn("Weak signal detected. Consider moving closer.");
                    }
                });
            })
            .catch(err => {
                console.error("RSSI monitoring failed:", err);
            });
    }
}
