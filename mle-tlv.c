/*
 * MLE-TLV.c
 *
 *  Created on: Feb 13, 2017
 *      Author: user
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "mle-tlv.h"
#include "mle.h"
#include "lib/random.h"
#include "net/rpl/rpl-dag.c"
#include "mle-route.h"

//will i need to include #ifdef and ifndef here?
#define MLE_MAX_NO_OF_NETWORK_PARAMETERS	0x04
mle_network_parameter_t mle_network_param[MLE_MAX_NO_OF_NETWORK_PARAMETERS];



//***************************************************************************************
//OLD
/*#define MLE_MODE_IDLE_WHEN_OFF 0
#define MLE_MODE_ON_WHEN_OFF   1
//....

#ifdef MLE_CONF_DEVICE_MODE
#define MLE_DEVICE_MODE MLE_CONF_DEVICE_MODE
#else
#define MLE_DEVICE_MODE MLE_MODE_IDLE_WHEN_OFF
#endif

*/
/***************************************************************************************************************************************/
//Organises received array of input data in to tlv structures
uint8_t get_tlv(mle_tlv_t *tlv,const uint8_t * input_data,uint8_t starting_position){

	uint8_t length_of_tlv = 2;							//initialised at two since type+length=2
	uint8_t j;

	tlv->type = input_data[starting_position];
	tlv->length = input_data[starting_position +1];

	for(j=0;j<=tlv->length;j++){
		tlv->value[j] = input_data[starting_position +2+j];
	}
	////////////////////////////////////////////////////////////
	if(tlv->type == 6){

		tlv->value[2] = 64;					//Debugging test

	}
	//////////////////////////////////////////////////////////


	length_of_tlv += tlv->length;

	return length_of_tlv;
}
/***************************************************************************************************************************************/
uint8_t new_mle_tlv_reader(uint8_t * input_data, uint8_t * challenge,uint8_t starting_position){

	uint8_t tlv_length;
	uint8_t length_of_tlv = 2;							//initialised at two since type+length=2
	//MAY NEED TO CONSIDER STORING THE DIFFERENT VALUES

	switch(input_data[starting_position]){

	case 0://SourceAddress:								//Why are these unresolved? Because enum is of type int (4 bytes) as opposed to uint8_t
		tlv_length = input_data[starting_position +1];
		//add address to routetable?
		length_of_tlv += tlv_length;
		break;

	case 1://Mode:
		tlv_length = input_data[starting_position +1];
		length_of_tlv += tlv_length;
		break;
/*
	case 2://Timeout:
		tlv_length = input_data[i+1];
		length_of_tlv += tlv_length;
		break;
*/
	case 3://Challenge:
			tlv_length = input_data[starting_position +1];
			uint8_t j;
			for(j=0;j<=tlv_length;j++){
				challenge[j] = input_data[starting_position +2+j];
			}
			length_of_tlv += tlv_length;
		break;
/*
//	case 4://Response:
//		tlv_length = input_data[i+1];
//		length_of_tlv += tlv_length;
//		break;

	case 5://LinkFrameCounter:
		tlv_length = input_data[i+1];
		length_of_tlv += tlv_length;
		break;

	case 6://LinkQuality:
		tlv_length = input_data[i+1];
		length_of_tlv += tlv_length;
		break;

//	case 7://NetworkParameter:
//		tlv_length = input_data[i+1];
//		length_of_tlv += tlv_length;
//		break;

	case 8://MleFrameCounter:
		tlv_length = input_data[i+1];
		length_of_tlv += tlv_length;
		break;
*/
	case 9://Route:
		tlv_length = input_data[starting_position +1];
		length_of_tlv += tlv_length;
		break;
	}
	return length_of_tlv;
}
/***************************************************************************************************************************************/
uint8_t mle_tlv_write(mle_tlv_type_t type, uint8_t *parsedtlv, uint8_t startingposition){

	mle_tlv_t tlv;
	uint8_t lengthoftlv;
	//tlv.value = NULL;										//reset the value: not working
	tlv.type = type;
	//printf("\nThe size of the NULL value is %d",sizeof(tlv.value));


//	uint k = 0;
//	printf("\nvalue inside writer (before assigning) is:");
//	for(k=0; k< 10; k++){
//
//		printf("%u ",tlv.value[k]);
//
//	}
//	printf("\n");



	switch(tlv.type){

		case 0://SourceAddress:								//Why are these unresolved? Because enum is of type int (4 bytes) as opposed to uint8_t
			tlv.length = source_address_function(tlv.value);
			break;

		case 1://Mode:
			tlv.length = mode_function(tlv.value);				//8 bit
			break;
/*
		case 2://Timeout:
			tlv.value = timeout_function();				//32 bit
			break;
*/
		case 3://Challenge:
			tlv.length = challenge_function(tlv.value);			//atleast 32 bit
			break;
/*
		case 4://Response:
			tlv.value = response_function();			//32 bit
			break;

		case 5://LinkFrameCounter:
			tlv.value = llframecounter_function();		//32 bit
			break;
*/
		case 6://LinkQuality:
			tlv.length= new_link_quality_function(tlv.value);			//1 byte for initial data, atleast 3 bytes per neighbour
			break;
/*
		case 7://NetworkParameter:
			tlv.value = networkparamter_function();		//5 bytes, followed by byte string
			break;

		case 8://MleFrameCounter:
			tlv.value = mleframecounter_function();		//32 bit
			break;*/

		case 9://Route TLV
			tlv.length = mle_route_function(tlv.value);
			break;
			}
	//printf("\ntlv.value = %p\n",tlv.value);
	//printf("*(tlv.value) = %u\n",*(tlv.value));




//	tlv.length = sizeof(tlv.value);					//will replace with counter in tlv function



	//printf("\nThe size of tlv.value inside writer is %d",sizeof(tlv.value));
	//uint8_t parsedtlv[30] = {0};


	//WILL NEED TO GIVE PARSER A STARTING POSITION
	//DEFAULT WITH NO SECURITY HEADER IS 2, THEN IT MUST ADD THE SIZE OF THE PREVIOUS TLVs
	mle_tlv_parser(&tlv, parsedtlv, startingposition);
	//printf("\nThe size of parsedtlv inside mle_tlv_writer is %d",sizeof(parsedtlv));




	//lengthoftlv = sizeof(tlv);
	//printf("\nThe size of tlv inside writer is %d",sizeof(tlv));
	lengthoftlv = sizeof(tlv.type) + sizeof(tlv.length) + tlv.length;
	return lengthoftlv;
}
/***************************************************************************************************************************************/
//parsing the TLV structure to the output array
//NEW function
void mle_tlv_parser(mle_tlv_t * tlv, uint8_t tbp[],uint8_t position){
	//new function: pass the array by reference rather than declaring a new static array
	//uint8_t i;
	uint8_t j;




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



//	printf("DEBUGGING tlv->length is %u\n", tlv->length);
//	printf("DEBUGGING starting position is %u\n", position);
//	printf("DEBUGGING limit is %u\n\n\n\n", limit);

	for(position+2, j=0; (position+2)<(tlv->length + limit); position++,j++){
//		printf("DEBUGGING position is %u\n", position);
//		printf("DEBUGGING tbp[%u] is %u\n", position +2, tbp[position+2]);
//		printf("DEBUGGING tlv->value[%u] is %u\n", j,tlv->value[j]);
		tbp[position+2] = tlv->value[j] | tbp[position+2];
//		printf("DEBUGGING tbp[%u] after OR is %u\n", position +2, tbp[position+2]);
//		printf("DEBUGGING loop count is %u\n\n", j);
	}


}
/***************************************************************************************************************************************/
uint8_t source_address_function(uint8_t * value){
	//containing 16 bit MAC address two MSB
	//REMEMBER TO INVOLVE THE SOURCE ADDRESS MACRO IN HERE
	uint8_t length = 0;
	uip_802154_shortaddr short_address;

	*value = 0;
	static uip_ipaddr_t ipaddr;


	uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);					//Sets the IPv6 address
	uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);											//Applies link-local(?) address to IPv6 and toggle link-local/multicast bit
	uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

