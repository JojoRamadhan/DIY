void loop() {

#ifdef ATMEGA32U4

  // it will read MIDI incoming messages if using ATMEGA32U4
  // MIDIread();
  // Cek apakah ada data MIDI masuk dari komputer melalui Serial (USB)
  if (Serial.available()) {
    byte midiData = Serial.read();
    sendMIDIToESP32(midiData);
  }

  // Cek apakah ada data MIDI dari ESP32 yang perlu dikirim balik ke PC
  if (Serial1.available()) {
    byte midiData = Serial1.read();
    Serial.write(midiData);
  }

#ifdef MIDI_DIN
  midi2.read();
#endif

#endif

#ifdef ATMEGA328
  MIDI.read();
#endif

#ifdef TEENSY

  // usbMIDI.read() needs to be called rapidly from loop().  When
  // each MIDI messages arrives, it return true.  The message must
  // be fully processed before usbMIDI.read() is called again.
  if (usbMIDI.read()) {
    processMIDI();
  }

#endif

#ifdef USING_BUTTONS
  buttons();
#endif

#ifdef USING_ENCODER
  encoders();
  isEncoderMoving();

#endif

#ifdef USING_ENCODER_MCP23017

  //isEncoderMoving_MCP23017();

#endif

  cpu.run(); // for threads

  /////////////////////////////////////////////
  // MOTORIZED FADERS
#ifdef USING_MOTORIZED_FADERS

  for (int i = 0; i < N_M_FADERS; i++) {

    touchLine[i] = capSense[i].capacitiveSensor(capSenseSpeed); // cs_7_8.capacitiveSensor(20); // checks the capacitance in each fader
    checkTouch(i, touchLine[i]); // checks if the fader is beign touched

    updateFader(i, faderPos[i]); // moves the fader if it receives a MIDI in message
    faderPPos[i] = faderPos[i];

  }

#endif

}

// Fungsi untuk mengirim data MIDI ke ESP32
void sendMIDIToESP32(byte midiData) {
  Serial1.write(midiData); // Kirim data melalui Serial1 ke ESP32
}
