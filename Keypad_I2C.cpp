
#include "Keypad_I2C.h"

/*
// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_I2C::begin(char *userKeymap) {
    Keypad_I2C::begin(userKeymap);
	TwoWire::begin();
	pinState = pinState_set( );
}
*/
// <<constructor>> Allows custom keymap, pin configuration, and keypad sizes.
Keypad_I2C::Keypad_I2C(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols, byte i2caddr) {
	rowPins = row;
	columnPins = col;
	sizeKpd.rows = numRows;
	sizeKpd.columns = numCols;

    keymap = userKeymap;

	setDebounceTime(10);
	setHoldTime(500);
	keypadEventListener = 0;

	startTime = 0;
	single_key = false;
	i2caddr=i2caddr;
}


// Initialize I2C
void Keypad_I2C::begin(void) {
	TwoWire::begin();
//	pinState = 0xff;
	pinState = pinState_set( );
}
/*
// Initialize I2C
void Keypad_I2C::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize I2C
void Keypad_I2C::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}
*/

// Returns a single key only. Retained for backwards compatibility.
char Keypad_I2C::getKey() {
	single_key = true;

	if (getKeys() && key[0].stateChanged && (key[0].kstate==PRESSED))
		return key[0].kchar;
	
	single_key = false;

	return NO_KEY;
}

// Populate the key list.
bool Keypad_I2C::getKeys() {
	bool keyActivity = false;

	// Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
	if ( (millis()-startTime)>debounceTime ) {
		scanKeys();
		keyActivity = updateList();
		startTime = millis();
	}

	return keyActivity;
}

// Private : Hardware scan
void Keypad_I2C::scanKeys() {
	// Re-intialize the row pins. Allows sharing these pins with other hardware.
	for (byte r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT_PULLUP);
	}

	// bitMap stores ALL the keys that are being pressed.
	for (byte c=0; c<sizeKpd.columns; c++) {
		pin_mode(columnPins[c],OUTPUT);
		pin_write(columnPins[c], LOW);	// Begin column pulse output.
		for (byte r=0; r<sizeKpd.rows; r++) {
			bitWrite(bitMap[r], c, !pin_read(rowPins[r]));  // keypress is active low so invert to high.
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		pin_write(columnPins[c],HIGH);
		pin_mode(columnPins[c],INPUT);
	}
}

// Manage the list without rearranging the keys. Returns true if any keys on the list changed state.
bool Keypad_I2C::updateList() {

	bool anyActivity = false;

	// Delete any IDLE keys
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kstate==IDLE) {
			key[i].kchar = NO_KEY;
			key[i].kcode = -1;
			key[i].stateChanged = false;
		}
	}

	// Add new keys to empty slots in the key list.
	for (byte r=0; r<sizeKpd.rows; r++) {
		for (byte c=0; c<sizeKpd.columns; c++) {
			boolean button = bitRead(bitMap[r],c);
			char keyChar = keymap[r * sizeKpd.columns + c];
			int keyCode = r * sizeKpd.columns + c;
			int idx = findInList (keyCode);
			// Key is already on the list so set its next state.
			if (idx > -1)	{
				nextKeyState(idx, button);
			}
			// Key is NOT on the list so add it.
			if ((idx == -1) && button) {
				for (byte i=0; i<LIST_MAX; i++) {
					if (key[i].kchar==NO_KEY) {		// Find an empty slot or don't add key to list.
						key[i].kchar = keyChar;
						key[i].kcode = keyCode;
						key[i].kstate = IDLE;		// Keys NOT on the list have an initial state of IDLE.
						nextKeyState (i, button);
						break;	// Don't fill all the empty slots with the same key.
					}
				}
			}
		}
	}

	// Report if the user changed the state of any key.
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].stateChanged) anyActivity = true;
	}

	return anyActivity;
}

