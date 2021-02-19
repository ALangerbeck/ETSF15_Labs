////////////////////////////
//
// V20 Skeleton.ino for UnoArduSim
// 
// Uses Simlib library
//
// 2020-12-23 Jens Andersson
//
////////////////////////////

#include <simlib.h>

//
// Runtime
//

//////////////////////////////////////////////////////////
//
// Add your global constant and variabel declaretions here
//

int state = NONE;
Shield sh; // note! no () since constructor takes no arguments
Transmit tx;
Receive rx;
unsigned long inputFrame;
int messageArray[2];
int uniqueSeq;
int crcgen(unsigned long frame);

void L1_send(unsigned long frame,int framelenght);
boolean L1_receive(int timeout);

//////////////////////////////////////////////////////////

//
// Code
//
void setup() {
	sh.begin();

	//////////////////////////////////////////////////////////
	//
	// Add init code here
	//

	state = APP_PRODUCE;
	sh.setMyAddress(4);
	uniqueSeq = 10;
	// Set your development node's address here

	//////////////////////////////////////////////////////////
}

void loop() {

	//////////////////////////////////////////////////////////
	//
	// State machine
	// Add code for the different states here
	//

	switch(state){

		case L1_SEND:
			Serial.println("[State] L1_SEND");
			// +++ add code here 
			L1_send(tx.frame,LEN_FRAME);
			state = L1_RECEIVE;
			// ---
			break;

		case L1_RECEIVE:
			Serial.println("[State] L1_RECEIVE");
			// +++ add code here 
			if(L1_receive(20000)){
				state = L2_FRAME_REC;
			}else{
				state = L2_RETRANSMIT;
			}
			// ---
			break;

		case L2_DATA_SEND:
			Serial.println("[State] L2_DATA_SEND"); 
			// +++ add code here 
			tx.frame_from = sh.getMyAddress();
			tx.frame_to = messageArray[0];
			tx.frame_type = 2;
			tx.frame_seqnum = uniqueSeq;
			tx.frame_payload = messageArray[1];
			tx.frame_generation();
			//Serial.println(tx.frame);
			tx.add_crc(crcgen(tx.frame));
			tx.tx_attempts = 0;
			state = L1_SEND;

			uniqueSeq++;
			// ---
			break;

		case L2_RETRANSMIT:
			Serial.println("[State] L2_RETRANSMIT");
			// +++ add code here 
			if(tx.tx_attempts < 3){
					tx.tx_attempts++;
					state = L1_SEND;
				}else{
					state = APP_PRODUCE;
				}
			// ---
			break;

		case L2_FRAME_REC:
			Serial.println("[State] L2_FRAME_REC");
			// +++ add code here 
			rx.frame = inputFrame;
			rx.frame_decompose();
			
			if((rx.frame_to != sh.getMyAddress()) | (rx.frame_type != FRAME_TYPE_ACK) | (crcgen(rx.frame) != 0)){
				state = L1_RECEIVE;
			}
			else{
				state = L2_ACK_REC;
			}
			// ---
			break;

		case L2_ACK_SEND:
			Serial.println("[State] L2_ACK_SEND");
			// +++ add code here 

			// ---
			break;

		case L2_ACK_REC:
			Serial.println("[State] L2_ACK_REC");
			// +++ add code here
			if (rx.frame_seqnum == tx.frame_seqnum){
				state = APP_PRODUCE;
			}else{
				state = L2_RETRANSMIT;
			}
			// ---
			break;

		case APP_PRODUCE: 
			Serial.println("[State] APP_PRODUCE");
			messageArray[1] = sh.select_led();
			messageArray[0] = sh.get_address();   //Should this be changed to only read switch 1&2?
			state = L2_DATA_SEND;
			// ---
			break;

		case APP_ACT:
			Serial.println("[State] APP_ACT");
			// +++ add code here 

			// ---
			break;

		case HALT:  
			Serial.println("[State] HALT");
			sh.halt();
			break;

		default:
			Serial.println("UNDEFINED STATE");
			break;
	}

	//////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////
//
// Add your functions here
//
void L1_send(unsigned long frame, int framelenght){

	for(int i = 0;i<LEN_PREAMBLE;i++){
		digitalWrite(PIN_TX, (PREAMBLE_SEQ >> (LEN_PREAMBLE - (i+1))) & 0b01);
		delay(T_S);
	}

	for(int i = 0;i<LEN_SFD;i++){
		digitalWrite(PIN_TX, (SFD_SEQ >> (LEN_SFD - (i+1))) & 0b01);
		delay(T_S);
	}

	for(int i = 0;i<framelenght;i++){
		digitalWrite(PIN_TX, (frame >> (framelenght - (i+1))) & 0b01);
		delay(T_S);
	}

	digitalWrite(PIN_TX, LOW);
}

boolean L1_receive(int timeout){
	long Start_time = millis(); //them when we start
	while(digitalRead(PIN_RX) == 0){
		if(millis() - Start_time > 20000){
			return false;
		}
	}
	byte symbolBuffer = 0b00;
	delay(0.5*T_S);
	while(symbolBuffer != SFD_SEQ){
		if(millis() - Start_time > 20000){
			return false;    
		}
		symbolBuffer = (symbolBuffer << 1) | digitalRead(PIN_RX);
		delay(T_S);
	}
	digitalWrite(DEB_1,HIGH);
	unsigned long byteBuffer = 0b00;
	for(int i=0;i<LEN_FRAME;i++){
		byteBuffer = (byteBuffer << 1) | digitalRead(PIN_RX);
		digitalWrite(DEB_2,digitalRead(PIN_RX));
		delay(T_S);
	}

	inputFrame = byteBuffer;
	digitalWrite(DEB_2,HIGH);
	return true;
}

int crcgen(unsigned long frame) {
	byte crc = 0x00;
	byte msb = 0x00;

	for (int i = LEN_FRAME-1; i >= 0; i--) {
		msb = (crc & 0x80);
		crc <<= 1;
		crc |= ((frame >> i) & 0x01);

		if (msb != 0) {
			crc ^= 0xA7; // Xor with Generator Poly
		}
	}
	return crc;
}