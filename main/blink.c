/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "ps4.h"

#include "nvs_flash.h"

static int battery = 0;
static int lx = 0, ly = 0, rx = 0, ry = 0;

void controller_event_cb( ps4_t ps4, ps4_event_t event )
{
    // Below has all accessible outputs from the controller
    if(ps4IsConnected()) {
      if ( ps4.button.up )
          printf("Up Button\n");
      if ( ps4.button.down )
          printf("Down Button\n");
      if ( ps4.button.left )
          printf("Left Button\n");
      if ( ps4.button.right )
          printf("Right Button\n");
        
      if ( ps4.button.upright )
          printf("Up Right\n");
      if ( ps4.button.upleft )
          printf("Up Left\n");
      if ( ps4.button.downleft )
          printf("Down Left\n");
      if ( ps4.button.downright )
          printf("Down Right\n");
        
      if ( ps4.button.triangle )
          printf("Triangle Button\n");
      if ( ps4.button.circle )
          printf("Circle Button\n");
      if ( ps4.button.cross )
          printf("Cross Button\n");
      if ( ps4.button.square )
          printf("Square Button\n");
        
      if ( ps4.button.l1 )
          printf("l1 Button\n");
      if ( ps4.button.r1 )
          printf("r1 Button\n");
        
      if ( ps4.button.l3 )
          printf("l3 Button\n");
      if ( ps4.button.r3 )
          printf("r3 Button\n");
        
      if ( ps4.button.share )
          printf("Share Button\n");
      if ( ps4.button.options )
          printf("Options Button\n");
        
      if ( ps4.button.ps )
          printf("PS Button\n");
      if ( ps4.button.touchpad )
          printf("Touch Pad Button\n");
        
      if ( ps4.button.l2 ) {
          printf("l2 button at %d\n", ps4.analog.button.l2);
      }
      if ( ps4.button.r2 ) {
          printf("r2 button at %d\n", ps4.analog.button.r2);
      }

      if ( event.analog_move.stick.lx ) {
        if(lx != ps4.analog.stick.lx) {
          printf("Left Stick x at %d\n", ps4.analog.stick.lx);
          lx = ps4.analog.stick.lx;
        }
      }
      if ( event.analog_move.stick.ly ) {
        if(ly != ps4.analog.stick.ly) {
          printf("Left Stick y at %d\n", ps4.analog.stick.ly);
          ly = ps4.analog.stick.ly;
        }
      }
      if ( event.analog_move.stick.rx ) {
        if(rx != ps4.analog.stick.rx) {
          printf("Right Stick x at %d\n", ps4.analog.stick.rx);
          rx = ps4.analog.stick.rx;
        }
      }
      if ( event.analog_move.stick.ry ) {
        if(ry != ps4.analog.stick.ry) {
          printf("Right Stick y at %d\n", ps4.analog.stick.ry);
          ry = ps4.analog.stick.ry;
        }
      }

      if(battery != ps4.status.battery) {
        if (ps4.status.charging)
          printf("The controller is charging\n");
        if (ps4.status.audio)
          printf("The controller has headphones attached\n");
        if (ps4.status.mic)
          printf("The controller has a mic attached\n");

        printf("Battey = %d\n", ps4.status.battery);

        battery = ps4.status.battery;
      }
    }
}

void Main_Task(void *pvParams)
{
	uint8_t new_mac[8] = {0x48,0x2C,0xA0,0x2D,0xBF,0xB3};
	ps4SetBluetoothMacAddress(new_mac);

	ps4SetEventCallback(controller_event_cb);
	ps4Init();
	while (!ps4IsConnected()){
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	printf("\r\nps4 Joystick connected !!!\r\n");
	while(1) {
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}


/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    xTaskCreate(&Main_Task, "Main_Task", 8192, NULL, 1, NULL);

    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
	//printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
	//printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
