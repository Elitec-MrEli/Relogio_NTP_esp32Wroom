
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

    Serial.print("t.stmp(desde.01/01/1970): ");
    Serial.print(ntp.getEpochTime());

    Serial.println();

  } else {
    Serial.print(" !Erro ao atualizar NTP!");
  }

   if (ntp.update()) {  
     Serial.println(" update true ");
   }else{
     Serial.println(" update false ");
   }

  delay(1000);
}
