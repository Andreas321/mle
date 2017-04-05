/*
 * Situation.c
 *
 *  Created on: Feb 16, 2017
 *      Author: user
 */


	//Similar file to rpl-dag.c
#include <stdint.h>
#include <stdio.h>
#include "core/net/nbr-table.h"
#include "mle.h"
#include "mle-tlv.h"



//This function will be placed inside all message handlers
//int node_familiarity_check(mle_tlv_t * tlv){																	//is this even necessary? Could use existing neighbour table functions instead
//
//	uint8_t i;
//
//	if(tlv->type == 0){
//
//		for(i=0; i< NBR_TABLE_MAX_NEIGHBORS - 1;i++){
//
//			if(tlv->value == mle_neighbours[i].short_address){													//Remember source address is in network byte order
//				//familiar node
//
//				return 1;
//			}
//			else{		//unfamiliar node
//
//				//add node information with blank iop and idr fields
//				return 0;
//			}
//		}
//
//
//
//
//	}
//	else{
//
//	//#error "INCORRECT MESSAGE HANDLER CALLED"
//
//	}
//
//	return 0;
//
//}
//Looks through stored mle neighbour table
mle_neighbour_t * mle_neighbour_table_lookup(uip_802154_shortaddr source_address){

	mle_neighbour_t * mle_nbr;
	uint8_t i;

	for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

	//	if(source_address.addr == *mle_neighbours[i].short_address){
		if(memcmp(&source_address.addr[0],&mle_neighbours[i].short_address.addr[0],2)){
			*mle_nbr = mle_neighbours[i];
			return mle_nbr;

		}
		else{
			i++;
		}
	}

	return 0;
}


//Once performed link config add neighbour to table
//Set transmit state on

//Set receive state on

//Set priority on

//Changing rank based on IDR
//Changing rank based on I
//Changing Rank based on O
//Changing Rank based on priority


//Function to lookup neighbours?

//function for if the timeout expires?

////LR_in template
//message_in(){
//
//	if(LA){
//		if(uip_ds6_nbr_ll_lookup(lladdr) == NULL){				//Not a configured neighbour
//
//				//no need for R check
//				if(tlv[0] == 0){							//Source address tlv
//
//					//add nbr
//					//set transmit state to 1
//					//add mle_iop to nbr data
//					//LAR
//				}
//		}
//		else{												//is a neighbour
//
//			if(R==1){										//Has received an accept previously
//				//set transmit state to 1
//				//add mle_nbr data to nbr
//				//LA
//			}
//			else{
//				//set transmit state to 1
//				//add mle_nbr data to nbr
//				//LAR
//			}
//		}
//	}
//	else{
//
//		//LRej
//	}
//
//}
//
////LAR in template
//message_in(){
//
//
//	//will already contain sender as neighbour
//
//	//find the neighbour from the table
//	void item =	uip_ds6_nbr_ll_lookup(lladdr)
//
//	//set R=1
//
//	//Need to figure out how to cast iop and idr to neighbour table data
//
//	if(LA){
//		//set transmit state to 1
//		//add mle_nbr data to nbr
//		//LA
//	}
//	else{
//
//		//LR
//	}
//
//}
////LA in
//message_in(){
//
//	//will already contain sender as neighbour
//
//	//find the neighbour from the table
//	void item =	uip_ds6_nbr_ll_lookup(lladdr)
//
//	//set R=1
//	//Need to figure out how to cast iop and idr to neighbour table data
//}



