/*
 * MLE-TLV.c
 *
 *  Created on: Feb 13, 2017
 *      Author: user
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "mle-tlv.h"

//**************POSSIBLY MODIFY THIS FOR MLE SPECIFIC WITH uint8_t?**********************
/*int simple_udp_sendto(struct simple_udp_connection *c,
                  	  const void *data, uint16_t datalen,
                  	  const uip_ipaddr_t *to)
{
	if(c->udp_conn != NULL) {
		uip_udp_packet_sendto(c->udp_conn, data, datalen,
                          	  to, UIP_HTONS(c->remote_port));
  }
  return 0;
}*/
//***************************************************************************************/

//function for parsing input to tlv format
void mle_tlv_reader(mle_tlv_t *tlv, uint8_t *input){
	// need to decide on the type of the input buffer maybe check UDP files?

	//Maybe include checking of the security header here?
	uint8_t i;

	tlv->type = input[0]; //first 8 bits of buffer ;
	tlv->length = input[1];//next 8 bits of the buffer;

	//This assumes that type and length will always be one byte
	//Orginal method, before union
	for(i = 2; i < (tlv->length + 2); i++){
			*(tlv->value) = (*(tlv->value) << 8) | input[i];
	}

	//New method with union
	/*if(tlv->length == 1){
		for(i = 2; i < (tlv->length + 2); i++){
				tlv->value.u8 = (tlv->value.u8 << 8) | input[i];
		}
	}
	else if(tlv->length == 2){
		for(i = 2; i < (tlv->length + 2); i++){
				tlv->value.u8 = (tlv->value.u16 << 8) | input[i];
		}
	}*/


}

uint8_t mle_tlv_write(mle_tlv_type_t type, uint8_t *parsedtlv, uint8_t startingposition){

	mle_tlv_t tlv;
	uint8_t lengthoftlv;
	tlv.value = NULL;										//reset the value: not working
	tlv.type = type;
	printf("\nThe size of the NULL value is %d",sizeof(tlv.value));
	switch(tlv.type){

		case 0://SourceAddress:								//Why are these unresolved? Because enum is of type int (4 bytes) as opposed to uint8_t
			source_address_function(tlv.value);
			break;

		case 1://Mode:
			mode_function(tlv.value);				//8 bit
			break;
/*
		case 2://Timeout:
			tlv.value = timeout_function();				//32 bit
			break;

		case 3://Challenge:
			challenge_function(tlv.value);			//atleast 32 bit
			break;
*//*
		case 4://Response:
			tlv.value = response_function();			//32 bit
			break;

		case 5://LinkFrameCounter:
			tlv.value = llframecounter_function();		//32 bit
			break;

		case 6://LinkQuality:
			tlv.value = linkquality_function();			//1 byte for initial data, atleast 3 bytes per neighbour
			break;

		case 7://NetworkParameter:
			tlv.value = networkparamter_function();		//5 bytes, followed by byte string
			break;

		case 8://MleFrameCounter:
			tlv.value = mleframecounter_function();		//32 bit
			break;*/
			}
	printf("\ntlv.value = %u\n",tlv.value);
	printf("*(tlv.value) = %u\n",*(tlv.value));
	tlv.length = sizeof(tlv.value);					//
	printf("\nThe size of tlv.value inside writer is %d",sizeof(tlv.value));
	//uint8_t parsedtlv[30] = {0};


	//WILL NEED TO GIVE PARSER A STARTING POSITION
	//DEFAULT WITH NO SECURITY HEADER IS 2, THEN IT MUST ADD THE SIZE OF THE PREVIOUS TLVs
	mle_tlv_parser(&tlv, parsedtlv, startingposition);
	printf("\nThe size of parsedtlv inside mle_tlv_writer is %d",sizeof(parsedtlv));




	lengthoftlv = sizeof(tlv);
	printf("\nThe size of tlv inside writer is %d",sizeof(tlv));
	return lengthoftlv;
}


//parsing the TLV structure to the output array
//NEW function
void mle_tlv_parser(mle_tlv_t * tlv, uint8_t tbp[],uint8_t position){
	//new function: pass the array by reference rather than declaring a new static array
	//uint8_t i;
	uint8_t j;

	printf("\ninside mle-parser value is %u\n", *(tlv->value));
	printf("\ninside mle-parser value is %u\n", *(tlv->value)  << 8);

	printf("\n\n*(tbp+4) is %u",*(tbp+4));
	printf("\n*(tbp+5) is %u",*(tbp+5));
	printf("\n*(tbp+6) is %u\n",*(tbp+6));

	/*
	 * 	//tbp will now need to begin from [2]
	 * 	//input for tbp may need to change
	 *
	 *
	 *
	 */
	uint8_t limit = position +2;												//'2' is dependent on type and length remaining one byte each
	tbp[position] = tlv->type;													//Position here will need to change when the security header is added
	tbp[position+1] = tlv->length;
	for(position+2, j=0; position<(tlv->length + limit); position++,j++){
		tbp[position+2] = tlv->value[j] | tbp[position+2];
	}


	printf("\n\n*(tbp+1) is %u",*(tbp+1));
	printf("\n*(tbp+2) is %u",*(tbp+2));
	printf("\n*(tbp+3) is %u\n",*(tbp+3));
	printf("\n\n*(tbp+4) is %u",*(tbp+4));
	printf("\n*(tbp+5) is %u",*(tbp+5));
	printf("\n*(tbp+6) is %u\n",*(tbp+6));
	printf("\n\n*(tbp+7) is %u",*(tbp+7));
	printf("\n*(tbp+8) is %u\n",*(tbp+8));
	printf("the sizeof tbp inside parser is %d",sizeof(tbp));


}
/*Old function
uint8_t * mle_tlv_parser(mle_tlv_t * tlv){

	uint8_t *output;								//Does this need to be initialized?
	uint8_t i;

 	output[0] = tlv->type;

	output[1] = tlv->length;

	for(i=2; i<=(tlv->length +2); i++){

		output[i] = (tlv->value << 8) | output[i];

	}

 	return output;
 }

*/

