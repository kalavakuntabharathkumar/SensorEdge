#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
constexpr int VIBRATION_PIN=34, TEMPERATURE_PIN=35, PROXIMITY_PIN=27;
constexpr uint32_t SAMPLE_PERIOD_MS=2; // 500 Hz target; measure actual timing.
struct Sample { uint32_t timestampMs; int vibrationRaw; int temperatureRaw; int proximity; };
QueueHandle_t sampleQueue;

void samplerTask(void*) {
  TickType_t lastWake=xTaskGetTickCount();
  const TickType_t period=pdMS_TO_TICKS(SAMPLE_PERIOD_MS);
  while(true) {
    Sample s{millis(),analogRead(VIBRATION_PIN),analogRead(TEMPERATURE_PIN),digitalRead(PROXIMITY_PIN)};
    if(sampleQueue && xQueueSend(sampleQueue,&s,0)!=pdPASS) {
      Sample discarded; xQueueReceive(sampleQueue,&discarded,0); xQueueSend(sampleQueue,&s,0);
    }
    vTaskDelayUntil(&lastWake,period);
  }
}
void connectWifi() {
  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  uint32_t start=millis();
  while(WiFi.status()!=WL_CONNECTED && millis()-start<15000) vTaskDelay(pdMS_TO_TICKS(250));
}
void reconnectMqtt() {
  if(mqtt.connected()) return;
  String id="SensorEdge-"+String((uint32_t)ESP.getEfuseMac(),HEX);
  if(mqtt.connect(id.c_str())) mqtt.subscribe("sensoredge/commands/#");
}
void setup() {
  Serial.begin(115200); pinMode(PROXIMITY_PIN,INPUT); analogReadResolution(12);
  sampleQueue=xQueueCreate(64,sizeof(Sample));
  xTaskCreatePinnedToCore(samplerTask,"sensorSampler",4096,nullptr,2,nullptr,1);
  connectWifi(); mqtt.setServer(MQTT_HOST,MQTT_PORT);
}
void loop() {
  if(WiFi.status()!=WL_CONNECTED) connectWifi();
  reconnectMqtt(); mqtt.loop();
  Sample s;
  if(sampleQueue && xQueueReceive(sampleQueue,&s,0)==pdPASS && mqtt.connected()) {
    char payload[180];
    snprintf(payload,sizeof(payload),"{\"timestamp_ms\":%lu,\"vibration_raw\":%d,\"temperature_raw\":%d,\"proximity\":%d}",
      (unsigned long)s.timestampMs,s.vibrationRaw,s.temperatureRaw,s.proximity);
    mqtt.publish(MQTT_TOPIC,payload);
  }
  delay(1);
}
