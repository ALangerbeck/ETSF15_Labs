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
			Serial.println(ledChoice);
			state = HALT;
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