// Consider uip_802154_shortaddr instead
	short_address.addr[0] = ipaddr.u8[14];
	short_address.addr[1] = ipaddr.u8[15];


	value[0] = short_address.addr[1];																//May need to swap this round, may not be in network byte order
	value[1] = short_address.addr[0];
	length += 2;

	return length;
}
/***************************************************************************************************************************************/
uint8_t mode_function(uint8_t * value){		//void for now
/*

	byte string representing the mode by which this link is used by the source of the message
	Check the format of Capability Information field in 802.15.4 Associate Command page 150
*/
	//***************************MAYBE LOOKUP THE CAPABILTIES OF A Z1 mote*******************************************
	uint8_t length = 1;
	/**value = 0;
	*value  = *value | 80;			//Setting bit[1] and bit[3] to 1 for now
	printf("\n\n\n\n*value = %u",*value);
	*/
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
	*value = DEVICE_CONFIG;
	return length;
}
/***************************************************************************************************************************************/
uint8_t timeout_function(uint8_t * value){

	//32 bit uint of the expected maximum interval between transmissions in seconds
	//Only included if the sender is an rxOffWhenIdle device.
	uint8_t length = 4;
	//A preconfigured macro would define this?
	//What happens if links time out? Are they removed from the neighbour table

	return length;
}
/***************************************************************************************************************************************/
uint8_t challenge_function(uint8_t * value){						//openthread has this returning const uint8_t *, where the variable is declared uint8_t mChallenge[kmaxSize]
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
	uint8_t length = 0;
	uint16_t valueA;
	uint16_t valueB;

	valueA = random_rand();
	valueB = random_rand();
	*value = (valueA << 16) | valueB;						//This will have to be replaced by the method below at some point
//	printf("\nvalueA is %u",valueA);
//	printf("\nvalueB is %u",valueB);
//	printf("\n*value is %u",*value);
	//sizeof(value)); returns 2
	//sizeof(*(value))); returns 1


/*	*value = 0 | valueB;
	*(value+1) = (valueB >> 8) | 0;
	*(value+2) = valueA | 0;
	*(value+3) = (valueA >> 8) | 0;
*/
//	printf("\n*value is %d\n",*value);
//	printf("\n*(value+1) is %d\n",*(value+1));
//	printf("\n*(value+2) is %d\n",*(value+2));
//	printf("\n*(value+3) is %d\n",*(value+3));




	length = 4;													//This is really dependent on the network configuration

	return length;
}
/***************************************************************************************************************************************/
uint8_t response_function(mle_tlv_type_t response, uint8_t *parsedtlv, uint8_t startingposition, mle_tlv_t *challenge_tlv/*uint8_t challenge[]*/){
	uint8_t lengthoftlv = 0;
	mle_tlv_t response_tlv;
	//mle_tlv_type_t response =4;

	response_tlv.type = response;

	uint8_t i;
	for(i=0; i<challenge_tlv->length;i++){
		response_tlv.value[i] = challenge_tlv->value[i];
	}

	//response_tlv.value[0] = challenge;
	//printf("\n the size of the challenge inside the response function is %d", sizeof(challenge)); //or maybe sizeof(*(challenge))
	//response_tlv.length = sizeof(response_tlv.value);
	//response_tlv.length = sizeof(challenge);
	response_tlv.length = challenge_tlv->length;
	//may need to change this with new method ^
		//could treat like other tlv's
		//or could pass it the length from the input response_tlv.length = sizeof(challenge);
		//or could do it based on network configuration as planned for challenge tlv

	mle_tlv_parser(&response_tlv, parsedtlv, startingposition);

	lengthoftlv = sizeof(response_tlv.type) + sizeof(response_tlv.length) + response_tlv.length;

	return lengthoftlv;
}
/***************************************************************************************************************************************/
void linklayer_frame_counter_function(uint8_t * value){
	//32 bit

	/*
	 encoded as N bit, for 802.15.4 this is 32




	 //From 802.15.4
The Frame Counter field is 4 octets in length and represents the macFrameCounter attribute of the originator
of a protected frame. It is used to provide semantic security of the cryptographic mechanism used to protect
a frame and to offer replay protection.


	//The frame counter identifies the packet

	 The frame counter is
maintained by the hardware radio. The sender increments
it after encrypting each packet. When it reaches
its maximum value, the radio returns an error code and
no further encryptions are possible.

//MLE
 IEEE 802.15.4 security uses frame counters to detect replayed
messages. MLE uses a two-message challenge and response protocol to
ensure that the MLE message containing a neighbor’s frame counter is
not itself a replayed message.




	 */


}
/***************************************************************************************************************************************/

