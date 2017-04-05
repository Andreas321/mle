/*
 * MLE-Commands.c
 *
 *  Created on: Feb 13, 2017
 *      Author: user
 */


#include "mle-commands.h"
#include "mle-tlv.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "simple-udp.h"
#include "mle.h"
#include "mle-route.h"

//refering to core/net/rpl/rpl.icmp6.c
/***************************************************************************************************************************************/
uint16_t link_request_out(uint8_t *buffer){
	printf("\nLINK REQUEST OUT\n");
	uint16_t length = 2;


	//Will probably call header function here
	//initial byte indicating unsecure message
	buffer[0] = 255;					//For example test

	//buffer[next byte] = LinkRequest; CommandType = 0;
	mle_command_type_t LinkRequest = 0;
	buffer[1] = LinkRequest;						//For example test, LinkRequest



	/*********TLVs***************/
	//Source Address
	//Mode
	//TimeOut
	//Challenge

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t Mode = 1;
	length += mle_tlv_write(Mode,buffer,length);

	/*if(MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE == 0){						//add when timeout tlv id finished
		mle_tlv_type_t TimeOut = 2;
		length += mle_tlv_write(TimeOut,buffer,length);
	}*/

	mle_tlv_type_t Challenge = 3;
	length += mle_tlv_write(Challenge,buffer,length);

	//Refer to rpl-icmp6.c
	//Refer to uip-icmp6.c and uip-icmp6.h
	//  tcpip_ipv6_output();


	return length;
}
/***************************************************************************************************************************************/
uint16_t link_accept_out(uint8_t *buffer,mle_tlv_t *challenge_tlv){
	printf("\nINK ACCEPT OUT");
		uint16_t length = 2;
		/*********TLVs************/
		//Source Address
		//Mode
		//TimeOut
		//Response
		//LinkLayerFrameCounter
		//MLEFrameCounter

		buffer[0] = 255;
		mle_command_type_t link_accept_and_request = 2;
		buffer[1] = link_accept_and_request;

		mle_tlv_type_t SourceAddress = 0;
		length += mle_tlv_write(SourceAddress,buffer,length);

		mle_tlv_type_t Mode = 1;
		length += mle_tlv_write(Mode,buffer,length);

		/*if(MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE == 0){						//add when timeout tlv id finished
			mle_tlv_type_t TimeOut = 2;
			length += mle_tlv_write(TimeOut,buffer,length);
		}*/

		mle_tlv_type_t Response = 4;
		length += response_function(Response,buffer,length,challenge_tlv);														//Maybe treat Response TLV differently as it requires input data

		/*
		mle_tlv_type_t LinkFrameCounter = 6;
		length += mle_tlv_write(LinkFrameCounter,buffer,length);

		mle_tlv_type_t MLEFrameCounter = 8;
		length += mle_tlv_write(MLEFrameCounter,buffer,length);

	 */
		return length;
}
/***************************************************************************************************************************************/
uint16_t link_accept_and_request_out(uint8_t *buffer,mle_tlv_t *challenge_tlv){
	printf("\nLINK ACCEPT AND REQUEST OUT");
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
	mle_command_type_t link_accept_and_request = 2;
	buffer[1] = link_accept_and_request;

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t Mode = 1;
	length += mle_tlv_write(Mode,buffer,length);

	/*if(MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE == 0){						//add when timeout tlv id finished
		mle_tlv_type_t TimeOut = 2;
		length += mle_tlv_write(TimeOut,buffer,length);
	}*/

	mle_tlv_type_t Challenge = 3;
	length += mle_tlv_write(Challenge,buffer,length);

	mle_tlv_type_t Response = 4;
	length += response_function(Response,buffer,length,challenge_tlv);														//Maybe treat Response TLV differently as it requires input data
	//printf("\nchallenge_received inside link_accept_and_request is %d",challenge_received);

	/*
	mle_tlv_type_t LinkFrameCounter = 6;
	length += mle_tlv_write(LinkFrameCounter,buffer,length);

	mle_tlv_type_t MLEFrameCounter = 8;
	length += mle_tlv_write(MLEFrameCounter,buffer,length);

 */
	return length;

}
/***************************************************************************************************************************************/
uint16_t link_reject_out(uint8_t *buffer){
	printf("\nINK REJECT OUT");
	uint16_t length = 2;
/*
 * If a large number of Link Accept messages arrive
 * If a unicast message is received from a neighbour for whom it does not have link config data for
 * 		If a large number of these arrive, the node may suspend sending link reject messages
 *
 */

	buffer[0] = 255;
	mle_command_type_t LinkReject = 4;
	buffer[1] = LinkReject;



	return length;
}
/***************************************************************************************************************************************/
uint16_t advertisement_out(uint8_t *buffer){
	printf("\nAVERTISEMENT OUT\n");

	//Will be multicasted. Therefore all nodes will have a table of every other node and who their neighbours are

	// will need some kind of timer to call this. Maybe in the mle.c file?
	//My guess of the included TLVs
	//Source Address
	//Link Quality TLV


	//Will create a list of all neighbour ip addresses and produce their IDR an I and O flags

	uint16_t length = 2;

	buffer[0] = 255;
	mle_command_type_t Advertisement = 4;
	buffer[1] = Advertisement;

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t LinkQuality = 6;
	length += mle_tlv_write(LinkQuality,buffer,length);

	//Maybe frame counter?




    return length;
}
/***************************************************************************************************************************************/
//New advertisement out with route
uint16_t route_advertisement_out(uint8_t *buffer){
	printf("\nAVERTISEMENT OUT\n");

	//Will be multicasted. Therefore all nodes will have a table of every other node and who their neighbours are

	// will need some kind of timer to call this. Maybe in the mle.c file?
	//My guess of the included TLVs
	//Source Address
	//Link Quality TLV


	//Will create a list of all neighbour ip addresses and produce their IDR an I and O flags

	uint16_t length = 2;

	buffer[0] = 255;
	mle_command_type_t Route_Advertisement = 7;
	buffer[1] = Route_Advertisement;

	mle_tlv_type_t SourceAddress = 0;
	length += mle_tlv_write(SourceAddress,buffer,length);

	mle_tlv_type_t LinkQuality = 6;
	length += mle_tlv_write(LinkQuality,buffer,length);

	mle_tlv_type_t Route = 9;
	length += mle_tlv_write(Route,buffer,length);

	//Maybe frame counter?




    return length;
}

