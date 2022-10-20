#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h> //Documentación: https://github.com/mobizt/ESP-Mail-Client/blob/master/README.md
#include <FirebaseESP32.h>
#include <ESP32Time.h>

// #include <iostream>
//Configuracion del servidor NTP
const char* ntpServer = "pool.ntp.org"; //Servidor NTP
const long gmtOffset_sec = (-4*3600) + 3600;     //Desplazamiento GMT
const int daylightOffset_sec = 0;       //Compensa

using namespace std;

// WiFi
#define SSID "ETRR Free"
#define PASSWORD ""

// Nombre servidor smtp
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT esp_mail_smtp_port_465

// Credenciales de la cuenta remitente
#define AUTHOR_EMAIL "pythonnexample@gmail.com"
#define AUTHOR_PASSWORD "rjvjltqrzdatisvu"

ESP32Time rtc;
//Defino Firebase objects
#define API_KEY "onRVw4e1U3GPcL9QOLwoLWteRvEk7945GKPgILH3"
#define DATABASE_URL "https://fir-test-50790-default-rtdb.firebaseio.com/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseConfig config;
SMTPSession smtp;

bool flag_conectado = false;
int contador_segundos;

bool valor = true;
void sendEmail (String);
void alarma_activada(void);
void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Bienvenido a la consola de pruebas...");

  WiFi.begin(SSID, PASSWORD);
  
  while((WiFi.status() != WL_CONNECTED) && (!flag_conectado)){
    delay(1000);
    Serial.println(contador_segundos);
    contador_segundos ++;
    if (contador_segundos == 60) flag_conectado = true; // Coloco esta condición como timeout = 2s, para que en caso de que no se pueda conectar pueda salir del lazo del while.
  }
  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  Firebase.begin(DATABASE_URL, API_KEY);
  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
    
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

bool flag = false;
//amplitud de la señal
float sinVal;
//frecuencia del tono
int toneVal;
//estado de la alarma
boolean stateAlarm;

void loop(){
    Firebase.getInt(fbdo,"User/alarma");
    Serial.println(fbdo.intData());
    if(fbdo.intData() == 1 && flag == false){
        Firebase.getString(fbdo,"User/email");
        String email = fbdo.stringData();
        Serial.print(email);
        sendEmail(email);
        flag = true;
    }else{
      Serial.println("oo");
    }
}

void sendEmail (String email){
    
    String textMsg = rtc.getTime("%A, %B %d %Y %H:%M:%S");
    // String textMsg = "asdasd";
    /* Declaramos una Sesion para poder configurar*/
    ESP_Mail_Session session;

    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;

    /* Declaramos una variable para el mensaje */
    SMTP_Message message;

    /* Especificar los encabezados*/
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ALARMA ACTIVADA";
    message.addRecipient(email,email);
    
    message.text.content = textMsg.c_str();

    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit; // sin codificación

    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

    /* Conectar al servidor con la configuracios de la sesión*/
    if (!smtp.connect(&session)){
      Serial.println("No se pudo conectar");
    }
    
    /* Envía el correo y cierra la sesión */
    if (!MailClient.sendMail(&smtp, &message)){
        Serial.println("Error sending Email, " + smtp.errorReason());
    }
    ESP_MAIL_PRINTF("Liberar memoria: %d\n", MailClient.getFreeHeap());
}

void alarma_activada(){
  Firebase.getInt(fbdo,"User\alarma");
  int alarma = firebaseData.intData() 
  if(alarma == 1){
    //se activa la alarma
    stateAlarm=!stateAlarm;
    delay(300);
  }

  if(stateAlarm==1){
    for(int x=0;x<180;x++){
      //convertimos los grados de 0 a 180 a radienas 
      sinVal = (sin(x*(3.1412/180)));
      //calculamos el valor de la frecuencioa
      toneVal = 2000+(int(sinVal*1000));
      tone(7, toneVal);
      delay(2);
      if(alarma == 1){
        stateAlarm=!stateAlarm;
        delay(300);
      }
    }
  }
  else{
    noTone(7);
  }
}