uint8_t new_link_quality_function(uint8_t * value){
	uint8_t length = 1;

	//Maybe: if mcast set C=1, else C=0;, could be used as an input parameter? C = 0 if update request is received.
	uint8_t complete = 0x80;

	uint8_t size = MLE_ADDRESS_SIZE - 1;				//Change this to "official" macro

	//Store C and size of source address
	value[0] = complete | size;

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//Set Dummy neighbours
	mle_neighbours[0].iop = 0xC0;
	mle_neighbours[0].incoming_idr = 2;
	mle_neighbours[0].short_address.addr[0] = 0;
	mle_neighbours[0].short_address.addr[1]	= 8;

	mle_neighbours[1].iop = 0x80;
	mle_neighbours[1].incoming_idr = 1;
	mle_neighbours[1].short_address.addr[0] = 0;
	mle_neighbours[1].short_address.addr[1]	= 9;

	mle_neighbours[2].iop = 0x00;
	mle_neighbours[2].incoming_idr = 0;
	mle_neighbours[2].short_address.addr[0] = 0;
	mle_neighbours[2].short_address.addr[1]	= 0;
	/////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////


	//May need to think about making this loop more resilient to future changes
	uint8_t i;
	for(i=0;i < NBR_TABLE_MAX_NEIGHBORS; i++){

		if((mle_neighbours[i].short_address.addr[0] != 0) || (mle_neighbours[i].short_address.addr[1] != 0)){
			value[length] = mle_neighbours[i].iop;
			value[length+1] = mle_neighbours[i].incoming_idr;
			value[length+2] = mle_neighbours[i].short_address.addr[1];
			value[length+3] = mle_neighbours[i].short_address.addr[0];
			length+=4;
		}
	}




	return length;
}
/***************************************************************************************************************************************/
uint8_t link_quality_function(uint8_t * value){


	uint8_t length = 1;
	//May want to use Objective Function 0? Posssibily replacing IDR?

/* C					1 if message contains *all* neighbourinf routers for which the source has link quality. Usually 1 for multicasts
 *						0 if the message  does not contain all. Unicasts normally contain only that neighbours link quality
 *
 * Res					000
 *
 * Size					Size in bytes of included neighbour link layer addresses minus 1. This supports addresses of lengths 1 to 16 bytes
 *
 * Neighbour Data		A sequence of neighbour records, each containing the receive and transmit flags, the estimated incoming link relability (IDR) and the neighbours link layer addresses
 *
 *						I					Incoming. 1 bit. '1' is the receive state for this neighbour is true
 *
 *						O					Outgoing. 1 bit. '1' if the transmit state for this neighbour is true
 *
 *						P					Priority. 1 bit. '1' if the sender expects to use this link for sending messages
 *
 *						Incoming IDR		8 bit unsigned. The estimated inverse delivery ratio of messages sent by the neihgbour to the source of this message. To allow for fractional IDR,
 *											value encoded is multiplied by 32.  A perfect link with an IDR of 1 would have a value of 0x20. A value of 0xFF indicates the link is unusuable.
 *
*						Address				link layer address of the neighbour
 */

/*#if multicast
 *  C = 128;			//first byte is 1
 *#else
 *	C = 0;
 *#endif
 *
 */

	//get rpl_of_t and take the path cost some how.
	//
	uint8_t i = 0;
	uint8_t c = 0x80;							//will need to make this variable

	uint8_t size_of_addresses = 0x01;			//supporting 2 byte address length - 1, USE THE MACRO

	value[0] = c | size_of_addresses;

	//Place loop here for each address
		//value[n] = I | O | P;
		//value[n+1] = incoming_idr;
		//value[n+2] = address
    rpl_print_neighbor_list();



//	uint8_t k;
//	for(k=0; k< 10; k++){
//
//		value[k] = NULL;
//
//	}
//	printf("\n");
//
//	value[3] = value[3] + 2;						//Value 3 contains a bit (2) that cannot be changed
//
//
//	printf("\nvalue inside LQ (before neighbours) is:");
//	for(k=0; k< 10; k++){
//
//		printf("%u ",value[k]);
//
//	}
//	printf("\n");




	//METHOD 2
//	rpl_parent_t *p = nbr_table_head(rpl_parents);
//    value[1] = rpl_get_parent_ipaddr(p)->u8[15];				//Need to check if this is the right order
//    printf("\nvalue[1] is %u",value[1]);
//    value[2] = rpl_get_parent_ipaddr(p)->u8[14];
//    printf("\nvalue[2] is %u",value[2]);
//    p = nbr_table_next(rpl_parents, p);
//
//
//    value[3] = rpl_get_parent_ipaddr(p)->u8[15];				//Need to check if this is the right order
//    printf("\nvalue[3] is %u",value[3]);
//    value[4] = rpl_get_parent_ipaddr(p)->u8[14];
//    printf("\nvalue[4] is %u",value[4]);

    //METHOD5
//	rpl_parent_t *p = nbr_table_head(rpl_parents);												//////////////////////////////////////////////////////////
//    //value[1] = 0;				//Need to check if this is the right order						//Setting this to 0 solves problems. Dont know why?
//    value[1] = 9;
//    printf("\nvalue[1] is %u",value[1]);														//////////////////////////////////////////////////////////
//    value[2] = 8;
//    printf("\nvalue[2] is %u",value[2]);
//    p = nbr_table_next(rpl_parents, p);
//
//
//    value[3] = 15;//Need to check if this is the right order
//    printf("\nvalue[3] is %u",value[3]);
//
//    value[4] = 6;
//    printf("\nvalue[4] is %u",value[4]);
//
//
//
//
//    length+=4;
//    //METHOD 3
//    rpl_parent_t *p = nbr_table_head(rpl_parents);
//    value+1 = rpl_get_parent_ipaddr(p)->u8[15];				//Need to check if this is the right order
//    value+2 = rpl_get_parent_ipaddr(p)->u8[14];
//
//     p = nbr_table_next(rpl_parents, p);
//
//
//    value+3= rpl_get_parent_ipaddr(p)->u8[15];				//Need to check if this is the right order
//    value+4= rpl_get_parent_ipaddr(p)->u8[14];



//	if(default_instance != NULL && default_instance->current_dag != NULL &&
//	      default_instance->of != NULL) {

		rpl_parent_t *p = nbr_table_head(rpl_parents);

		while(p != NULL) {
			  //const struct link_stats *stats = rpl_get_parent_link_stats(p);


			  //printf("\n\nParent node address is %u\n",rpl_get_parent_ipaddr(p)->u8[15]);
			  value[length] = rpl_get_parent_ipaddr(p)->u8[15];				//Need to check if this is the right order


			  //printf("\n\nParent node address is %u\n",rpl_get_parent_ipaddr(p)->u8[14]);
			  value[length+1] = rpl_get_parent_ipaddr(p)->u8[14];

			  length += (size_of_addresses + 1);								//This before IOP IDR are included, remember to use SIZE_OF_MLE_ADDRESS MACRO

			  p = nbr_table_next(rpl_parents, p);
			  i++;
			 // printf("\nloop count is %u\n",i);
		}

	//}


//    printf("\n\nvalue[1] = %u:\n", value[1]);
//    printf("\n\nvalue[2] = %u:\n", value[2]);
//    printf("\n\nvalue[3] = %u:\n", value[3]);
//    printf("\n\nvalue[4] = %u:\n", value[4]);
	return length;
}
/***************************************************************************************************************************************/
//This tlv will be styled differently for example purposes
//As MLE is not given the ability to decide how to change network parameters, that will be up to the example file.
uint8_t network_parameter_function(mle_tlv_type_t network_parameter, uint8_t *parsedtlv, uint8_t startingposition, uint8_t param_id, uint32_t delay, uint32_t value){
/*
	 Parameter ID 			The ID of the parameter to be changed.

	 Delay 					The delay before setting the parameter, in
	 	 	 	 	 	 	milliseconds. This is a four-byte unsigned
	 	 	 	 	 	 	integer. Having a delay gives time for the new
	 	 	 	 	 	 	value to propagate throughout the network. It
	 	 	 	 	 	 	may also be used for limiting the time a
	 	 	 	 	 	 	particular parameter setting is in use, by
	 	 	 	 	 	 	including two different values for a single
	 	 	 	 	 	 	parameter, with two different delays.

	 Value 					A byte string containing the new value of the
	 	 	 	 	 	 	parameter. The format of this value is
	 	 	 	 	 	 	determined by the particular parameter.
*/
	uint8_t lengthoftlv;
	mle_tlv_t parameter_tlv;
//	printf("\nuint32_t delay is %lu\n",delay);
	parameter_tlv.type = network_parameter;



	parameter_tlv.value[0] = param_id;

	uint8_t i;

	for(i=0; i<sizeof(delay);i++){

		parameter_tlv.value[i+1] = 0;

		parameter_tlv.value[i+1] = parameter_tlv.value[i+1] | (delay >> (8*(sizeof(delay)-i-1)));				//BIG ENDIAN //Remember to slighlty edit this to include value field

		//parameter_tlv.value[i+1] = 17;

	}

	for(i=sizeof(delay); i<(sizeof(delay)+sizeof(value));i++){

		parameter_tlv.value[i+1] = 0;

		parameter_tlv.value[i+1] = parameter_tlv.value[i+1] | (value >> (8*(sizeof(value)-i-1)));				//BIG ENDIAN //Remember to slighlty edit this to include value field



	}





	parameter_tlv.length = sizeof(param_id) + sizeof(delay) + sizeof(value);

//	printf("\nsizeof(param_id) is %u",sizeof(param_id));
//	printf("\nsizeof(delay) is %u",sizeof(delay));
//	printf("\nsizeof(value) is %u",sizeof(value));
	//parameter_tlv.length = 11;

	mle_tlv_parser(&parameter_tlv, parsedtlv, startingposition);

	//tlv.value will contain 3 fields
	//uint8_t network_parameter_id_t param;
	//uint32_t delay;
	//value;

	//Channel 					//use RADIO_PARAM_CHANNEL					//int value
	//PAD ID					//use RADIO_PARAM_PAN_ID					//int value




	lengthoftlv = sizeof(parameter_tlv.type) + sizeof(parameter_tlv.length) + parameter_tlv.length;


	return lengthoftlv;
}
/***************************************************************************************************************************************/
void mle_frame_counter_function(uint8_t * value){
	/*
			Contains senders outgoing frame counter, 32 bit uint
	*/

}
/***************************************************************************************************************************************/

