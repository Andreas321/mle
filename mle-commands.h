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
uint16_t link_accept_out();
uint16_t link_reject_out(uint8_t *buffer);
uint16_t link_accept_and_request_out(uint8_t *buffer);



void advertisement_function();
void update_function();
void update_request_function();

uint8_t link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer);




#endif /* MLE_COMMANDS_H_ */
