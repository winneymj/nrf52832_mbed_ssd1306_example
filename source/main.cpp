/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <events/mbed_events.h>
#include <mbed.h>
#include <PinNames.h>
#include <lvgl/lvgl.h>
#include <lv_drivers/display/ST7789.h>

#define LVGL_TICK 5
#define TICKER_TIME 0.001 * LVGL_TICK

Ticker ticker;
DigitalOut _led1(LED1);

#define SSD1306_SWITCHCAPVCC true

/** This example demonstrates all the basic setup required
 *  for pairing and setting up link security both as a central and peripheral
 *
 *  The example is implemented as two classes, one for the peripheral and one
 *  for central inheriting from a common base. They are run in sequence and
 *  require a peer device to connect to.
 *
 *  During the test output is written on the serial connection to monitor its
 *  progress.
 */

// static const uint8_t DEVICE_NAME[] = "SM_device";
// static const uint16_t uuid16_list[] = {LEDService::LED_SERVICE_UUID};

// static int colour = BLACK;

// define the Serial object
// Serial pc(USBTX, USBRX);
// LEDService *ledServicePtr;
// SPI mySPI(SPI_PSELMOSI0, NC, SPI_PSELSCK0, NC);
// Adafruit_SSD1306_Spi *display;
// Adafruit_SSD1306_Spi display(mySPI, P0_16, P0_17, P0_14 , 128, 64);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char logo_bmp[] =
{ 0B00000000, 0B11000000,
  0B00000001, 0B11000000,
  0B00000001, 0B11000000,
  0B00000011, 0B11100000,
  0B11110011, 0B11100000,
  0B11111110, 0B11111000,
  0B01111110, 0B11111111,
  0B00110011, 0B10011111,
  0B00011111, 0B11111100,
  0B00001101, 0B01110000,
  0B00011011, 0B10100000,
  0B00111111, 0B11100000,
  0B00111111, 0B11110000,
  0B01111100, 0B11110000,
  0B01110000, 0B01110000,
  0B00000000, 0B00110000 };

/** Base class for both peripheral and central. The same class that provides
 *  the logic for the application also implements the SecurityManagerEventHandler
 *  which is the interface used by the Security Manager to communicate events
 *  back to the applications. You can provide overrides for a selection of events
 *  your application is interested in.
 */
// class SMDevice : private mbed::NonCopyable<SMDevice>,
//                  public SecurityManager::EventHandler
// {

// public:
//     SMDevice(BLE &ble, events::EventQueue &event_queue) :
//         _led1(LED1, 0),
//         _ble(ble),
//         _event_queue(event_queue),
//         _handle(0),
//         _is_connecting(false) { };

//     virtual ~SMDevice()
//     {
//         if (_ble.hasInitialized()) {
//             _ble.shutdown();
//         }
//     };

//     /** Start BLE interface initialisation */
//     void run()
//     {
//         printf("SMDevice:run: ENTER\r\n");

//         ble_error_t error;

//         /* to show we're running we'll blink every 500ms */
//         _event_queue.call_every(1000, this, &SMDevice::blink);

//         if (_ble.hasInitialized()) {
//             printf("Ble instance already initialised.\r\n");
//             return;
//         }

//         /* this will inform us off all events so we can schedule their handling
//          * using our event queue */
//         _ble.onEventsToProcess(
//             makeFunctionPointer(this, &SMDevice::schedule_ble_events)
//         );

//         error = _ble.init(this, &SMDevice::on_init_complete);

//         if (error) {
//             printf("Error returned by BLE::init.\r\n");
//             return;
//         }

//         /* this will not return until shutdown */
//         _event_queue.dispatch_forever();

//         printf("SMDevice:run: EXIT\r\n");
//     };

//     /* event handler functions */

