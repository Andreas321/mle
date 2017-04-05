/*
 * mle.h
 *
 *  Created on: Mar 19, 2017
 *      Author: user
 */

#ifndef MLE_H_
#define MLE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "core/net/nbr-table.h"
#include "mle-tlv.h"


#define RECEIVE_STATE 		0x80
#define TRANSMIT_STATE 		0x40
#define PRIORITY			0x20


//Can i write this to the data field of the already existing neighbour tables? nbr table or uip

typedef struct mle_node_neighbours{

	uip_802154_shortaddr 	short_address;							//Maybe change this to the "official" one		//Or change to lladdr_t
	uint8_t 				mode;
	uint32_t 				timeout;
	uint8_t 				iop;
	uint8_t 				incoming_idr;
	uint8_t 				link_cost;
	uint32_t 				latest_mle_frame_counter;

}mle_neighbour_t;

mle_neighbour_t mle_neighbours[NBR_TABLE_MAX_NEIGHBORS];//= {{{0}}};

//static mle_neighbour_t empty_mle_structure;



//single hop Neighbours and their neighbours
typedef struct mle_node_table{

	mle_neighbour_t neighbour[NBR_TABLE_MAX_NEIGHBORS];

}mle_node_table_t;


//This function will be placed inside all message handlers
int node_familiarity_check(mle_tlv_t * tlv);

mle_neighbour_t * mle_neighbour_table_lookup(/*source address*/uip_802154_shortaddr source_address);



#endif /* MLE_H_ */
