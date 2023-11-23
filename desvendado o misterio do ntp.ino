
#include <NTPClient.h> 
#include <WiFi.h>
#include "time.h"
#include <TimeLib.h>

const char ssid[] = "elitec_testes";
const char password[] = "12345678";

const char* ntpServer[] = {"pool.ntp.org_off", "200.20.186.76", "189.45.192.3"}; //200.20 é do ntp br//o 189.45 é da unifique
WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);
/*
int _hour=23;
int _min=15;
int _sec=0;
int _dia=18;
int _mes=11;
int _ano=2023;
*/
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando no WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConectado!\n");


  //boolean ajusta_hora_pelo_ntp = true;
  setTime(23, 15, 0, 18, 11, 2023); // hora, mim, seg, dia, mes, ano
  ntp.setTimeOffset(-10800);
  ntp.begin();  
  
}

void loop() {
    //Serial.print("ONa: ");
    //Serial.print(millis());
    //if ((millis() >= 30000)&&(millis() >= 31000)){ ntp.setPoolServerName("pool.ntp.org"); }
    if ((millis() >= 20000)&&(millis() <= 21000)){ ntp.setPoolServerName("200.20.186.76_off"); }  
    if ((millis() >= 90000)&&(millis() <= 91000)){ ntp.setPoolServerName("pool.ntp.org"); }   
    //if ((millis() >= 30000)&&(millis() >= 31000)){ setTime(23, 15, 0, 10, 11, 2023); }
    //if ((millis() >= 50000)&&(millis() >= 51000)){ setTime(01, 01, 59, 28, 02, 2019); }  
    if ((millis() >= 60000)&&(millis() <= 61000)){ setTime(01, 10, 20, 01, 02, 1980); }   
   
  if (ntp.forceUpdate()) { 
    delay(10);      
    Serial.println("NTP! OK");
  } else {
    delay(10);
    Serial.print("!Erro ao atualizar NTP!");
    Serial.println();
  }             
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
          Serial.print(" NOW: "+String(now()));
          Serial.println(" MILLIS: "+String(millis()/1000));   

                printf("NTP: %02d:%02d:%02d %02d/%02d/%4d\n", hour(ntp.getEpochTime()), minute(ntp.getEpochTime()), second(ntp.getEpochTime()), day(ntp.getEpochTime()), month(ntp.getEpochTime()), year(ntp.getEpochTime()));                

                verifica_sincronia_ESP_x_NTP( year(ntp.getEpochTime()), month(ntp.getEpochTime()), day(ntp.getEpochTime()), hour(ntp.getEpochTime()), minute(ntp.getEpochTime()), second(ntp.getEpochTime()));
  delay(1000);
}

void verifica_sincronia_ESP_x_NTP(int ano_, int mes_ , int dia_, int hour_, int min_, int sec_){
  //if (ajusta_hora_pelo_ntp == true){
   if (year()!=ano_){
     Serial.println("ano diferente, ajustando...");     
     setTime(hour_, min_, sec_, dia_, mes_, ano_); // hora, mim, seg, dia, mes, ano
     delay(10);
   }
   if (month()!=mes_){
     Serial.println("mes diferente, ajustando...");
     setTime(hour_, min_, sec_, dia_, mes_, ano_); // hora, mim, seg, dia, mes, ano
     delay(10);
   }
   if (day()!=dia_){
     Serial.println("dia diferente, ajustando...");
     setTime(hour_, min_, sec_, dia_, mes_, ano_); // hora, mim, seg, dia, mes, ano
     delay(10);
   }
   if (hour()!=hour_){
     Serial.println("hora diferente, ajustando...");
     setTime(hour_, min_, sec_, dia_, mes_, ano_); // hora, mim, seg, dia, mes, ano
     delay(10);
   }
   if (minute()!=min_){
     Serial.println("minuto diferente, ajustando...");
     setTime(hour_, min_, sec_, dia_, mes_, ano_); // hora, mim, seg, dia, mes, ano
     delay(10);
   }
  //}
}



//Serial.print(" HORARIO: ");
    //Serial.print(ntp.getFormattedTime());

    //Serial.print(" H: ");
    //Serial.print(ntp.getHours());

    //Serial.print(" M: ");
    //Serial.print(ntp.getMinutes());

    //Serial.print(" S: ");
    //Serial.print(ntp.getSeconds());

    //Serial.print(" D.SEM(0=domingo): ");
    //Serial.print(ntp.getDay());

    //Serial.print(" t.stmp(desde.01/01/1970): ");
    //Serial.print(ntp.getEpochTime());

//if (ntp.update()) {  
     //Serial.println("ntp.update(): true ");
   //}else{
     //Serial.println("ntp.update(): false ");
   //}
        /*
        struct tm timeinfo;
        getLocalTime(&timeinfo);
        int _hour  = timeinfo.tm_hour;
        int _min   = timeinfo.tm_min;
        int _sec   = timeinfo.tm_sec;
        int _dia   = timeinfo.tm_mday;
        int _mes   = timeinfo.tm_mon+1;
        int _ano   = timeinfo.tm_year+1900;
        int _isdst = timeinfo.tm_isdst;
        Serial.println(&timeinfo, "NTP: %H:%M:%S %d/%m/%Y ");  
        Serial.println("H: "+String(_hour)+":"+String(_min)+":"+String(_sec));
        
        time_t t = now(); // Store the current time in time variable t

        hour(t);          // Returns the hour for the given
        minute(t);        // Returns the minute for the given
        second(t);        // Returns the second for the given
        day(t);           // The day for the given time 
        weekday(t);       // Day of the week for the 
        month(t);         // The month for the given time t
        year(t);          // The year for the given time t
        */
