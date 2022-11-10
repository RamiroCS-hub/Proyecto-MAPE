/*
 * GET - https://www.php.net/manual/es/reserved.variables.get.php. emoroni.
 */

#include <ov2640a.h>

OV2640 inic_camera (void)
{
    camera_config_t config;
    OV2640 info_ov2640;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    info_ov2640.psramFound = psramFound();

    // Estoy trabajando en modos de sacar la foto en una calidad inferior a la máxima que podría
    // sacar con esta cámara. emoroni.
    // init with high specs to pre-allocate larger buffers
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA; //;FRAMESIZE_SVGA FRAMESIZE_UXGA
        config.jpeg_quality = 10;  //0-63 lower number means higher quality
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_CIF;
        config.jpeg_quality = 12;  //0-63 lower number means higher quality
        config.fb_count = 1;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        info_ov2640.error = "Camera init failed with error 0x" + err;
        ESP.restart();
    }

    return info_ov2640;
}