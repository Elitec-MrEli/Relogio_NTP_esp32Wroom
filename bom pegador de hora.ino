#include <WiFi.h>
#define NTP_SERVER     "a.ntp.br"
#define UTC_OFFSET      -10800
#define UTC_OFFSET_DST  0
struct tm timeinfo;
void printLocalTime() {
  timeinfo = {0};
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Connection Err");
    return;
  }
  Serial.println(&timeinfo);  
  Serial.println(&timeinfo, "%H:%M:%S");
  Serial.println(&timeinfo, "%d/%m/%Y   %Z -3");
}
void setup() {
  Serial.begin(9600);
  WiFi.begin("elitec_testes", "12345678", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}
void loop() {
  printLocalTime();
  delay(1000);
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}
