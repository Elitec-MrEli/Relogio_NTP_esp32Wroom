
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

  ntp.begin();

  //GMT em segundos
  // +1 = 3600
  // +8 = 28800
  // -1 = -3600
  // -3 = -10800 (BRASIL)
  ntp.setTimeOffset(-10800);
}

void loop() {
    Serial.print("ONa: ");
    Serial.println(millis());


    if ((millis() >= 30000)&&(millis() >= 32000)){ ntp.setPoolServerName("pool.ntp.org"); }
       

       
  if (ntp.forceUpdate()) {   

    Serial.print("HORARIO: ");
    Serial.print(ntp.getFormattedTime());

    Serial.print(" HORA: ");
    Serial.print(ntp.getHours());

    Serial.print(" MINUTOS: ");
    Serial.print(ntp.getMinutes());

    Serial.print(" SEGUNDOS: ");
    Serial.print(ntp.getSeconds());

    Serial.print(" DIA DA SEMANA (0=domingo): ");
    Serial.print(ntp.getDay());

    Serial.print(" EPOCH TIME (Segundos desde 01/01/1970): ");
    Serial.print(ntp.getEpochTime());

    Serial.println();

  } else {
    Serial.println("!Erro ao atualizar NTP!");
  }
  delay(1000);
}
