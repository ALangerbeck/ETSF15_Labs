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

int ledChoice;
void L1_send(unsigned long frame,int framelenght);

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
			state = HALT;
			// ---
			break;

		case L1_RECEIVE:
			Serial.println("[State] L1_RECEIVE");
			// +++ add code here 

			// ---
			break;

		case L2_DATA_SEND:
			Serial.println("[State] L2_DATA_SEND"); 
			// +++ add code here 
			tx.frame_from = 0;
			tx.frame_to = 0;
			tx.frame_type = 0;
			tx.frame_seqnum = 0;
			tx.frame_payload = ledChoice;
			tx.frame_crc = 0;
			tx.frame_generation();
			state = L1_SEND;
			// ---
			break;

		case L2_RETRANSMIT:
			Serial.println("[State] L2_RETRANSMIT");
			// +++ add code here 

			// ---
			break;

		case L2_FRAME_REC:
			Serial.println("[State] L2_FRAME_REC");
			// +++ add code here 

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

			// ---
			break;

		case APP_PRODUCE: 
			Serial.println("[State] APP_PRODUCE");
			// +++ add code here
			ledChoice = sh.select_led();
			//Serial.println(ledChoice);
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
