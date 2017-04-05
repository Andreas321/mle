/*
 * mle_route.c
 *
 *  Created on: Apr 3, 2017
 *      Author: user
 */


#include <stdint.h>
#include <stdio.h>
#include "core/net/nbr-table.h"
#include "mle.h"
#include "mle-tlv.h"
#include "mle-route.h"




//input a route, does one already exist, returns index
int mle_route_lookup(uip_802154_shortaddr * destination){

	printf("\nin route lookup");
	printf("\nDEBUGGING: INPUT: addr[0] = %u  addr[1] = %u",destination->addr[0],destination->addr[1]);

	uint8_t i;

	for(i=0; i < MAX_MLE_ROUTES; i++){

		//if(memcmp(&destination->addr,&mle_routes[i].destination_short_address.addr,2)){
		if((destination->addr[0] == mle_routes[i].destination_short_address.addr[0]) && (destination->addr[1] == mle_routes[i].destination_short_address.addr[1])){

			return i;													//returns index
		}
	}

	printf("\nRoute does not currently exists\n");

	return MAX_MLE_ROUTES;									//a value thats too large
}

//Compare the lengths, returns TRUE if new route is smaller
int mle_route_compare(mle_route_t * mle_route, int index){

	uint8_t neighbour_path_cost;
	//will need to compare current cost with advertised + neighbour cost

	neighbour_path_cost = get_path_cost_from_destination(mle_route->next_hop_short_address);

	if((mle_route->path_cost + neighbour_path_cost) < mle_routes[index].path_cost){

		return 1;
	}
	return 0;
}

//add a route
void mle_route_add(mle_route_t * mle_route, int index){

	uint8_t neighbour_path_cost;

	printf("DEBUGGING: Adding route to index = %u\n",index);

	//if destination != neighbour
		//add idr to destination path cost

	if(!(memcmp(&mle_routes[index].destination_short_address, &mle_route->next_hop_short_address, MLE_ADDRESS_SIZE))){					//Does not work for now

		//mle_route->path_cost + neighbour cost

		neighbour_path_cost = get_path_cost_from_destination(mle_route->next_hop_short_address);

		//Adds the next hop cost to the advertised cost.
		mle_route->path_cost += neighbour_path_cost;
	}


	//Overwrite the current route

	memcpy(&mle_routes[index].destination_short_address, &mle_route->destination_short_address, MLE_ADDRESS_SIZE);
	memcpy(&mle_routes[index].next_hop_short_address, &mle_route->next_hop_short_address, MLE_ADDRESS_SIZE);

	mle_routes[index].path_cost = mle_route->path_cost;
}

//remove a route need to understand the possible reason
void mle_route_remove(){



}

//A function to get path cost from the destination address
uint8_t get_path_cost_from_destination(uip_802154_shortaddr	destination){

	uint8_t path_cost;
	uint8_t i;

	printf("\nDEBUGGING: get path cost from dest: addr[0] = %u  addr[1] = %u",destination.addr[0],destination.addr[1]);


	for(i=0; i < MAX_MLE_ROUTES; i++){

		if(memcmp(&destination.addr[0],&mle_routes[i].destination_short_address.addr[0],2)){

			printf("\nExisting path cost found\n");

			path_cost = mle_routes[i].path_cost;
			return path_cost;
		}
		else{
			i++;
		}
	}

	printf("\nRoute does not currently exists\n");

	return 0;
}





void mle_neighbour_to_route(uip_802154_shortaddr mle_neighbour_address, uint8_t link_cost){

	uint8_t index = mle_route_lookup(&mle_neighbour_address);

	mle_route_t mle_neighbour;

	memcpy(&mle_neighbour.destination_short_address, &mle_neighbour_address, MLE_ADDRESS_SIZE);

	//Next-hop address will be the same as the destination for neighbours
	memcpy(&mle_neighbour.next_hop_short_address, &mle_neighbour_address, MLE_ADDRESS_SIZE);

	//Path cost will be the same as the link cost
	mle_neighbour.path_cost = link_cost;

	if(mle_route_compare(&mle_neighbour,index)){

		mle_route_add(&mle_neighbour,index);
	}

}

//Print route table
void mle_route_table_print(void){

	uint8_t i;

	printf("\n|  Destination  |  Next Hop  |  Path  |");

	for(i=0; i < MAX_MLE_ROUTES; i++){

		printf("\n|       %u%u      |",mle_routes[i].destination_short_address.addr[0],mle_routes[i].destination_short_address.addr[1]);
		printf("     %u%u     |",mle_routes[i].next_hop_short_address.addr[0],mle_routes[i].next_hop_short_address.addr[1]);
		printf("   %u    |", mle_routes[i].path_cost);

	}


}


