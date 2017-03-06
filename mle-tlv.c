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

uint8_t * mle_tlv_write(mle_tlv_type_t type){

	mle_tlv_t tlv;
	//uint8_t *parsedtlv;

	tlv.type = type;

	switch(tlv.type){

		case 0://SourceAddress:								//Why are these unresolved? Because enum is of type int (4 bytes) as opposed to uint8_t
			*(tlv.value) = source_address_function();		//16 bit can be multiple
			break;

		case 1://Mode:
			*(tlv.value) = mode_function();				//8 bit
			break;
/*
		case 2://Timeout:
			tlv.value = timeout_function();				//32 bit
			break;

		case 3://Challenge:
			tlv.value = challenge_function();			//atleast 32 bit
			break;

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


	tlv.length = sizeof(tlv.value);					//INSERTED 2 TEMPORARILLY, length is 4 atm as tlv.value is initialised as uint32_t??
	//parsedtlv = mle_tlv_parser(&tlv);
	//memcpy(parsedtlv,mle_tlv_parser(&tlv),sizeof(mle_tlv_parser(&tlv)));					//Check if *parsedtlv works instead of parsedtlv

	uint8_t parsedtlv[30] = {0};
	mle_tlv_parser(&tlv, parsedtlv);
	return parsedtlv;
}


//parsing the TLV structure to the output array
//NEW function
void mle_tlv_parser(mle_tlv_t * tlv, uint8_t tbp[]){
	//new function: pass the array by reference rather than declaring a new static array
	uint8_t i;
	uint8_t j;
	*tbp = tlv->type;
	*(tbp + 1) = tlv->length;


	//Original before union definition
	for(i=2, j=0; i<(tlv->length +2); i++,j++){

		*(tbp + i) = (*(tlv->value) >> (8*j)) | *(tbp + i);
	}

	//Possible solution to union problem
/*
	if(tlv->length == 1){

		for(i=2, j=0; i<(tlv->length +2); i++,j++){

			*(tbp + i) = ((tlv->value.u8) >> 8) | *(tbp + i);
		}
	}
	else if(tlv->length == 2){

		for(i=2, j=0; i<(tlv->length +2); i++,j++){

			*(tbp + i) = ((tlv->value.u16) >> (8*j)) | *(tbp + i);
		}
	}*/




	//i++;
	//*(tbp+i) = '\0';				//may need to think about terminating the array. Issue as uint8_t array as opposed to int array

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

uint16_t source_address_function(){

	uint16_t value;

	//containing 16 bit MAC address two MSB

	//Somehow need to pass this bytes 8 and 9 of addr

	//value = 0x5555;								//example for now


	static uip_ipaddr_t ipaddr;
	uint8_t state;
	uint8_t i;


	uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);					//Sets the IPv6 address
	uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);											//Applies link-local(?) address to IPv6 and toggle link-local/multicast bit
	uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);



	for(i=0;i<2;i++){
		memcpy(&value + i,ipaddr.u8[i+8],sizeof(uint8_t));
	}

/*
 *		//try this after
 * 		memcpy(&value,ipaddr.u8[8],2*sizeof(uint8_t));
 */




	return value;
}
uint8_t mode_function(void){		//void for now
/*

	byte string representing the mode by which this link is used by the source of the message
	Check the format of Capability Information field in 802.15.4 Associate Command page 150
*/
	//***************************MAYBE LOOKUP THE CAPABILTIES OF A Z1 mote*******************************************

	uint8_t value = 0;
	value  = value | 80;			//Setting bit[0] and bit[3] to 1 for now

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
	return value;
}
uint32_t timeout_function(){

	//32 bit uint of the expected maximum interval between transmissions in seconds
	uint32_t value;
	//Need to understand how to use the system timer.
	return value;
}
uint32_t challenge_function(){						//openthread has this returning const uint8_t *, where the variable is declared uint8_t mChallenge[kmaxSize]
	/*
		randomly chosen byte string
		rfc4086 describe generation
		must be atleast 4 bytes in length
		new value for each transmission

		Open thread states it has a max size of 8 bytes in src/core/thread/mle_tlvs.hpp

	*/
		//refer to mle_router.cpp line 547
	//"size of challenge is determined by the network configuration"
	uint32_t value;
	uint16_t valueA;
	uint16_t valueB;

	srand(time(NULL));
	valueA = rand();
	valueB = rand();
	value = (valueA << 16) | valueB;

	return value;
}
uint32_t response_function(){
	uint32_t value;

	return value;
}
uint32_t linklayer_frame_counter_function(){
	uint32_t value;

	return value;
}
void link_quality_function(){

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
void parameter_function(){
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
uint32_t mle_frame_counter_function(){
	uint32_t value;
	/*
			Contains senders outgoing frame counter, 32 bit uint
	*/

	return value;
}

