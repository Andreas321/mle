/*
 * mle-route.h
 *
 *  Created on: Apr 3, 2017
 *      Author: user
 */

#ifndef MLE_ROUTE_H_
#define MLE_ROUTE_H_

#define MAX_MLE_ROUTES			15								//Random number for now, maybe check uip_ds6_route.c for 'official'

#define MLE_ROUTE_DATA_SIZE 	3

typedef struct mle_route{

	uip_802154_shortaddr 	destination_short_address;
	uip_802154_shortaddr 	next_hop_short_address;
	uint8_t 				path_cost;

}mle_route_t;


mle_route_t mle_routes[MAX_MLE_ROUTES];



//input a route, does one already exist
int mle_route_lookup(uip_802154_shortaddr * destination);

//Compare the lengths, returns TRUE if new route is smaller
int mle_route_compare(mle_route_t * mle_route, int index);

//add a route
void mle_route_add(mle_route_t * mle_route, int index);

//remove a route need to understand the possible reason
void mle_route_remove();

uint8_t get_path_cost_from_destination(uip_802154_shortaddr	destination);
//Add neighbour to route table

void mle_neighbour_to_route(uip_802154_shortaddr neighbour, uint8_t link_cost);


void mle_route_table_print(void);

#endif /* MLE_ROUTE_H_ */
