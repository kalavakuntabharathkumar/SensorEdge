# SensorEdge — Embedded Conveyor Monitoring and Automation

Starter project showing ESP32 sensor sampling, MQTT telemetry, and a Java Spring Boot REST endpoint.

## Firmware
Open `firmware/platformio.ini` with PlatformIO. Copy `firmware/include/secrets.example.h` to `secrets.h` and enter local credentials. Never commit secrets.

Pins and analog readings are examples and require calibration for real sensors. Modbus RTU/PLC integration is documented as an adapter boundary because register maps depend on the hardware.

## Dashboard
Requires Java 17+, Maven and PostgreSQL configuration. From `dashboard/` run:
```bash
mvn spring-boot:run
```
The sample endpoint keeps recent records in memory; PostgreSQL is configured but persistence/MQTT broker consumption remain follow-up tasks.

## Safety
This is a learning scaffold, not a validated industrial controller. Do not connect to live equipment without qualified controls engineering, safety interlocks and hardware-in-the-loop testing. The metrics in the original project description are not verified by this code.
