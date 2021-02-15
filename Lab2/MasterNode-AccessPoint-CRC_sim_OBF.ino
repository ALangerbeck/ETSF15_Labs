// Simulated simple MasterNode and AccessPoint

int bin_to_int(byte bin_array[], int start, int len);
void int_to_binarray(int in, int len, byte bin_array[], int start);
void transmit(byte symbols[]);
byte f1(byte in_array[]);
bool f2(byte in_array[]);

const int IO1 = 1; // Tx
const int IO2 = 13; // Rx
const int IO3 = 03; // address sw #4 (LSb)
const int IO4 = 04; // address sw #3
const int T_S = 100; // symbol time
const int NUM_SYMBS = 48; // 8+8+32
const unsigned int START = 43646;

int i;
byte symbol_in[48];
byte symbol_out[48];
int addrsw3, addrsw4;

int frameSource;
int frameDest;
int frameType;
int frameSEQ;
int framePayload;
int frameCRC;

void setup() {
	pinMode(IO2, INPUT);
	pinMode(IO1, OUTPUT);
	pinMode(IO3, INPUT);
	pinMode(IO4, INPUT);
	digitalWrite(IO1,LOW);
}

void loop() {
	while (digitalRead(IO2) == LOW) {
	} // wait for PREAMBLE start
	delay(0.5*T_S);
	for (i = 0; i < NUM_SYMBS; i++) {
		symbol_in[i] = digitalRead(IO2);
		delay(T_S);
	}			

	transmit(symbol_in);

	addrsw4 = digitalRead(IO3);
	addrsw3 = digitalRead(IO4);

	if ((addrsw4 == 0) | (addrsw4 == 1 && f2(symbol_in))) {

		frameSource = bin_to_int(symbol_in,16,4);
		frameDest = bin_to_int(symbol_in,20,4);
		frameType = bin_to_int(symbol_in,24,4);
		frameSEQ = bin_to_int(symbol_in,28,4);
		framePayload = bin_to_int(symbol_in,32,8);
		frameCRC = bin_to_int(symbol_in,40,8);

		i = 48;
		int_to_binarray(START, 16, symbol_out, 0);
		int_to_binarray(frameDest, 4, symbol_out, 16);
		int_to_binarray(frameSource, 4, symbol_out, 20);
		int_to_binarray(1, 4, symbol_out, 24); //type = ACK
		int_to_binarray(0, 8, symbol_out, 32); // payload = 0
		int_to_binarray(0, 8, symbol_out, 40); // CRC = 0

		if (addrsw3 == 0) {
			int_to_binarray(frameSEQ, 4, symbol_out, 28);
		} else {
			int_to_binarray(frameSEQ-1, 4, symbol_out, 28);
		}

		if (addrsw4 == 1) {
			int_to_binarray(f1(symbol_out), 8, symbol_out, 40);
		}

		delay(3000); // simulate MasterNode -- AP transfer
		transmit(symbol_out);
	}
}

int bin_to_int(byte bin_array[], int start, int len) {
	int k, j, result;
	result = 0;
	k = start; 
	j = 0;
	while (j<len) {
		//Serial.println(result);
		result <<= 1;
		result |= (bin_array[k] & 1);
		++j;
		++k;
	}
	return result;
}

void int_to_binarray(int in, int len, byte bin_array[], int start) {
	int k, j;
	k = start + len - 1;
	j = 0;
	while (j<len) {
		bin_array[k] = in & 1;
		in >>= 1;
		--k;
		++j;
	}
}

void transmit(byte symbols[]) {
	int k;
	for (k = 0; k < NUM_SYMBS; k++) {
		digitalWrite(IO1, symbols[k]);
		delay(T_S);
	}
	digitalWrite(IO1, LOW);
}

byte f1(byte q0[]){byte q1=0xA7;byte q2=0;int q3;
byte q4;q3=16;while(q3<48){q4=q2&0x80;q2<<=1;q2|=q0[q3]&1;if(q4!=0){q2^=q1;}++q3;}
return q2;}
bool f2(byte q0[]){byte q1=0xA7;int q3=16;byte q4;
byte q2=0;while(q3<48){q4=q2&0x80;q2<<=1;q2|=q0[q3]&1;if(q4!=0){q2^=q1;}++q3;}return(bool)(q2==0);}