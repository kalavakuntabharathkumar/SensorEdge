package com.sensoredge.dashboard;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
@WebMvcTest(TelemetryController.class)
class TelemetryControllerTest {
 @Autowired MockMvc mvc;
 @Test void acceptsValidTelemetry() throws Exception {
  mvc.perform(post("/api/telemetry").contentType(MediaType.APPLICATION_JSON)
   .content("{\"timestampMs\":123,\"vibrationRaw\":20,\"temperatureRaw\":30,\"proximity\":1}"))
   .andExpect(status().isAccepted()).andExpect(jsonPath("$.vibrationRaw").value(20));
 }
 @Test void rejectsMissingFields() throws Exception {
  mvc.perform(post("/api/telemetry").contentType(MediaType.APPLICATION_JSON).content("{\"timestampMs\":123}"))
   .andExpect(status().isBadRequest());
 }
}
