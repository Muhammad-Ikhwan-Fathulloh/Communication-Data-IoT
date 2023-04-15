#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST"; //Enter SSID
const char* password = ""; //Enter Password

using namespace websockets;

String channel_controlling = "my-controlling";

WebsocketsClient client;

void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }
    
    String JSON_Data = "{\"event\":";
           JSON_Data += "\"pusher:subscribe\",";
           JSON_Data += "\"data\":";
           JSON_Data += "{\"channel\":\"";
           JSON_Data += channel_controlling;
           JSON_Data += "\"}}";
    // try to connect to Websockets server
    bool connected = client.connect("ws://ws-ap1.pusher.com:80/app/APP_KEY?client=esp32&version=7.0.3&protocol=5");
    
    if(connected) {
        Serial.println("Connected!");
        Serial.println(JSON_Data);
        client.send(JSON_Data);
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() {
    // {"message": [{"control_1":"1","control_2":"1","control_3":"1","control_4":"1"}]}
    controlling();
}

void controlling(){
    if(client.available()) {
        bool control = client.poll();
        if(control) {
            Serial.println("Connected!");
            // Serial.println(client.poll());
            client.onMessage([&](WebsocketsMessage message){
                Serial.print("Got Message: ");
                char json[500];
                String payload = message.data();
                payload.toCharArray(json, 500);
                StaticJsonDocument<200> doc;
                deserializeJson(doc, json);
                Serial.println(payload);

                String payloads = doc["data"];
                Serial.println("Data : "+payloads);
                char jsons[500];
                payloads.toCharArray(jsons, 500);
                StaticJsonDocument<200> docs;
                deserializeJson(docs, jsons);
                String payloadx = docs["message"];
                int payloadt = sizeof(docs["message"]) ;
                Serial.println(payloadx);
                // Serial.println(payloadt);
                char jsonx[500];
                payloadx.toCharArray(jsonx, 500);
                StaticJsonDocument<200> docx;
                deserializeJson(docx, jsonx);
                    
                String control_1 = docx[0]["control_1"];
                String control_2 = docx[0]["control_2"];
                String control_3 = docx[0]["control_3"];
                String control_4 = docx[0]["control_4"];
                // Serial.println(payloads);
                Serial.println(payloadx);

                Serial.print("Control 1 : ");
                Serial.println(control_1);

                Serial.print("Control 2 : ");
                Serial.println(control_2);

                Serial.print("Control 3 : ");
                Serial.println(control_3);

                Serial.print("Control 4 : ");
                Serial.println(control_4);

                if(control_1 == "1"){
                    pinMode(2, OUTPUT);
                    digitalWrite(2, HIGH);
                }else{
                    pinMode(2, OUTPUT);
                    digitalWrite(2, LOW);
                }
                if(control_2 == "1"){
                    pinMode(4, OUTPUT);
                    digitalWrite(4, HIGH);
                }else{
                    pinMode(4, OUTPUT);
                    digitalWrite(4, LOW);
                }
                if(control_3 == "1"){
                    pinMode(5, OUTPUT);
                    digitalWrite(5, HIGH);
                }else{
                    pinMode(5, OUTPUT);
                    digitalWrite(5, LOW);
                }
                if(control_4 == "1"){
                    pinMode(18, OUTPUT);
                    digitalWrite(18, HIGH);
                }else{
                    pinMode(18, OUTPUT);
                    digitalWrite(18, LOW);
                }
            });
        } else {

        }
    }
}