/***************************************************************************************************************************************/
uint8_t update_out(uint8_t *buffer, uint8_t param_id, uint32_t delay, uint32_t value){				//using int at the moment as both identified values are of int type
	printf("\nUPDATE OUT\n");
	//printf("\nuint32_t delay is %lu\n",delay);
	uint8_t length = 2;
	//Can this only be sent by Leaders?

	buffer[0] = 255;																		//remember to remove this later
	//Network Parameter TLV

	mle_command_type_t Update = 5;
	buffer[1] = Update;


//	mle_tlv_type_t NetworkParameter = 0;
//	length += mle_tlv_write(NetworkParameter,buffer,length);

	mle_tlv_type_t NetworkParameter = 7;
	length += network_parameter_function(NetworkParameter,buffer, length,param_id,delay,value);

	//Multicasted to all devices in the network

	//Look at examples/extended-rf-api
	//and core/dev/radio.h


//	   MLE can also be used to distribute configuration values that are
//	   shared across a network, such as the channel and PAN ID.  Network-
//	   wide configuration uses multicasts and requires some form of multi-
//	   hop multicast forwarding.  These messages are sent infrequently, so
//	   forwarding with simple flooding is sufficient.


//	Along with the values themselves, the
//	   multicast messages include a delay value indicating when the new
//	   value takes effect.  The delay avoids having the parameters change
//	   while the multicast is still propagating.
	return length;
}
/***************************************************************************************************************************************/
void update_request_function(){
	printf("\nUPDATE REQUEST OUT");
	/*Devices that have just recently joined the network and do not yet have the currrent network values
	 *
	 * Unicast request to a neighbour
	 *
	 */

//	   In addition to network wide dissemination, a device that does not
//	   have the current network values, either because it has just joined
//	   the network or for any other reason, can send a unicast request to a
//	   neighbor.  The neighbor will respond by sending the current network
//	   values.
}
/***************************************************************************************************************************************/
//uint16_t link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer){
//
//	//what are conditions of accept/reject
//		//or are messages without security headers just ignored
//	uint16_t outputlength;
//	uint8_t challenge_received;
//	if(data[0] == 255){													//unsecure for the moment
//
//		uint8_t i = 2;													//Once again dependent on the security header
//		while(i<datalength){
//			i += new_mle_tlv_reader(data,&challenge_received,i);
//		}
//
//
//
//
//
//		//if(){
//			//"If the neighbour requires a liveness check"
//			//If considered a neighbour
//			//if R==0
//			//Add source address to the mle neighbour table
//			/*if(tlv[0]->
//			 *
//			 *
//			 */
//
//
//
//			outputlength  = link_accept_and_request_out(output_buffer,&challenge_received);
//		//}
//		//else{
//			//If Receive State == 1
//			//Add source address to the mle neighbour table
//			//link_accept_out();
//		//}
//
//	}
//	else{
//		//If too many Link Requests arrive
//		//If a message is receives a secured unicast message for which it does not have link config data
//			//If a large number of such messages arrive a device may send a Link Reject
//		outputlength = link_reject_out(output_buffer);
//	}
//
//	return outputlength;
//}
///***************************************************************************************************************************************/
uint16_t link_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer){
	printf("\nLINK REQUEST IN\n");
	//what are conditions of accept/reject
		//or are messages without security headers just ignored
	uint16_t outputlength;
	mle_tlv_t tlv[NO_OF_LINK_REQUEST_TLV];


	if(data[0] == 255){													//unsecure for the moment
		uint8_t j =0;
		uint8_t i = 2;													//Once again dependent on the security header
		while(i<datalength){
			i += get_tlv(&tlv[j],data,i);
			j++;
		}


		//Find which tlv is holding the source address
		uint8_t source_address_index = mle_tlv_search(tlv,NO_OF_LINK_REQUEST_TLV,0);


		//store source address in temporary struct
		uip_802154_shortaddr source_address;
		source_address.addr[0] = tlv[source_address_index].value[1];
		source_address.addr[1] = tlv[source_address_index].value[0];

		//Retrieves neighbour from table with that source address
		mle_neighbour_t * mle_nbr = mle_neighbour_table_lookup(source_address);

		//If neighbour not included in the mle neighbour table
		if((mle_nbr->short_address.addr) == NULL){
			//Need loop to find end of neighbours
			for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

				if(mle_neighbours[i].short_address.addr[1] == 0){//mle_neighbours[i].short_address.addr[1] == 0){

					//point to this structure
					*mle_nbr = mle_neighbours[i];

					//Store source address
					memcpy(mle_nbr->short_address.addr,source_address.addr,2);
					//Initialize iop
					mle_nbr->iop = 0;

					break;
				}

			}

		}






		uint8_t k;
		uint8_t challenge_position;
		for(k=0;k<NO_OF_LINK_REQUEST_TLV;k++){							//May need to change no_of_tlvs as it can vary

			//Search for Challenge tlv
			if(tlv[k].type == 3){
				challenge_position=k;
			}

		}
		/*
		 * Include neighbour lookup table function here
		 */



		if(!(mle_nbr->iop && RECEIVE_STATE)){																//if R==0
			//"If the neighbour requires a liveness check"???
			if(tlv[0].type == 0){
				//Store address
				memcpy(mle_nbr->short_address.addr,tlv[0].value,MLE_ADDRESS_SIZE);								//Maybe use "official" macro for short address

				mle_nbr->iop = mle_nbr->iop | TRANSMIT_STATE;											//Set T to 1
			}

			outputlength  = link_accept_and_request_out(output_buffer,&tlv[challenge_position]);
		}
		else if(mle_nbr->iop && RECEIVE_STATE){																//R == 0
			outputlength = link_accept_out(output_buffer,&tlv[challenge_position]);
		}


		////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		mle_neighbour_to_route(mle_nbr->short_address, mle_nbr->incoming_idr);

		////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////



	}
	else{
		//If too many Link Requests arrive
		//If a message is receives a secured unicast message for which it does not have link config data
			//If a large number of such messages arrive a device may send a Link Reject
		outputlength = link_reject_out(output_buffer);
	}

	return outputlength;
}
/***************************************************************************************************************************************/
uint16_t link_accept_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer){
	printf("\nLINK ACCEPT IN");
	//Set Recieve State to true for this neighbour
	//Reject if response is false
		//May require declaring challenge globally


	uint16_t outputlength;
	mle_tlv_t tlv[NO_OF_LINK_ACCEPT_TLV];



	if(data[0] == 255){													//unsecure for the moment
		uint8_t j =0;
		uint8_t i = 2;													//Once again dependent on the security header
		while(i<datalength){
			i += get_tlv(&tlv[j],data,i);
			j++;
		}





		//Find which tlv is holding the source address
		uint8_t source_address_index = mle_tlv_search(tlv,NO_OF_LINK_REQUEST_TLV,0);


		//store source address in temporary struct
		uip_802154_shortaddr source_address;
		source_address.addr[0] = tlv[source_address_index].value[1];
		source_address.addr[1] = tlv[source_address_index].value[0];

		//Retrieves neighbour from table with that source address
		mle_neighbour_t * mle_nbr = mle_neighbour_table_lookup(source_address);



		//This part will probably need to be removed from here
		//If neighbour not included in the mle neighbour table
		if((mle_nbr->short_address.addr) == NULL){
			//Need loop to find end of neighbours
			for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

				if(mle_neighbours[i].short_address.addr[1] == 0){//mle_neighbours[i].short_address.addr == NULL){
					//Store source address
					memcpy(mle_neighbours[i].short_address.addr,source_address.addr,2);
					//Initialize iop
					mle_neighbours[i].iop = 0;

					break;
				}
				else{
					printf("\nMLE NEIGHBOUR TABLE IS FULL\n");
				}
			}

		}



		//A link accept has been received therefore receive state can be set to 1
		mle_nbr->iop = mle_nbr->iop	| RECEIVE_STATE;





		uint8_t k;
		uint8_t response_position;
		for(k=0;k<NO_OF_LINK_ACCEPT_TLV;k++){
			//Search for Response tlv
			if(tlv[k].type == 4){
				response_position=k;
			}

		}

		//IF the response is correct, set T to 1?
		//What happens if receive state is false?
			//Do we send Link Reject?

//		MLE uses a two-message challenge and response protocol to
//		   ensure that the MLE message containing a neighbor's frame counter is
//		   not itself a replayed message.



	}
	else{
		//If response is incorrect, do we send out a link reject?
		outputlength = link_reject_out(output_buffer);
	}

	return outputlength;
}
/***************************************************************************************************************************************/
uint16_t link_accept_and_request_in(const uint8_t * data, uint16_t datalength,uint8_t * output_buffer){
	printf("\nLINK ACCEPT AND REQUEST IN\n");
		uint16_t outputlength;
		//uint8_t challenge_received[4];										//4 FOR NOW, BUT WILL DEPEND ON NETWORK PARAM, MAKE MACRO
		mle_tlv_t tlv[NO_OF_LINK_ACCEPT_AND_REQUEST_TLV];




		//Replace this if with a different condition for accept/reject
		if(data[0] == 255){													//unsecure for the moment
			uint8_t j =0;
			uint8_t i = 2;													//Once again dependent on the security header
			while(i<datalength){
				i += get_tlv(&tlv[j],data,i);
				j++;
			}


			//Find which tlv is holding the source address
			uint8_t source_address_index = mle_tlv_search(tlv,NO_OF_LINK_ACCEPT_AND_REQUEST_TLV,0);


			//store source address in temporary struct
			uip_802154_shortaddr source_address;
			source_address.addr[0] = tlv[source_address_index].value[1];
			source_address.addr[1] = tlv[source_address_index].value[0];

			//Retrieves neighbour from table with that source address
			mle_neighbour_t * mle_nbr = mle_neighbour_table_lookup(source_address);

			//If neighbour not included in the mle neighbour table
			if((mle_nbr->short_address.addr) == NULL){
				//Need loop to find end of neighbours
				for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

					if(mle_neighbours[i].short_address.addr[1] == 0){//mle_neighbours[i].short_address.addr == NULL){
						//Store source address
						memcpy(mle_neighbours[i].short_address.addr,source_address.addr,2);
						//Initialize iop
						mle_neighbours[i].iop = 0;
						break;
					}

				}

			}


			//A link accept has been received therefore receive state can be set to 1
			mle_nbr->iop = mle_nbr->iop	| RECEIVE_STATE;




			uint8_t k;
			uint8_t challenge_position;
			for(k=0;k<NO_OF_LINK_ACCEPT_AND_REQUEST_TLV;k++){							//May need to change no_of_tlvs as it can vary
				//Search for Challenge tlv
				if(tlv[k].type == 3){
					challenge_position=k;
				}

			}


			mle_nbr->iop = mle_nbr->iop | TRANSMIT_STATE;								//Set T to 1

			////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////

			mle_neighbour_to_route(mle_nbr->short_address, mle_nbr->incoming_idr);

			////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////

			outputlength  = link_accept_out(output_buffer,&tlv[challenge_position]);

		}
		else{
			//If too many Link Requests arrive
			//If a message is receives a secured unicast message for which it does not have link config data
				//If a large number of such messages arrive a device may send a Link Reject
			//Reject if response is false
				//May require declaring challenge globally
			outputlength = link_reject_out(output_buffer);
		}

		return outputlength;
}
/***************************************************************************************************************************************/
void link_reject_in(){
	printf("\nLINK REJECT IN");

	mle_neighbour_t mle_neighbour;
	mle_neighbour.iop = 0;
	mle_neighbour.iop = mle_neighbour.iop	^ RECEIVE_STATE;					//XOR Receive state to turn it off.


}
/***************************************************************************************************************************************/
uint16_t advertisement_in(uint8_t *data, uint16_t datalength){
	printf("\nAVERTISEMENT IN");
	uint16_t outputbuf_length = 10;				//Randomly set to 10 for now

	//maybe call mle_tlv_reader?

	//maybe call advertisement_handler?

	return outputbuf_length;
}
/***************************************************************************************************************************************/
void advertisement_handler(uint8_t *data, uint16_t datalength){
	printf("\nAVERTISEMENT HANDLER");

	/* If a link quality TLV is received from a node but the link is not configured to the neighbour
	 * A node that does not have a link configured to a neighbor but receives a Link Quality TLV from that neighbor with the node’s O flag
	 * set to "1" SHOULD send an MLE message with a Link Quality TLV with that neighbor’s I bit set to "0". This message may either be a
	 * regular multicast Advertisement or a unicast to that neighbor containing only a single Neighbor Data record.
	 */


	mle_tlv_t tlv[NO_OF_ADVERTISEMENT_TLV];


		uint8_t j =0;
		uint8_t i = 2;													//Once again dependent on the security header
		while(i<datalength){
			i += get_tlv(&tlv[j],data,i);
			j++;
		}




		//Find which tlv is holding the source address
		uint8_t source_address_index = mle_tlv_search(tlv,NO_OF_ADVERTISEMENT_TLV,0);


		//store source address in temporary struct
		uip_802154_shortaddr source_address;
		source_address.addr[0] = tlv[source_address_index].value[1];
		source_address.addr[1] = tlv[source_address_index].value[0];

		//Retrieves neighbour from table with that source address
		mle_neighbour_t * mle_nbr = mle_neighbour_table_lookup(source_address);

		//If neighbour not included in the mle neighbour table
		if((mle_nbr->short_address.addr) == NULL){
			//Need loop to find end of neighbours
			for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

				if(mle_neighbours[i].short_address.addr[1] == 0){

					//point to this structure
					*mle_nbr = mle_neighbours[i];

					//Store source address
					memcpy(mle_nbr->short_address.addr,source_address.addr,2);
					//Initialize iop
					mle_nbr->iop = 0;

					printf("\nNode with short address %u%u added to mle neighbour table\n",mle_nbr->short_address.addr[0],mle_nbr->short_address.addr[1]);

					break;
				}
			}

		}




	//Find which tlv is holding the source address
	uint8_t link_quality_index = mle_tlv_search(tlv,NO_OF_ADVERTISEMENT_TLV,6);

	printf("\n index is %u",link_quality_index);


	link_quality_reader(&tlv[link_quality_index]);






