#include <WiFi.h>
#include "time.h"
#include <TimeLib.h>

const char* ssid       = "S96Pro";
const char* password   = "12345678";

int _hour=23;
int _min=15;
int _sec=0;
int _dia=18;
int _mes=11;
int _ano=2023;
int _isdst;

boolean ajusta_hora_pelo_ntp = true;

const char* ntpServer[] = {"pool.ntp.org", "200.20.186.76", "189.45.192.3"}; //200.20 é do ntp br//o 189.45 é da unifique
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;


void setup(){
  Serial.begin(9600);
  delay(100);

  setTime(23, 15, 0, 18, 11, 2023); // hora, mim, seg, dia, mes, ano

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer[0], ntpServer[1], ntpServer[2]); //TENTA COM AS 3 ALTERNATIVAS DE SERVER
  //printLocalTime();

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(1000);
  printLocalTime();

          Serial.print("ESP: ");
          if (hour()<10){Serial.print("0");}
          Serial.print(hour());
          Serial.print(":");
          if (minute()<10){Serial.print("0");}
          Serial.print(minute());
          Serial.print(":");
          if (second()<10){Serial.print("0");}
          Serial.print(second());
          Serial.print(" ");
          Serial.print(day());
          Serial.print("/");
          Serial.print(month());
          Serial.print("/");
          Serial.print(year()); 
          Serial.println();
          
          //Serial.println("h_ "+String(_hour)+":"+String(_min)+":"+String(_sec)+" a_ "+String(_ano)+" "+String(_mes)+" "+String(_dia)+" isdst: "+String(_isdst));
          
          verifica_sincronia_ESP_x_NTP(_ano,_mes,_dia,_hour,_min);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha de obter a hora dos server: "+String(ntpServer[0])+", "+String(ntpServer[1])+", "+String(ntpServer[2]));
    ajusta_hora_pelo_ntp = false;
    return;
  }
  boolean ajusta_hora_pelo_ntp = true;
  Serial.println(&timeinfo, "NTP: %H:%M:%S %d/%m/%Y ");  
   _hour = timeinfo.tm_hour;
   _min  = timeinfo.tm_min;
   _sec  = timeinfo.tm_sec;
   _dia  = timeinfo.tm_mday;
   _mes  = timeinfo.tm_mon+1;
   _ano  = timeinfo.tm_year+1900;
  _isdst = timeinfo.tm_isdst;

}

void verifica_sincronia_ESP_x_NTP(int ano_, int mes_ , int dia_, int hour_, int min_ ){
  if (ajusta_hora_pelo_ntp == true){
   if (year()!=ano_){
     Serial.println("ano diferente, ajustando...");
     setTime(_hour, _min, _sec, _dia, _mes, _ano); // hora, mim, seg, dia, mes, ano
     delay(1000);
   }
   if (month()!=mes_){
     Serial.println("mes diferente, ajustando...");
     setTime(_hour, _min, _sec, _dia, _mes, _ano); // hora, mim, seg, dia, mes, ano
     delay(1000);
   }
   if (day()!=dia_){
     Serial.println("dia diferente, ajustando...");
     setTime(_hour, _min, _sec, _dia, _mes, _ano); // hora, mim, seg, dia, mes, ano
     delay(1000);
   }
   if (hour()!=hour_){
     Serial.println("hora diferente, ajustando...");
     setTime(_hour, _min, _sec, _dia, _mes, _ano); // hora, mim, seg, dia, mes, ano
     delay(1000);
   }
   if (minute()!=min_){
     Serial.println("minuto diferente, ajustando...");
     setTime(_hour, _min, _sec, _dia, _mes, _ano); // hora, mim, seg, dia, mes, ano
     delay(1000);
   }
  }
}
