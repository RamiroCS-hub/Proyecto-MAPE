#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h> //Documentación: https://github.com/mobizt/ESP-Mail-Client/blob/master/README.md

//Configuracion del servidor NTP
const char* ntpServer = "pool.ntp.org"; //Servidor NTP
const long gmtOffset_sec = -4*3600;     //Desplazamiento GMT
const int daylightOffset_sec = 0;       //Compensa

// WiFi
#define WIFI_SSID "Etrr Free"
#define WIFI_PASSWORD ""

// Nombre servidor smtp
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT esp_mail_smtp_port_465

// Credenciales de la cuenta remitente
#define AUTHOR_EMAIL "pythonnexample@gmail.com"
#define AUTHOR_PASSWORD "rjvjltqrzdatisvu"

// Declaramos una variable para referenciar la sesión SMTP 
SMTPSession smtp;
ESP32Time rtc;

void setup(){

    Serial.begin(9600);
    Serial.println();
    Serial.print("Conectando al Acess point...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}
void sendEmail (string);
void loop(){
    string email;
    Firebase.getInt(fbdo,"alarma");
    if(firebaseData.intData() == 1){
        Firebase.getString(fbdo,"email")
        email = firebaseData.stringData()
        sendEmail(email);
    }
}

void sendEmail (email){
    
    strin textMsg = rtc.getTime("%A, %B %d %Y %H:%M:%S");

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
    message.addRecipient(email,email)
    
    message.text.content = textMsg.c_str();

    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit; // sin codificación

    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

    /* Conectar al servidor con la configuracios de la sesión*/
    if (!smtp.connect(&session)) Serial.pintln("No se pudo conectar")
    
    /* Envía el correo y cierra la sesión */
    if (!MailClient.sendMail(&smtp, &message)){
        Serial.println("Error sending Email, " + smtp.errorReason());
    }
    ESP_MAIL_PRINTF("Liberar memoria: %d\n", MailClient.getFreeHeap());
}