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
#include "simple-udp.h"

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

	uint16_t length = 2;
	//uint16_t length2;				//temporary


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
	//Mode
	//TimeOut
	//Challenge

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t Mode = 1;
	length += mle_tlv_write(Mode,buffer,length);

//	mle_tlv_type_t Challenge = 3;
//	length += mle_tlv_write(Challenge,buffer,length);

	//Refer to rpl-icmp6.c
	//Refer to uip-icmp6.c and uip-icmp6.h
	//  tcpip_ipv6_output();

	//length += length2;									//This '2' assumes no security header
	printf("length is %d",length);
	//return length actually used - George

	return length;
}

uint16_t link_accept_out(uint8_t *buffer){

	printf("\nin the link accept function");
	uint16_t length = 2;
	//auxiliary header function
	buffer[0] = 255;
	//CommandType = 1;
	mle_command_type_t LinkAccept = 1;
	buffer[1] = LinkAccept;



	/*********TLVs************/
	//Source Address
	//Mode
	//TimeOut
	//Response
	//LinkLayerFrameCounter
	//MLEFrameCounter

	//Refer to rpl-icmp6.c
	//Refer to uip-icmp6.c and uip-icmp6.h
	//  tcpip_ipv6_output();

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t Mode = 1;
	length += mle_tlv_write(Mode,buffer,length);
/*
 	mle_tlv_type_t TimeOut = 2;
	length += mle_tlv_write(TimeOut,buffer,length);

	mle_tlv_type_t Response = 4;
	length += mle_tlv_write(Response,buffer,length);

	mle_tlv_type_t LinkFrameCounter = 6;
	length += mle_tlv_write(LinkFrameCounter,buffer,length);

	mle_tlv_type_t MLEFrameCounter = 8;
	length += mle_tlv_write(MLEFrameCounter,buffer,length);

 */

	length = sizeof(buffer);
	return length;
}

uint16_t link_reject_out(uint8_t *buffer){
	printf("\nin the link reject function");
	uint16_t length = 2;


	return length;
}
uint16_t link_accept_and_request_out(uint8_t *buffer){

	printf("\nin the link accept and request function");
	uint16_t length = 2;
	/*********TLVs************/
	//Source Address
	//Mode
	//TimeOut
	//Challenge
	//Response
	//LinkLayerFrameCounter
	//MLEFrameCounter

	buffer[0] = 255;
	mle_command_type_t link_accept_and_request = 1;
	buffer[1] = link_accept_and_request;

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t Mode = 1;
	length += mle_tlv_write(Mode,buffer,length);
/*
 	mle_tlv_type_t TimeOut = 2;
	length += mle_tlv_write(TimeOut,buffer,length);

	mle_tlv_type_t Challenge = 3;
	length += mle_tlv_write(Challenge,buffer,length);

	mle_tlv_type_t Response = 4;
	length += mle_tlv_write(Response,buffer,length);

	mle_tlv_type_t LinkFrameCounter = 6;
	length += mle_tlv_write(LinkFrameCounter,buffer,length);

	mle_tlv_type_t MLEFrameCounter = 8;
	length += mle_tlv_write(MLEFrameCounter,buffer,length);

 */
	return length;

}


void advertisement_function(){}
void update_function(){}
void update_request_function(){}

uint8_t link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer){

	//what are conditions of accept/reject
		//other than check security header
		//or are messages without security headers just ignored
	uint8_t outputlength;

	if(data[0] == 255){													//unsecure for the moment

		//if(){
			outputlength  = link_accept_and_request_out(output_buffer);
		//}
		//else{
			//link_accept_out();
		//}
	}
	else{
		outputlength = link_reject_out(output_buffer);
	}

	return outputlength;
}