void source_address_function(uint8_t * value){
	//containing 16 bit MAC address two MSB

	*value = 0;
	static uip_ipaddr_t ipaddr;
	  int i;
	  uint8_t state;

	uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);					//Sets the IPv6 address
	uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);											//Applies link-local(?) address to IPv6 and toggle link-local/multicast bit
	uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);


	  printf("IPv6 addresses: ");
	  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
	    state = uip_ds6_if.addr_list[i].state;
	    if(uip_ds6_if.addr_list[i].isused &&
	       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
	      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
	      printf("\n");
	    }
	  }

	value[0] = ipaddr.u8[9];
	value[1] = ipaddr.u8[8];

	printf("\n");
	printf("The first address byte is ipaddr[8] = %u From ipaddr within the Source Address function", ipaddr.u8[8]);
	printf("\n");

	printf("\n");
	printf("The second address byte is ipaddr[9] = %u From ipaddr within the Source Address function", ipaddr.u8[9]);
	printf("\n");

	printf("\n");
	printf("value is %u From within the Source Address function", *value);
	printf("\n");

	printf("\n");
	printf("value[1] is %u From within the Source Address function", value[1]);
	printf("\n");

}
void mode_function(uint8_t * value){		//void for now
/*

	byte string representing the mode by which this link is used by the source of the message
	Check the format of Capability Information field in 802.15.4 Associate Command page 150
*/
	//***************************MAYBE LOOKUP THE CAPABILTIES OF A Z1 mote*******************************************

	*value = 0;
	*value  = *value | 80;			//Setting bit[0] and bit[3] to 1 for now
	printf("\n\n\n\n*value = %u",*value);
	/*
	bit 0 = Alternate PAN coordinator;
		//set to one if the device is capable of becoming the PAN coordinator
	bit 1 = Device Type;																						//Highlighted as important
		//set to one if the device is an FFD, or zero if RFD
	bit 2 = Power Source;
		//set to one if the device is receiving power from the alternating current mains.
	bit 3 = Receive On when idle;																				//Highlighted as important
		//set to one if the device does not disable its receiver to conserve power during idle periods.
	bit 4 = Reserved;
	bit 5 = Reserved;
	bit 6 = Security Capability;
		//set to one if the device is capable of sending and receiving cryptographically protected MAC frames as specified in 7.5.8.2;
	bit 7 = Allocate Address;
		//set to one if the device wishes the coordinator to allocate a 16-bit short address as a result of the association procedure.
*/

}
void timeout_function(uint8_t * value){

	//32 bit uint of the expected maximum interval between transmissions in seconds
	//Only included if the sender is an rxOffWhenIdle device.
	//at the moment I am only considering devices that are rxOnWhenIdle as seen by the mode tlv.


}
void challenge_function(uint8_t * value){						//openthread has this returning const uint8_t *, where the variable is declared uint8_t mChallenge[kmaxSize]
	/*
		randomly chosen byte string
		rfc4086 describe generation
		must be atleast 4 bytes in length
		new value for each transmission

		Open thread states it has a max size of 8 bytes in src/core/thread/mle_tlvs.hpp

	*/
		//refer to mle_router.cpp line 547
	//"size of challenge is determined by the network configuration"
	//uint32_t value;

	uint16_t valueA;
	uint16_t valueB;

	srand(time(NULL));
	valueA = rand();
	valueB = rand();
	*value = (valueA << 16) | valueB;
	printf("\nvalueA is %u",valueA);
	printf("\nvalueB is %u",valueB);
	printf("\n*value is %u",*value);


}
void response_function(uint8_t * value){
	//atleast  32bit
}
void linklayer_frame_counter_function(uint8_t * value){
	//32 bit
}
void link_quality_function(uint8_t * value){

	//First byte is C,Res,Size  0|000|0000


	//Next bytes are Neighbour data


	//First byte is I,O,P,Res  0|0|0|00000
	//Next byte is


	/*First bit 'C' is = 1/0
		1 for complete neighbour link qualities (multicast)
		0 for incomplete (unicast)
	Reserved = Must be set to 000
	Size = size in bytes of included neighbour link address minus 1
	Neighbour data = Sequence of neighbour records
		Containing Receive and Transmit State flags
		Estimated incoming link reliability (IDR)
		Neighbour link layer addresses
		.
		.
		.
		.
		.*/

}
void parameter_function(uint8_t * value){
	/*Specifies value of link layer parameter shared across the network
			Parameter ID
				0 Channel
				1 PAN ID
				2 Permit Joining
				3 Beacon Payload
			Delay, 4 byte uint
			Value, determined by the particular parameter*/

	//network_parameter_id_t param;
	//uint32_t delay;
	//value;

}
void mle_frame_counter_function(uint8_t * value){
	/*
			Contains senders outgoing frame counter, 32 bit uint
	*/

}

