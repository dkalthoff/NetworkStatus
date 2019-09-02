






void setup() {
    // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  initDisplay();
  connectToWiFi();
  sendToDisplay("Connected to WiFi", 1);
}

void loop() {
  
  delay(1000); 
}
