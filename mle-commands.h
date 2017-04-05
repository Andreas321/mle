/*
 * MLE-Commands.h
 *
 *  Created on: Feb 13, 2017
 *      Author: user
 */

#ifndef MLE_COMMANDS_H_
#define MLE_COMMANDS_H_

//#error explode
#include "stdint.h"
#include "simple-udp.h"
#include "mle-tlv.h"

#if IDLE_RF_ON_BIT

#define NO_OF_LINK_REQUEST_TLV 				0x03
#define NO_OF_LINK_ACCEPT_TLV				0x05
#define NO_OF_LINK_ACCEPT_AND_REQUEST_TLV	0x06

#else															//TimeOut TLV will be included in some messages

#define NO_OF_LINK_REQUEST_TLV 				0x04
#define NO_OF_LINK_ACCEPT_TLV				0x06
#define NO_OF_LINK_ACCEPT_AND_REQUEST_TLV	0x07


#endif


//
//#if IDLE_RF_ON_BIT MODE_TLV_IDLE_RF_OFF							//TimeOut TLV will be included in some messages
//
//#define NO_OF_LINK_REQUEST_TLV 				0x04
//#define NO_OF_LINK_ACCEPT_TLV				0x06
//#define NO_OF_LINK_ACCEPT_AND_REQUEST_TLV	0x07
//
//#else
//
//#define NO_OF_LINK_REQUEST_TLV 				0x03
//#define NO_OF_LINK_ACCEPT_TLV				0x05
//#define NO_OF_LINK_ACCEPT_AND_REQUEST_TLV	0x06
//
//
//#endif


#define NO_OF_LINK_REJECT_TLV				0x00				//Guess
#define NO_OF_ADVERTISEMENT_TLV				0x02				//Guess. Possibly 3 when Route TLV is made?
#define NO_OF_ROUTE_ADVERTISEMENT_TLV		0x03
#define NO_OF_UPDATE_TLV					0x01
#define NO_OF_UPDATE_REQUEST_TLV			0x00				//Guess

enum{									//will need to change this as enum is in int not uint8_t
	LinkRequest				= 0,
	LinkAccept				= 1,
	LinkAcceptandRequest	= 2,
	LinkReject				= 3,
	Advertisement			= 4,
	Update					= 5,
	UpdateRequest			= 6,


};
typedef uint8_t mle_command_type_t;



uint16_t link_request_out(uint8_t *buffer);
uint16_t link_accept_out(uint8_t *buffer,mle_tlv_t *challenge_tlv);
uint16_t link_reject_out(uint8_t *buffer);
uint16_t link_accept_and_request_out(uint8_t *buffer,mle_tlv_t *challenge_tlv);
//OLD uint16_t link_accept_and_request_out(uint8_t *buffer,uint8_t *challenge_received);
uint16_t advertisement_out(uint8_t *buffer);

uint16_t route_advertisement_out(uint8_t *buffer);




uint8_t update_out(uint8_t *buffer, uint8_t param_id, uint32_t delay, uint32_t value);
void update_request_function();

uint16_t link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer);
uint16_t new_link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer);

uint16_t link_accept_in();
uint16_t link_accept_and_request_in();
void link_reject_in();
void advertisement_handler();
void route_advertisement_handler(uint8_t *data, uint16_t datalength);
int update_in(const uint8_t *data, uint16_t datalength, uint32_t * interval, uint32_t * parameter_value);

#endif /* MLE_COMMANDS_H_ */