/*
Search through an array of received tlv's for a specific type, function will return the index.
Yet to be tested. For now it is only included inside update in.
*/
uint8_t mle_tlv_search(mle_tlv_t tlv[], uint8_t no_of_tlv,  mle_tlv_type_t type){

	uint8_t k;
	uint8_t index;

	for(k=0;k<no_of_tlv;k++){							//May need to change no_of_tlvs as it can vary
			//Search for Challenge tlv
			if(tlv[k].type == type){
				index = k;
				return index;
			}
//			else{
//
//				return NULL;
//			}

		}

	return 10;											//Fail

}
/***************************************************************************************************************************************/
//Value of this particular tlv contains 3 fields, which are read using this function
//Will need to consider if network byte order will affect this function.
int network_param_reader(mle_tlv_t * tlv, uint32_t * delay, uint32_t * pvalue){


	//network_param_value = tlv->value[0];

	*delay = 0;

	uint8_t i;
	for(i=0; i<sizeof(uint32_t); i++){
		//printf("\nDEBUGGING: tlv->value[%u+1] is %u",i,tlv->value[i+1]);
		*delay = (*delay << 8) | (tlv->value[i+1]);



	}
//	printf("\n\nDEBUGGING IN PARAM READER: *delay inside param reader is: %lu",*delay);
//	printf("\nDEBUGGING IN PARAM READER: *delay address inside param reader is: %p\n\n",delay);
//



	*pvalue = 0;
//
//	for(i=sizeof(uint32_t); i<(sizeof(uint32_t)+sizeof(uint32_t)); i++){
//
//		printf("\nDEBUGGING: tlv->value[%u+1] is %u",i,tlv->value[i+1]);
//		*value = ((*value) << 8) | (tlv->value[i+1]);
//	}


	*pvalue = *pvalue | (tlv->value[i+1]);

	for(i=(sizeof(uint32_t)+1); i<(sizeof(uint32_t)+sizeof(uint32_t)); i++){

		//printf("\nDEBUGGING: tlv->value[%u+1] is %u",i,tlv->value[i+1]);
		*pvalue = ((*pvalue) << 8) | (tlv->value[i+1]);
		//printf("\nDEBUGGING: *value inside loop is: %lu",*pvalue);
	}

	//*value = 300;

//	printf("\n\nDEBUGGING IN PARAM READER: *value inside param reader is: %lu",*pvalue);
//	printf("\nDEBUGGING IN PARAM READER: *value address inside param reader is: %p\n\n",pvalue);



	switch(tlv->value[0]){

	case 0	:	//Channel
		return 0;
		break;
	case 1	:	//PAN ID
		return 1;
		break;
	}
//	case 2	:	//Permit Joining
//
//		break;
//
//	case 3	:	//Beacan Payload
//
//		break;
//	}

	return 5;
}
/**********************************************************************************************************************************/
void link_quality_reader(mle_tlv_t * tlv){

	//v0 = c size
	uint8_t size = tlv->value[0] & 0x0F;

	uint8_t single_neighbour_data = 2 + size + 1;


	uint8_t no_of_neighbours = tlv->length / single_neighbour_data;

	printf("\nNumber of neighbours included in advertisement = %u",no_of_neighbours);


	printf("\n|  Short Address  |  Receive State  |  Transmit State  |  Incoming Delivery Ratio  |");
	uint8_t i;
	uint8_t incoming;
	uint8_t outgoing;
	uint8_t idr;
	uint8_t address[2];

	for(i=1; i < tlv->length; i+=single_neighbour_data){

		incoming = tlv->value[i] & 0x80? 1 : 0;
		outgoing = tlv->value[i] & 0x40? 1 : 0;
		idr = 59;//tlv->value[i+1];
		address[1]	= tlv->value[i+2];
		address[0]	= tlv->value[i+3];

		printf("\n|        %u%u       ",address[0],address[1]);
		printf("|        %u        ",incoming);
		printf("|        %u         ",outgoing);
		printf("|            %u              |",idr);

	}

	printf("\n");





}