// Private
// This function is a state machine but is also used for debouncing the keys.
void Keypad_I2C::nextKeyState(byte idx, boolean button) {
	key[idx].stateChanged = false;

	switch (key[idx].kstate) {
		case IDLE:
			if (button==CLOSED) {
				transitionTo (idx, PRESSED);
				holdTimer = millis(); }		// Get ready for next HOLD state.
			break;
		case PRESSED:
			if ((millis()-holdTimer)>holdTime)	// Waiting for a key HOLD...
				transitionTo (idx, HOLD);
			else if (button==OPEN)				// or for a key to be RELEASED.
				transitionTo (idx, RELEASED);
			break;
		case HOLD:
			if (button==OPEN)
				transitionTo (idx, RELEASED);
			break;
		case RELEASED:
			transitionTo (idx, IDLE);
			break;
	}
}

// New in 2.1
bool Keypad_I2C::isPressed(char keyChar) {
	for (byte i=0; i<LIST_MAX; i++) {
		if ( key[i].kchar == keyChar ) {
			if ( (key[i].kstate == PRESSED) && key[i].stateChanged )
				return true;
		}
	}
	return false;	// Not pressed.
}

// Search by character for a key in the list of active keys.
// Returns -1 if not found or the index into the list of active keys.
int Keypad_I2C::findInList (char keyChar) {
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kchar == keyChar) {
			return i;
		}
	}
	return -1;
}

// Search by code for a key in the list of active keys.
// Returns -1 if not found or the index into the list of active keys.
int Keypad_I2C::findInList (int keyCode) {
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kcode == keyCode) {
			return i;
		}
	}
	return -1;
}

// New in 2.0
char Keypad_I2C::waitForKey() {
	char waitKey = NO_KEY;
	while( (waitKey = getKey()) == NO_KEY );	// Block everything while waiting for a keypress.
	return waitKey;
}

// Backwards compatibility function.
KeyState Keypad_I2C::getState() {
	return key[0].kstate;
}

// The end user can test for any changes in state before deciding
// if any variables, etc. needs to be updated in their code.
bool Keypad_I2C::keyStateChanged() {
	return key[0].stateChanged;
}

// The number of keys on the key list, key[LIST_MAX], equals the number
// of bytes in the key list divided by the number of bytes in a Key object.
byte Keypad_I2C::numKeys() {
	return sizeof(key)/sizeof(Key);
}

// Minimum debounceTime is 1 mS. Any lower *will* slow down the loop().
void Keypad_I2C::setDebounceTime(uint debounce) {
	debounce<1 ? debounceTime=1 : debounceTime=debounce;
}

void Keypad_I2C::setHoldTime(uint hold) {
    holdTime = hold;
}

void Keypad_I2C::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

void Keypad_I2C::transitionTo(byte idx, KeyState nextState) {
	key[idx].kstate = nextState;
	key[idx].stateChanged = true;

	// Sketch used the getKey() function.
	// Calls keypadEventListener only when the first key in slot 0 changes state.
	if (single_key)  {
	  	if ( (keypadEventListener!=NULL) && (idx==0) )  {
			keypadEventListener(key[0].kchar);
		}
	}
	// Sketch used the getKeys() function.
	// Calls keypadEventListener on any key that changes state.
	else {
	  	if (keypadEventListener!=NULL)  {
			keypadEventListener(key[idx].kchar);
		}
	}
}

void Keypad_I2C::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // I2CxWrite( )


int Keypad_I2C::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	TwoWire::requestFrom((int)i2caddr, 1);
//	TwoWire::requestFrom((int)i2caddr, (int)i2cwidth);
	word pinVal = TwoWire::read( );
//	if (i2cwidth > 1) {
//		pinVal |= TwoWire::read( ) << 8;
//	} 
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_I2C::port_write( word i2cportval ) {
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( i2cportval & 0x00FF);
//	if (i2cwidth > 1) {
//		TwoWire::write( i2cportval >> 8 );
//	}
	TwoWire::endTransmission();
	pinState = i2cportval;
} // port_write( )

word Keypad_I2C::pinState_set( ) {
	TwoWire::requestFrom( (int)i2caddr, 1 );
//	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
	pinState = TwoWire::read( );
//	if (i2cwidth > 1) {
//		pinState |= TwoWire::read( ) << 8;
//	}
	return pinState;
} // set_pinState( )


/*
|| @changelog
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
