#include <WiFi.h>
#include "time.h"
#include <TimeLib.h>
#include <NTPClient.h>

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

/* -------- Configurações de relógio on-line----------- */
WiFiUDP udp;
const char* _serverP = "pool.ntp.org";      //NTP GLOBAL      //SERVIDOR NTP PRINCIPAL
const char* _serverB = "a.st1.ntp.br";     //NTP BR          //SERVIDOR NTP SECUNDARIO
const char* _serverP2 = "189.45.192.3";     //PARA TESTES DE DECINCRONISMO //ACABOU FICANADO NO CODIGO! //SERVER PROVEDOR UNIFIQUE

/*OUTROS SERVIDORES*/
//const char* _serverP = "a.st1.ntp.br";                        //NTP BR
//const char* _serverB = "0.br.pool.ntp.org";                   //NTP GLOBAL

String _server_ativo = "erro Desconhecido nos Servidores NTP!";  //ARMAZENA O ESTADO DO SERVIDOR PARA FUTURAS BUSCAS DE HORARIO ATUALIZADO E ATUALIZAÇOES
String hora = "Ainda Desconhecida";                              //ARMAZENA A HORA CERTA ATUALIZADA OU DO SISTEMA PARA FINS DE IMPRESSAO E CONTROLE
String _timestamp_S_NTP = "Ainda desconhecido";                  //ARMAZENA O TIMESTAMP CERTO ATUALIZADO OU DO NTP PARA FINS DE IMPRESSAO E CONTROLE
long _time_stamp_CTR = 1;
long _time_stamp_60s = -1;
char hora_formatada[64];           //PARA MANIPUNAR A HORA TIMESTAMP INTERNA DO MICROCONTROLADOR
char data_formatada[64];           //PARA MANIPUNAR A DATA TIMESTAMP INTERNA DO MICROCONTROLADOR
boolean conexao_web_ntp = false;   //SETA ESTADO DA CONEXAO COM O SERVER NTP O QUE VALIDA TAMBEM A CONEXAO COM A INTERNET
boolean _assincronia_NTP = false;  //SETA ESTADO DO SINCRONISMO PARA QUE FAÇA TENTATIVA DE SINCRONIA A CADA MINUTO

time_t _time_stamp;

NTPClient ntp_P(udp, _serverP, -3 * 3600, 60000);    //SETA UM OBJETO COM AS CONFIGURAÇÕES DO SERVER PRINCIPAL
NTPClient ntp_B(udp, _serverB, -3 * 3600, 60000);    //SETA O SEGUNDO OBJETO COM AS CONF DO SERVER DE BACKUP
NTPClient ntp_P2(udp, _serverP2, -3 * 3600, 60000);  //SETA UM TERCEIRO OBJETO COM AS CONFIGURAÇÕES DO SERVER DE BACKUP 2

struct tm data;  //CRIA UMA INFRAESTRUTURA QUE CONTEM AS INFORMAÇOES DA DATA.
                 /*
           struct tm {
             int tm_sec;         // segundos,  faixa de 0 até 59        
             int tm_min;         // minutos, faixa de 0 até 59           
             int tm_hour;        // hora, faixa de 0 até 23             
             int tm_mday;        // dia do mês, faixa de 1 até 31  
             int tm_mon;         // mês, faixa de 0 até 11             
             int tm_year;        // O número para o ano depois de 1900   
             int tm_wday;        // dia da semana, faixa de 0 até 6    
             int tm_yday;        // dia no ano, faixa de 0 até 365  
             int tm_isdst;       // horário de verão             
          };
          */



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
  temos_conexao("ntp_P");
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


void temos_conexao(char* _ntp_) {
  if (_ntp_ == "ntp_P") {
    ntp_P.begin();
    if (ntp_P.forceUpdate()) {
      Serial.print("|NTP ok| ");  //Serial.print("|NTP offline: "+String(_serverP)+"|");
      if (conexao_web_ntp == false) {
        _assincronia_NTP = true;
        //Tempos_de_sincronismo_NTP();
        _server_ativo = _serverP;
      }
      hora = ntp_P.getFormattedTime();         // PEGA A HORA DO SERVER NTP 
      _timestamp_S_NTP = ntp_P.getEpochTime(); // PEGA A TIMESTAMP DO SERVER NTP 
    } else {
      //ATRIBUIR ERRO DE WEB AQUI
      Serial.print("|NTP OFFLINE| ");  //Serial.print("|NTP offline: "+String(_serverP)+"|");
      conexao_web_ntp = false;
    }
  } else {
    if (_ntp_ == "ntp_B") {
      ntp_B.begin();
      if (ntp_B.forceUpdate()) {
        Serial.print("|NTP ok| ");
        if (conexao_web_ntp == false) {
          _assincronia_NTP = true;
          //Tempos_de_sincronismo_NTP();
          _server_ativo = _serverB;
        }
        hora = ntp_B.getFormattedTime();        ///* PEGA A HORA DO SERVER NTP 
        _timestamp_S_NTP = ntp_B.getEpochTime(); //PEGA A TIMESTAMP DO SERVER NTP 
      } else {
        //ATRIBUIR ERRO DE WEB AQUI
        Serial.print("|NTP OFFLINE| ");  //Serial.print("|NTP offline: "+String(_serverB)+"|");
        conexao_web_ntp = false;
      }
    } else {
      if (_ntp_ == "ntp_P2") {
        ntp_P2.begin();
        if (ntp_P2.forceUpdate()) {
          Serial.print("|NTP ok| ");
          if (conexao_web_ntp == false) {
            _assincronia_NTP = true;
           //Tempos_de_sincronismo_NTP();
            _server_ativo = _serverP2;
          }
          hora = ntp_P2.getFormattedTime();         // PEGA A HORA DO SERVER NTP 
          _timestamp_S_NTP = ntp_P2.getEpochTime(); // PEGA A TIMESTAMP DO SERVER NTP 
        } else {
          //ATRIBUIR ERRO DE WEB AQUI
          Serial.print("|NTP OFFLINE| ");  //Serial.print("|NTP offline: "+String(_serverP2)+"|");
          conexao_web_ntp = false;
        }
      }
    }
  }
}
