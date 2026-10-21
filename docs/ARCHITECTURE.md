# Architecture
1. ESP32 samples sensor inputs and publishes telemetry over MQTT.
2. A production MQTT consumer should validate payload schema, freshness, device identity and message authentication.
3. Spring Boot exposes a minimal REST endpoint with bounded in-memory retention.
4. PostgreSQL settings are included, but persistence and MQTT broker consumption are not implemented yet.

Modbus RTU is not implemented: baud rate, parity, slave ID and register mapping depend on the actual PLC.
