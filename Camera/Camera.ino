#include <Arduino.h>

#include <WiFi_connection.h>
#include <ov2640a.h>
#include <ESP_Mail_Client.h>
#include <esp_camera.h>

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT esp_mail_smtp_port_465

/* The log in credentials */
#define AUTHOR_EMAIL "nicoesp32@gmail.com"
#define AUTHOR_PASSWORD "skouwtzqjdlynihe"

#define WIFI_SSID "ETRR Free"
#define WIFI_PASSWORD ""
#define TIMEOUT 20

SMTPSession smtp;
void SendEmail(void);

void setup(){
  Serial.begin(115200);
  Serial.println("a");
  inic_conexion(WIFI_SSID, WIFI_PASSWORD, TIMEOUT);

  OV2640 info_camera = inic_camera();

  SendEmail();
}

void SendEmail(void){
    camera_fb_t * fb = NULL;
    /* Empleando la función esp_camera_fb_get obtengo un puntero al comienzo de la trama almacenada
        en el buffer de la cámara que contiene la imagen sacada en ese momento e información sobre ella. 
        Es un puntero del tipo camera_fb_t. emoroni */
    fb = esp_camera_fb_get(); 
    if(!fb) {
        Serial.println("Camera capture failed");
        delay(1000);
        ESP.restart();
    }
    
    /* Declare the session config data */
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = F("mydomain.net");

    /* Set the NTP config time */
    session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
    session.time.gmt_offset = 3;
    session.time.day_light_offset = 0;

    /* Declare the message class */
    SMTP_Message message;

    /* Enable the chunked data transfer with pipelining for large message if server supported */
    message.enable.chunking = true;

    /* Set the message headers */
    message.sender.name = F("ESP Mail");
    message.sender.email = AUTHOR_EMAIL;

    message.subject = F("Test sending camera image");
    message.addRecipient(F("nbassin@etrr.edu.ar"), F("nbassin@etrr.edu.ar"));

    message.html.content = F("<span style=\"color:#ff0000;\">The camera image.</span><br/><br/><img src=\"cid:image-001\" alt=\"esp32 cam image\"  width=\"800\" height=\"600\">");

    /** The content transfer encoding e.g.
     * enc_7bit or "7bit" (not encoded)
     * enc_qp or "quoted-printable" (encoded) <- not supported for message from blob and file
     * enc_base64 or "base64" (encoded)
     * enc_binary or "binary" (not encoded)
     * enc_8bit or "8bit" (not encoded)
     * The default value is "7bit"
     */
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /** The HTML text message character set e.g.
     * us-ascii
     * utf-8
     * utf-7
     * The default value is utf-8
     */
    message.html.charSet = F("utf-8");

    SMTP_Attachment att;

    /** Set the inline image info e.g.
     * file name, MIME type, file path, file storage type,
     * transfer encoding and content encoding
     */
    att.descr.filename = F("camera.jpg");
    att.descr.mime = F("image/jpg");

    att.blob.data = (fb->buf);
    att.blob.size = (fb->len);

    att.descr.content_id = F("image-001"); // The content id (cid) of camera.jpg image in the src tag

    /* Need to be base64 transfer encoding for inline image */
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    /* Add inline image to the message */
    message.addInlineImage(att);

    /* Connect to server with the session config */
    if (!smtp.connect(&session))
        return;

    /* Start sending the Email and close the session */
    if (!MailClient.sendMail(&smtp, &message, true)){
        Serial.print("Error sending Email, ");
        Serial.println(smtp.errorReason());
    }

    // to clear sending result log
    // smtp.sendingResult.clear();

    ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());

    // Elimino la imagen
    esp_camera_fb_return(fb);
}

void loop(){
    
}