//     /** Respond to a pairing request. This will be called by the stack
//      * when a pairing request arrives and expects the application to
//      * call acceptPairingRequest or cancelPairingRequest */
//     virtual void pairingRequest(
//         ble::connection_handle_t connectionHandle
//     ) {
//         printf("Pairing requested. Authorising.\r\n");
//         _ble.securityManager().acceptPairingRequest(connectionHandle);
//     }

//     /** Inform the application of a successful pairing. Terminate the demonstration. */
//     virtual void pairingResult(
//         ble::connection_handle_t connectionHandle,
//         SecurityManager::SecurityCompletionStatus_t result
//     ) {
//         printf("SMDevice:pairingResult: ENTER\r\n");
//         if (result == SecurityManager::SEC_STATUS_SUCCESS) {
//             printf("Pairing successful\r\n");
//         } else {
//             printf("Pairing failed\r\n");
//         }

//         // /* disconnect in 500 ms */
//         // _event_queue.call_in(
//         //     500, &_ble.gap(),
//         //     &Gap::disconnect, _handle, Gap::REMOTE_USER_TERMINATED_CONNECTION
//         // );
//         printf("SMDevice:pairingResult: EXIT\r\n");
//     }

//     /** Inform the application of change in encryption status. This will be
//      * communicated through the serial port */
//     virtual void linkEncryptionResult(
//         ble::connection_handle_t connectionHandle,
//         ble::link_encryption_t result
//     ) {
//         printf("SMDevice:linkEncryptionResult: ENTER\r\n");
//         if (result == ble::link_encryption_t::ENCRYPTED) {
//             printf("Link ENCRYPTED\r\n");
//         } else if (result == ble::link_encryption_t::ENCRYPTED_WITH_MITM) {
//             printf("Link ENCRYPTED_WITH_MITM\r\n");
//         } else if (result == ble::link_encryption_t::NOT_ENCRYPTED) {
//             printf("Link NOT_ENCRYPTED\r\n");
//         }
//         printf("SMDevice:linkEncryptionResult: EXIT\r\n");
//     }

// private:
//     /** Override to start chosen activity when initialisation completes */
//     virtual void start() = 0;

//     /**
//      * This callback allows the LEDService to receive updates to the ledState Characteristic.
//      *
//      * @param[in] params
//      *     Information about the characterisitc being updated.
//      */
//     void onDataWrittenCallback(const GattWriteCallbackParams *params) {
//         printf("SMDevice:onDataWrittenCallback: ENTER\r\n");
//         if ((params->handle == ledServicePtr->getValueHandle()) && (params->len == 1)) {
//             printf("onDataWrittenCallback:" + *(params->data));
//             printf("\r\n");
//             // actuatedLED = *(params->data);
//         }
//         printf("SMDevice:onDataWrittenCallback: EXIT\r\n");
//     }

//     /** This is called when BLE interface is initialised and starts the demonstration */
//     void on_init_complete(BLE::InitializationCompleteCallbackContext *event)
//     {
//         printf("SMDevice:on_init_complete: ENTER\r\n");
//         ble_error_t error;

//         if (event->error) {
//             printf("Error during the initialisation\r\n");
//             return;
//         }

//         /* If the security manager is required this needs to be called before any
//          * calls to the Security manager happen. */
//         error = _ble.securityManager().init();

//         if (error) {
//             printf("Error during init %d\r\n", error);
//             return;
//         }

//         /* Tell the security manager to use methods in this class to inform us
//          * of any events. Class needs to implement SecurityManagerEventHandler. */
//         _ble.securityManager().setSecurityManagerEventHandler(this);

//         /* print device address */
//         Gap::AddressType_t addr_type;
//         Gap::Address_t addr;
//         _ble.gap().getAddress(&addr_type, addr);
//         printf("Device address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
//                addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);

//         /* when scanning we want to connect to a peer device so we need to
//          * attach callbacks that are used by Gap to notify us of events */
//         _ble.gap().onConnection(this, &SMDevice::on_connect);
//         _ble.gap().onDisconnection(this, &SMDevice::on_disconnect);

