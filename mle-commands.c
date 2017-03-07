/*
 * MLE-Commands.c
 *
 *  Created on: Feb 13, 2017
 *      Author: user
 */


#include "mle-commands.h"
#include "mle-tlv.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
//refering to core/net/rpl/rpl.icmp6.c

static void auxiliary_header(){

	//Probably will want to include all this header info in separate function
	/*if(message secured){
	 *
	 * buffer[0] = 0;
	 *
	 * Auxiliary header();
	 * 		buffer[1] = security control;
	 *
	 *
	 * 		buffer[2][3][4][5] = Frame Counter
	 *
	 *
	 * 		buffer[6] = Key identifier (length of 0/1/5/9)
	 *
	 *}
	 *else{
	 *		buffer[0] = 255;
	 *
	 *
	 *}
	 */


}

uint16_t link_request_out(uint8_t *buffer){

	uint16_t length;

	//Will probably call header function here
	//initial byte indicating unsecure message
	buffer[0] = 255;					//For example test

	//buffer[next byte] = LinkRequest; CommandType = 0;
	mle_command_type_t LinkRequest = 0;
	buffer[1] = LinkRequest;						//For example test, LinkRequest

		//Maybe (Must be a cleverer way)
		/*if(auxiliaryheaderflag){
		 	 buffer[5] = LinkRequest;
		 }
		 else{
		 	 buffer[1] = LinkRequest;
		 }*/



	/*********TLVs***************/
	//Source Address
		//Long and Short
	//Mode
	//TimeOut
	//Challenge

	mle_tlv_type_t SourceAddress = 0;
	memcpy(&buffer[2],mle_tlv_write(SourceAddress),4);									//temporarily set this size to 4
	printf("The size of mel tlv write is %u\n",sizeof(mle_tlv_write(SourceAddress)));
	//mle_tlv_type_t Mode = 1;
	//memcpy((void*) &buffer[2],mle_tlv_write(Mode),sizeof(mle_tlv_write(Mode)));





	length = sizeof(buffer);					//Will this actually return the length of non-empty terms?

	//Refer to rpl-icmp6.c
	//Refer to uip-icmp6.c and uip-icmp6.h
	//  tcpip_ipv6_output();


	//return length actually used - George
	return length;
}

uint16_t link_accept_out(uint8_t *buffer){

	uint16_t length;
	//auxiliary header function
	buffer[0] = 255;
	//CommandType = 1;
	mle_command_type_t LinkAccept = 1;
	buffer[1] = LinkAccept;


	/*********TLVs************/
	//Source Address
		//Long and Short
	//Mode
	//TimeOut
	//Response
	//LinkLayerFrameCounter
	//MLEFrameCounter
	//Refer to rpl-icmp6.c
	//Refer to uip-icmp6.c and uip-icmp6.h
	//  tcpip_ipv6_output();

	mle_tlv_type_t SourceAddress = 0;
	//buffer[2] = mle_tlv_write(SourceAddress);


	length = sizeof(buffer);
	return length;
}


void link_accept_and_request_function(){}
void link_reject_function(){}
void advertisement_function(){}
void update_function(){}
void update_request_function(){}