//	//Transmit State is set to the Recieve state as stated in the advertisement
//	//If C = 1 but the receiving node's address is not included in the advertisement, the recipients Transmit State for the sender is set to false
//	uint8_t link_quality_position;
//	//use new_mle_tlv_reader to retrieve position of LinkQuality TLV
//
//	uint8_t c = data[link_quality_position];
//	uint8_t address_size = 2;
//	uint8_t i;
//	uint8_t data_per_neighbour;
//
//	uint8_t receive_state;						//If advertiser is able to receive from that neighbour this is True
//	uint8_t transmit_state;						//If advertiser is able to transmit to that neighbour this is True
//	uint8_t priority;
//
//	if(c & 0x80){								//C=1
//
//		address_size = c - 0x80;
//
//	}
//	else{
//
//		address_size = c;
//	}
//
//	data_per_neighbour = address_size + 2;
//
//	for(i=0;i<datalength;i+=data_per_neighbour){
//		receive_state = data[i] & 0x80;
//		transmit_state = data[i] & 0x40;
//		priority = data[i] & 0x20;
//	}


}
/***************************************************************************************************************************************/
//New advertisement handler with route tlv
void route_advertisement_handler(uint8_t *data, uint16_t datalength){
	printf("\nAVERTISEMENT HANDLER");

	/* If a link quality TLV is received from a node but the link is not configured to the neighbour
	 * A node that does not have a link configured to a neighbor but receives a Link Quality TLV from that neighbor with the node’s O flag
	 * set to "1" SHOULD send an MLE message with a Link Quality TLV with that neighbor’s I bit set to "0". This message may either be a
	 * regular multicast Advertisement or a unicast to that neighbor containing only a single Neighbor Data record.
	 */


	mle_tlv_t tlv[NO_OF_ROUTE_ADVERTISEMENT_TLV];


		uint8_t j =0;
		uint8_t i = 2;													//Once again dependent on the security header
		while(i<datalength){
			i += get_tlv(&tlv[j],data,i);
			j++;
		}




		//Find which tlv is holding the source address
		uint8_t source_address_index = mle_tlv_search(tlv,NO_OF_ROUTE_ADVERTISEMENT_TLV,0);
		//store source address in temporary struct
		uip_802154_shortaddr source_address;
		source_address.addr[0] = tlv[source_address_index].value[1];
		source_address.addr[1] = tlv[source_address_index].value[0];

		//Retrieves neighbour from table with that source address
		mle_neighbour_t * mle_nbr = mle_neighbour_table_lookup(source_address);

		//If neighbour not included in the mle neighbour table
		if((mle_nbr->short_address.addr) == NULL){
			//Need loop to find end of neighbours
			for(i=0; i < NBR_TABLE_MAX_NEIGHBORS; i++){

				if(mle_neighbours[i].short_address.addr[1] == 0){

					//point to this structure
					*mle_nbr = mle_neighbours[i];

					//Store source address
					memcpy(mle_nbr->short_address.addr,source_address.addr,2);
					//Initialize iop
					mle_nbr->iop = 0;

					printf("\nNode with short address %u%u added to mle neighbour table\n",mle_nbr->short_address.addr[0],mle_nbr->short_address.addr[1]);

					break;
				}
			}

		}




	//Find which tlv is holding the link quality
	uint8_t link_quality_index = mle_tlv_search(tlv,NO_OF_ROUTE_ADVERTISEMENT_TLV,6);

	printf("\nLink Quality index is %u",link_quality_index);


	link_quality_reader(&tlv[link_quality_index]);


	//Find which tlv is holding the mle route tlv
	uint8_t mle_route_index = mle_tlv_search(tlv,NO_OF_ROUTE_ADVERTISEMENT_TLV,9);

	printf("\nMLE route index is %u",mle_route_index);


	mle_route_reader(&tlv[mle_route_index],source_address);




}