//         /* handle timeouts, for example when connection attempts fail */
//         _ble.gap().onTimeout().add(this, &SMDevice::on_timeout);

//         _ble.gattServer().onDataWritten(this, &SMDevice::onDataWrittenCallback);
        
//         bool initialValueForLEDCharacteristic = false;
//         ledServicePtr = new LEDService(_ble, initialValueForLEDCharacteristic);

//         /* start test in 500 ms */
//         _event_queue.call_in(500, this, &SMDevice::start);

//         printf("SMDevice:on_init_complete: EXIT\r\n");
//     };

//     /** This is called by Gap to notify the application we connected */
//     virtual void on_connect(const Gap::ConnectionCallbackParams_t *connection_event) = 0;

//     /** This is called by Gap to notify the application we disconnected,
//      *  in our case it ends the demonstration. */
//     void on_disconnect(const Gap::DisconnectionCallbackParams_t *event)
//     {
//         printf("SMDevice:on_disconnect: ENTER\r\n");
//         printf("SMDevice:on_disconnect: Reason=0x%X\r\n", event->reason);
//         // printf("Disconnected - demonstration ended \r\n");
//         // _event_queue.break_dispatch();
//         BLE::Instance().gap().startAdvertising();
//         printf("SMDevice:on_disconnect: EXIT\r\n");
//     };

//     /** End demonstration unexpectedly. Called if timeout is reached during advertising,
//      * scanning or connection initiation */
//     void on_timeout(const Gap::TimeoutSource_t source)
//     {
//         printf("SMDevice:on_timeout: ENTER\r\n");
//         printf("Unexpected timeout - aborting \r\n");
//         // _event_queue.break_dispatch();
//         printf("SMDevice:on_timeout: EXIT\r\n");
//     };

//     /** Schedule processing of events from the BLE in the event queue. */
//     void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
//     {
//         // printf("SMDevice:schedule_ble_events: ENTER\r\n");
//         _event_queue.call(mbed::callback(&context->ble, &BLE::processEvents));
//         // printf("SMDevice:schedule_ble_events: EXIT\r\n");
//     };

//     /** Blink LED to show we're running */
//     void blink(void)
//     {
//         Gap::GapState_t gapState = _ble.gap().getState();
//         printf("Gap State:Advertising=%s, connected=%s\r\n", gapState.advertising ? "On" : "Off", gapState.connected ? "Yes" : "No");

//         // Solid led if advertising
//         if (gapState.advertising) {
//             _led1.write(0);
//         }
//         else if (gapState.connected) {
//             _led1 = !_led1;
//         }
//         colour = (colour == BLACK) ? WHITE : BLACK;
//         // display.fillRect(0, 0, 127, 63, colour);
//         // display.drawLine(0, 0, 127, 63, WHITE);
//         // display.display();
//     };

// private:
//     DigitalOut _led1;

// protected:
//     BLE &_ble;
//     events::EventQueue &_event_queue;
//     ble::connection_handle_t _handle;
//     bool _is_connecting;
// };

// /** A peripheral device will advertise, accept the connection and request
//  * a change in link security. */
// class SMDevicePeripheral : public SMDevice {
// public:
//     SMDevicePeripheral(BLE &ble, events::EventQueue &event_queue)
//         : SMDevice(ble, event_queue) { }

//     virtual void start()
//     {
//         printf("SMDevicePeripheral:start: ENTER\r\n");
//         /* Set up and start advertising */

//         ble_error_t error;
//         GapAdvertisingData advertising_data;

//         /* add advertising flags */
//         advertising_data.addFlags(GapAdvertisingData::LE_GENERAL_DISCOVERABLE | GapAdvertisingData::BREDR_NOT_SUPPORTED);

//         /* add device name */
//         advertising_data.addData(GapAdvertisingData::COMPLETE_LOCAL_NAME, DEVICE_NAME, sizeof(DEVICE_NAME));