//Function for writing routes to route tlv
uint8_t mle_route_function(uint8_t * value){

	uint8_t length = 0;
	uint8_t i;


		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////
		//Set Dummy routes

		//Neighbour 08
		mle_routes[0].path_cost = 2;
		mle_routes[0].destination_short_address.addr[0] = 0;
		mle_routes[0].destination_short_address.addr[1]	= 8;
		mle_routes[0].next_hop_short_address.addr[0] 	= 0;
		mle_routes[0].next_hop_short_address.addr[1]	= 8;

		//Neighbour 09
		mle_routes[1].path_cost = 1;
		mle_routes[1].destination_short_address.addr[0] = 0;
		mle_routes[1].destination_short_address.addr[1]	= 9;
		mle_routes[1].next_hop_short_address.addr[0] 	= 0;
		mle_routes[1].next_hop_short_address.addr[1]	= 9;

		//Route to 15 via 08
		mle_routes[2].path_cost = 3;
		mle_routes[2].destination_short_address.addr[0] = 1;
		mle_routes[2].destination_short_address.addr[1]	= 5;
		mle_routes[2].next_hop_short_address.addr[0] 	= 0;
		mle_routes[2].next_hop_short_address.addr[1]	= 8;

		//Route to 13 via 09
		mle_routes[3].path_cost = 4;
		mle_routes[3].destination_short_address.addr[0] = 1;
		mle_routes[3].destination_short_address.addr[1]	= 3;
		mle_routes[3].next_hop_short_address.addr[0] 	= 0;
		mle_routes[3].next_hop_short_address.addr[1]	= 9;

		//Route to 06 via 09
		mle_routes[4].path_cost = 2;
		mle_routes[4].destination_short_address.addr[0] = 0;
		mle_routes[4].destination_short_address.addr[1]	= 6;
		mle_routes[4].next_hop_short_address.addr[0] 	= 0;
		mle_routes[4].next_hop_short_address.addr[1]	= 9;


//		//empty address
//		mle_routes[5].path_cost = 3;
//		mle_routes[5].destination_short_address.addr[0] = 1;
//		mle_routes[5].destination_short_address.addr[1]	= 1;
//		mle_routes[5].next_hop_short_address.addr[0] 	= 0;
//		mle_routes[5].next_hop_short_address.addr[1]	= 9;

		mle_routes[5].path_cost = 0;
		mle_routes[5].destination_short_address.addr[0] = 0;
		mle_routes[5].destination_short_address.addr[1]	= 0;
		mle_routes[5].next_hop_short_address.addr[0] 	= 0;
		mle_routes[5].next_hop_short_address.addr[1]	= 0;

		/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////



	//Looks through all entries of routes and writes to tlv if they're addresses are not null
	for(i=0; i < MAX_MLE_ROUTES; i++){

		if((mle_routes[i].destination_short_address.addr[0] != 0) || (mle_routes[i].destination_short_address.addr[1] != 0)){

			value[length] 		= mle_routes[i].destination_short_address.addr[1];
			value[length+1] 	= mle_routes[i].destination_short_address.addr[0];
//			value[length+2] 	= mle_routes[i].next_hop_short_address.addr[1];
//			value[length+3] 	= mle_routes[i].next_hop_short_address.addr[0];
			value[length+2] 	= mle_routes[i].path_cost;
			length += 3;

		}
	}


	return length;
}
//Reads routes from tlv and adds routes to list if new or shorter
void mle_route_reader(mle_tlv_t * tlv, uip_802154_shortaddr source_address){

	uint8_t i;
	int route_index;
	mle_route_t temp_route;



	//Used as empty space in the list of routes
	uip_802154_shortaddr empty_address;
	empty_address.addr[0] = 0;															//This syntax is probably wrong
	empty_address.addr[1] = 0;

	memcpy(&temp_route.next_hop_short_address,&source_address,MLE_ADDRESS_SIZE);

	for(i=0; i < tlv->length; i += MLE_ROUTE_DATA_SIZE){


		//store the address temporarily
		temp_route.destination_short_address.addr[1] = tlv->value[i];
		temp_route.destination_short_address.addr[0] = tlv->value[i+1];


		temp_route.path_cost = tlv->value[i+2];


		printf("\nDEBUGGING: addr[0] = %u  addr[1] = %u  path = %u",temp_route.destination_short_address.addr[0],temp_route.destination_short_address.addr[1],temp_route.path_cost );

		//perform lookup
		route_index = mle_route_lookup(&temp_route.destination_short_address);

		//if contained
		if(route_index < MAX_MLE_ROUTES){
			printf("Route found for %u", i);
			//compare path cost with that of currently listed route
			if(mle_route_compare(&temp_route, route_index)){

				//if less, replace the route
				mle_route_add(&temp_route, route_index);

			}

		}
		//find empty space in the list and add the route
		else{
			//Look for an empty space
			printf("\nDEBUGGING: Looking for empty space");
			route_index = mle_route_lookup(&empty_address);
			//if there is an empty space, add the route
			if(route_index < MAX_MLE_ROUTES){
				mle_route_add(&temp_route, route_index);
			}
		}

	}


	mle_route_table_print();


}


//Frame_counter_in function to store FC and check the latest received frame counter and compare against stored frame counters
