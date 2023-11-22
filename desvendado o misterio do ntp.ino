
#include <NTPClient.h> 
#include <WiFi.h>

const char ssid[] = "elitec_testes";
const char password[] = "12345678";

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando no WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConectado!\n");
  
  ntp.setTimeOffset(-10800);
  ntp.begin();  
  
}

void loop() {
    Serial.print("ONa: ");
    Serial.print(millis());


    if ((millis() >= 30000)&&(millis() >= 32000)){ ntp.setPoolServerName("pool.ntp.org"); }
    if ((millis() >= 50000)&&(millis() >= 52000)){ ntp.setPoolServerName("pool.ntp.org_off"); }  
    if ((millis() >= 70000)&&(millis() >= 72000)){ ntp.setPoolServerName("gps.ntp.br"); }   

       
  if (ntp.forceUpdate()) {   

    Serial.print(" HORARIO: ");
    Serial.print(ntp.getFormattedTime());

    Serial.print(" H: ");
    Serial.print(ntp.getHours());

    Serial.print(" M: ");
    Serial.print(ntp.getMinutes());

    Serial.print(" S: ");
    Serial.print(ntp.getSeconds());

    Serial.print(" D.SEM(0=domingo): ");
    Serial.print(ntp.getDay());

    Serial.print(" t.stmp(desde.01/01/1970): ");
    Serial.print(ntp.getEpochTime());

    Serial.println();

  } else {
    Serial.print(" !Erro ao atualizar NTP! ");

    Serial.println();
  }

   if (ntp.update()) {  
     Serial.println("ntp.update(): true ");
   }else{
     Serial.println("ntp.update(): false ");
   }

   struct tm timeinfo;
   getLocalTime(&timeinfo);
   int _hour  = timeinfo.tm_hour;
   int _min   = timeinfo.tm_min;
   int _sec   = timeinfo.tm_sec;
   int _dia   = timeinfo.tm_mday;
   int _mes   = timeinfo.tm_mon+1;
   int _ano   = timeinfo.tm_year+1900;
   int _isdst = timeinfo.tm_isdst;
   Serial.println("H: "+String(_hour)+":"+String(_min)+":"+String(_sec));


  delay(1000);
}