//         advertising_data.addData(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));

//         error = _ble.gap().setAdvertisingPayload(advertising_data);

//         if (error) {
//             printf("Error during Gap::setAdvertisingPayload\r\n");
//             return;
//         }

//         /* advertise to everyone */
//         _ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
//         /* how many milliseconds between advertisements, lower interval
//          * increases the chances of being seen at the cost of more power */
//         _ble.gap().setAdvertisingInterval(5000);
//         _ble.gap().setAdvertisingTimeout(0);

//         error = _ble.gap().startAdvertising();

//         if (error) {
//             printf("Error during Gap::startAdvertising.\r\n");
//             return;
//         }

//         /** This tells the stack to generate a pairingRequest event
//          * which will require this application to respond before pairing
//          * can proceed. Setting it to false will automatically accept
//          * pairing. */
//         _ble.securityManager().setPairingRequestAuthorisation(true);

//         printf("SMDevicePeripheral:start: EXIT\r\n");
//     };

//     /** This is called by Gap to notify the application we connected,
//      *  in our case it immediately requests a change in link security */
//     virtual void on_connect(const Gap::ConnectionCallbackParams_t *connection_event)
//     {
//         printf("SMDevicePeripheral:on_connect: ENTER\r\n");
//         ble_error_t error;

//         /* store the handle for future Security Manager requests */
//         _handle = connection_event->handle;

//         /* Request a change in link security. This will be done
//         * indirectly by asking the master of the connection to
//         * change it. Depending on circumstances different actions
//         * may be taken by the master which will trigger events
//         * which the applications should deal with. */
//         error = _ble.securityManager().setLinkSecurity(
//             _handle,
//             SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM
//         );

//         if (error) {
//             printf("Error during SM::setLinkSecurity %d\r\n", error);
//             return;
//         }
//         printf("SMDevicePeripheral:on_connect: EXIT\r\n");
//     };
// };

// void testdrawline() {
//   int16_t i;

//   display.clearDisplay(); // Clear display buffer

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, 0, i, display.height()-1, WHITE);
//     display.display(); // Update screen with each newly-drawn line
//     wait_ms(1);
//   }
//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(0, 0, display.width()-1, i, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   wait_ms(250);

//   display.clearDisplay();

//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(0, display.height()-1, i, 0, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   wait_ms(250);

//   display.clearDisplay();

//   for(i=display.width()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   for(i=display.height()-1; i>=0; i-=4) {
//     display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   wait_ms(250);

//   display.clearDisplay();

//   for(i=0; i<display.height(); i+=4) {
//     display.drawLine(display.width()-1, 0, 0, i, WHITE);
//     display.display();
//     wait_ms(1);
//   }
//   for(i=0; i<display.width(); i+=4) {
//     display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000); // Pause for 2 seconds
// }