/***************************************************************************************************************************************/
//returns an int value for the radio paramter
int update_in(const uint8_t *data, uint16_t datalength, uint32_t * interval, uint32_t * parameter_value){

	mle_tlv_t tlv[NO_OF_UPDATE_TLV];									//This method allows future version of mle draft to include more tlvs in update messages
	uint8_t network_param_index;

		//Security 255 not included here

		uint8_t j =0;
		uint8_t i = 2;													//Once again dependent on the security header
		while(i<datalength){
			i += get_tlv(&tlv[j],data,i);
			j++;
		}

		uint8_t network_parameter = 7;

		//uint8_t tlv_size = (sizeof(tlv)/sizeof(*tlv));

		network_param_index = mle_tlv_search(tlv, NO_OF_UPDATE_TLV,network_parameter);

		int this_will_return;
		//DELAY TIME will be extracted from the network param tlv
		//identify value by reading ID, then set the param using the value					//May be in network byte order

		this_will_return = network_param_reader(&tlv[network_param_index],interval,parameter_value);


		//printf("\nDEBUGGING IN UPDATE IN: interval is %lu, and mle_network parameter value is %lu\n",*interval,*parameter_value);
		//printf("\nDEBUGGING IN UPDATE IN: interval is %p, and mle_network parameter value is %p\n",interval,parameter_value);
		return this_will_return;
		//NEed to also return actualy parameter value

}
