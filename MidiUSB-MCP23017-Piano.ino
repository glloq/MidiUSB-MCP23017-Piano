#include <MIDIUSB.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;
Adafruit_MCP23017 mcp4;
Adafruit_MCP23017 mcp5;
Adafruit_MCP23017 mcp6;

int noteStates[88];
int numNotesPlaying = 0;

void setup() {
  // Initialize MCP23017 modules
  mcp1.begin(0);
  mcp2.begin(1);
  mcp3.begin(2);
  mcp4.begin(3);
  mcp5.begin(4);
  mcp6.begin(5);

  // Initialize note states to 0 (not playing)
  for (int i = 0; i < 88; i++) {
    noteStates[i] = 0;
  }

  // Configure pins as outputs
  for (int i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp2.pinMode(i, OUTPUT);
    mcp3.pinMode(i, OUTPUT);
    mcp4.pinMode(i, OUTPUT);
    mcp5.pinMode(i, OUTPUT);
    mcp6.pinMode(i, OUTPUT);
  }

  // Configure hardware interrupt for note timing
  cli(); // disable global interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0; // initialize counter value to 0
  OCR1A = 62500; // = 16MHz / (256 * 4) - 1 (prescaler = 256)
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12); // prescaler = 256
  TIMSK1 |= (1 << OCIE1A); // enable interrupt on OCR1A match
  sei(); // enable global interrupts
}

void loop() {
  // Check for MIDI messages
  if (MidiUSB.available()) {
    midiEventPacket_t midiEvent = MidiUSB.read();

    // Check if it's a note on message
    if (midiEvent.header == 0x09) {
      int note = midiEvent.byte1 - 21; // convert MIDI note number to array index
      noteStates[note] = 1; // mark note as playing
      numNotesPlaying++; // increment number of notes playing
    }
    // Check if it's a note off message
    else if (midiEvent.header == 0x08) {
      int note = midiEvent.byte1 - 21; // convert MIDI note number to array index
      noteStates[note] = 0; // mark note as not playing
      numNotesPlaying--; // decrement number of notes playing
    }
  }
}

// Interrupt service routine for note timing
ISR(TIMER1_COMPA_vect) {  
 // Play notes
  for (int i = 0; i < 88; i++) {
    if (noteStates[i] == 1) {
      int mcpNum = i / 16; // determine which MCP23017 module controls this note
      int mcpPin = i % 16; // determine which pin on the MCP23017 controls this note
      switch (mcpNum) {
        case 0:
          mcp1.digitalWrite(mcpPin, HIGH);
          break;
        case 1:
          mcp2.digitalWrite(mcpPin, HIGH);
          break;
        case 2:
          mcp3.digitalWrite(mcpPin, HIGH);
          break;
        case 3:
          mcp4.digitalWrite(mcpPin, HIGH);
          break;
        case 4:
          mcp5.digitalWrite(mcpPin, HIGH);
          break;
        case 5:
          mcp6.digitalWrite(mcpPin, HIGH);
          break;
      }
    }
  }

  // Delay to allow notes to play for one timer tick
  delay(15);

  // Stop notes
  for (int i = 0; i < 88; i++) {
    if (noteStates[i] == 1) {
      int mcpNum = i / 16; // determine which MCP23017 module controls this note
      int mcpPin = i % 16; // determine which pin on the MCP23017 controls this note
      switch (mcpNum) {
        case 0:
          mcp1.digitalWrite(mcpPin, LOW);
          break;
        case 1:
          mcp2.digitalWrite(mcpPin, LOW);
          break;
        case 2:
          mcp3.digitalWrite(mcpPin, LOW);
          break;
        case 3:
          mcp4.digitalWrite(mcpPin, LOW);
          break;
        case 4:
          mcp5.digitalWrite(mcpPin, LOW);
          break;
        case 5:
          mcp6.digitalWrite(mcpPin, LOW);
          break;
      }
    }
  }

  // Check if no notes are playing and turn off interrupt if so
  if (numNotesPlaying == 0) {
    TIMSK1 &= ~(1 << OCIE1A); // disable interrupt on OCR1A match
  }
}
