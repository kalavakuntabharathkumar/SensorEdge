package com.sensoredge.dashboard;
import java.time.Instant;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedDeque;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/telemetry")
public class TelemetryController {
 private final ConcurrentLinkedDeque<Map<String,Object>> recent=new ConcurrentLinkedDeque<>();
 private static final int MAX_RECORDS=500;
 @PostMapping
 public ResponseEntity<Map<String,Object>> ingest(@RequestBody TelemetryRequest r) {
  if(r==null || r.vibrationRaw()==null || r.temperatureRaw()==null || r.proximity()==null)
   return ResponseEntity.badRequest().body(Map.of("error","All sensor fields are required"));
  Map<String,Object> row=Map.of("receivedAt",Instant.now().toString(),
   "timestampMs",r.timestampMs()==null?0L:r.timestampMs(),"vibrationRaw",r.vibrationRaw(),
   "temperatureRaw",r.temperatureRaw(),"proximity",r.proximity());
  recent.addFirst(row); while(recent.size()>MAX_RECORDS) recent.pollLast();
  return ResponseEntity.accepted().body(row);
 }
 @GetMapping public Map<String,Object> latest() { return Map.of("count",recent.size(),"records",recent); }
 public record TelemetryRequest(Long timestampMs,Integer vibrationRaw,Integer temperatureRaw,Integer proximity) {}
}