// void testdrawrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=2) {
//     display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testfillrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2; i+=3) {
//     // The INVERSE color is used so rectangles alternate white/black
//     display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
//     display.display(); // Update screen with each newly-drawn rectangle
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testdrawcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
//     display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testfillcircle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
//     // The INVERSE color is used so circles alternate white/black
//     display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
//     display.display(); // Update screen with each newly-drawn circle
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testdrawroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, WHITE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testfillroundrect(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<display.height()/2-2; i+=2) {
//     // The INVERSE color is used so round-rects alternate white/black
//     display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
//       display.height()/4, INVERSE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testdrawtriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
//     display.drawTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, WHITE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testfilltriangle(void) {
//   display.clearDisplay();

//   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
//     // The INVERSE color is used so triangles alternate white/black
//     display.fillTriangle(
//       display.width()/2  , display.height()/2-i,
//       display.width()/2-i, display.height()/2+i,
//       display.width()/2+i, display.height()/2+i, INVERSE);
//     display.display();
//     wait_ms(1);
//   }

//   wait_ms(2000);
// }

// void testdrawchar(void) {
//   display.clearDisplay();

//   display.setTextSize(1);      // Normal 1:1 pixel scale
//   display.setTextColour(WHITE); // Draw white text
//   display.setCursor(0, 0);     // Start at top-left corner
//   display.cp437(true);         // Use full 256 char 'Code Page 437' font

//   // Not all the characters will fit on the display. This is normal.
//   // Library will draw what it can and the rest will be clipped.
//   for(int16_t i=0; i<256; i++) {
//     if(i == '\n') display.writeChar(' ');
//     else          display.writeChar(i);
//   }

//   display.display();
//   wait_ms(2000);
// }

// void testdrawstyles(void) {
//   display.clearDisplay();

//   display.setTextSize(1);             // Normal 1:1 pixel scale
//   display.setTextColour(WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.printf("Hello, world!");

//   display.setTextColour(BLACK, WHITE); // Draw 'inverse' text
//   display.printf("%f", 3.141592);

//   display.setTextSize(2);             // Draw 2X-scale text
//   display.setTextColour(WHITE);
//   display.printf("0x%X\n", 0xDEADBEEF);

//   display.display();
//   wait_ms(2000);
// }

// void testscrolltext(void) {
//   display.clearDisplay();

//   display.setTextSize(2); // Draw 2X-scale text
//   display.setTextColour(WHITE);
//   display.setCursor(10, 0);
//   display.printf("scroll\n");
//   display.display();      // Show initial text
//   wait_ms(100);

//   // Scroll in various directions, pausing in-between:
//   display.startscrollright(0x00, 0x0F);
//   wait_ms(2000);
//   display.stopscroll();
//   wait_ms(1000);
//   display.startscrollleft(0x00, 0x0F);
//   wait_ms(2000);
//   display.stopscroll();
//   wait_ms(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   wait_ms(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   wait_ms(2000);
//   display.stopscroll();
//   wait_ms(1000);
// }

// void testdrawbitmap(void) {
//   display.clearDisplay();

//   display.drawBitmap(
//     (display.width()  - LOGO_WIDTH ) / 2,
//     (display.height() - LOGO_HEIGHT) / 2,
//     logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
//   display.display();
//   wait_ms(1000);
// }

// #define XPOS   0 // Indexes into the 'icons' array in function below
// #define YPOS   1
// #define DELTAY 2

// void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
//   int8_t f, icons[NUMFLAKES][3];

//   // Initialize 'snowflake' positions
//   for(f=0; f< NUMFLAKES; f++) {
//     icons[f][XPOS]   = rand() % display.width();
//     icons[f][YPOS]   = -LOGO_HEIGHT;
//     icons[f][DELTAY] = (rand() % 6) + 1;
//   }

//   for(;;) { // Loop forever...
//     display.clearDisplay(); // Clear the display buffer

//     // Draw each snowflake:
//     for(f=0; f< NUMFLAKES; f++) {
//       display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
//     }

//     display.display(); // Show the display buffer on the screen
//     wait_ms(100);        // Pause for 1/10 second

//     // Then update coordinates of each flake...
//     for(f=0; f< NUMFLAKES; f++) {
//       icons[f][YPOS] += icons[f][DELTAY];
//       // If snowflake is off the bottom of the screen...
//       if (icons[f][YPOS] >= display.height()) {
//         // Reinitialize to a random position, just off the top
//         icons[f][XPOS]   = rand() % display.width();
//         icons[f][YPOS]   = -LOGO_HEIGHT;
//         icons[f][DELTAY] = (rand() % 6) + 1;
//       }
//     }
//   }
// }

void lvl_ticker_func()
{
  //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
  //It is required for the internal timing of LittlevGL.
  lv_tick_inc(LVGL_TICK);

  //Call lv_task_handler() periodically every few milliseconds. 
  //It will redraw the screen if required, handle input devices etc.  
  lv_task_handler();
}

int main()
{
  printf("\r\n main: ENTER \r\n\r\n");

  // Initalize the display driver st7789
  // st7789_init();
  // display.init();

  printf("main: st7789_init() done\r\n");

  lv_init();
  printf("main: lv_init() done\r\n");
  static lv_disp_buf_t disp_buf;
  static lv_color_t buf[LV_HOR_RES_MAX * 6];
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 6);
  printf("main: lv_disp_buf_init() done\r\n");

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  // disp_drv.flush_cb = st7789_flush;
  // disp_drv.buffer = &disp_buf;
  // lv_disp_drv_register(&disp_drv);

  ticker.attach(callback(&lvl_ticker_func), TICKER_TIME);

  events::EventQueue queue;

  st7789_init();
  st7789_init();

#define WHITE_COLOUR 0xFFFF
#define BLACK_COLOUR 0x0000
#define RED_COLOUR   0xF800
#define GREEN_COLOUR 0x07E0
#define BLUE_COLOUR  0x001F

  _led1 = 0;
  while (true) {
    wait_ms(1000); // Pause for 1 seconds
    st7789_fillRect(40, 40, 100, 50, BLACK_COLOUR);
    wait_ms(1000); // Pause for 1 seconds
    st7789_fillRect(40, 40, 100, 50, WHITE_COLOUR);
    wait_ms(1000); // Pause for 1 seconds
    st7789_fillRect(40, 40, 100, 50, RED_COLOUR);
    wait_ms(1000); // Pause for 1 seconds
    st7789_fillRect(40, 40, 100, 50, GREEN_COLOUR);
    wait_ms(1000); // Pause for 1 seconds
    st7789_fillRect(40, 40, 100, 50, BLUE_COLOUR);
    _led1 = !_led1;
  }

  printf("main: 1\r\n");
  _led1 = 1;
  wait_ms(2000); // Pause for 2 seconds
  printf("main: 2\r\n");
  _led1 = 0;
  wait_ms(2000); // Pause for 2 seconds
  _led1 = 1;
  printf("main: 3\r\n");
  queue.dispatch_forever();

    // mySPI.format(8, 1); // Mode 1.
    // mySPI.frequency(8000000);
    // display = new Adafruit_SSD1306_Spi(mySPI, P0_16, P0_17, P0_14 , 128, 64);

    // BLE& ble = BLE::Instance();
    // events::EventQueue queue;

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // display.display();
  wait_ms(2000); // Pause for 2 seconds

  // Clear the buffer
  // display.clearDisplay();

  // Draw a single pixel in white
  // display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  // display.display();
  wait_ms(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  // testdrawline();      // Draw many lines

  // testdrawrect();      // Draw rectangles (outlines)

  // testfillrect();      // Draw rectangles (filled)

  // testdrawcircle();    // Draw circles (outlines)

  // testfillcircle();    // Draw circles (filled)

  // testdrawroundrect(); // Draw rounded rectangles (outlines)

  // testfillroundrect(); // Draw rounded rectangles (filled)

  // testdrawtriangle();  // Draw triangles (outlines)

  // testfilltriangle();  // Draw triangles (filled)

  // testdrawchar();      // Draw characters of the default font

  // testdrawstyles();    // Draw 'stylized' characters

  // testscrolltext();    // Draw scrolling text

  // testdrawbitmap();    // Draw a small bitmap image

  // // Invert and restore display, pausing in-between
  // display.invertDisplay(true);
  // wait_ms(1000);
  // display.invertDisplay(false);
  // wait_ms(1000);

  // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
//  pc.printf("hahah");
    // printf("\r\n PERIPHERAL \r\n\r\n");
    // SMDevicePeripheral peripheral(ble, queue);
    // peripheral.run();

    printf("\r\n main: EXIT \r\n\r\n");
    return 0;
}