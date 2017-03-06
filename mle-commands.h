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
void link_accept_and_request_function();
void link_reject_function();
void advertisement_function();
void update_function();
void update_request_function();





#endif /* MLE_COMMANDS_H_ */
