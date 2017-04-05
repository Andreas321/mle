/*
 * MLE-TLV.h
 *
 *  Created on: Jan 29, 2017
 *      Author: user
 */

#ifndef MLE_TLV_H_
#define MLE_TLV_H_

#include <stdint.h>
#include <stdio.h>
#include "core/net/nbr-table.h"
#include "nbr-table.h"
/*---------------------------------------------------------------------------*/
/*
 * Firstly, you allow users to configure their device.
 *
 * Starting with the mains/battery power
 */
//Alternate PAN Coordinator
#ifdef MLE_CONF_DEVICE_CONFIG_ALTERNATE_PAN_COORDINATOR
#define MLE_DEVICE_CONFIG_ALTERNATE_PAN_COORDINATOR MLE_CONF_DEVICE_CONFIG_ALTERNATE_PAN_COORDINATOR
#else
#define MLE_DEVICE_CONFIG_ALTERNATE_PAN_COORDINATOR 0
#endif

//Device Type
#ifdef MLE_CONF_DEVICE_CONFIG_DEVICE_TYPE
#define MLE_DEVICE_CONFIG_DEVICE_TYPE MLE_CONF_DEVICE_CONFIG_DEVICE_TYPE
#else
#define MLE_DEVICE_CONFIG_DEVICE_TYPE 0
#endif

#ifdef MLE_CONF_DEVICE_CONFIG_MAINS_POWERED
#define MLE_DEVICE_CONFIG_MAINS_POWERED MLE_CONF_DEVICE_CONFIG_MAINS_POWERED
#else
#define MLE_DEVICE_CONFIG_MAINS_POWERED 0
#endif

/* Ditto for RF state when idle */
#ifdef MLE_CONF_DEVICE_CONFIG_RF_ON_WHEN_IDLE
#define MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE MLE_CONF_DEVICE_CONFIG_RF_ON_WHEN_IDLE
#else
#define MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE 0
#endif

//Security Capability
#ifdef MLE_CONF_DEVICE_CONFIG_SECURITY_CAPABILITY
#define MLE_DEVICE_CONFIG_SECURITY_CAPABILITY MLE_CONF_DEVICE_CONFIG_SECURITY_CAPABILITY
#else
#define MLE_DEVICE_CONFIG_SECURITY_CAPABILITY 0
#endif

//Allocate Address
#ifdef MLE_CONF_DEVICE_CONFIG_ALLOCATE_ADDRESS
#define MLE_DEVICE_CONFIG_ALLOCATE_ADDRESS MLE_CONF_DEVICE_CONFIG_ALLOCATE_ADDRESS
#else
#define MLE_DEVICE_CONFIG_ALLOCATE_ADDRESS 0
#endif

/*---------------------------------------------------------------------------*/
/* Define correct bit values for all fields */


//Backwards for network byte order?
//think of these as constants
//PAN Coordinator bit 0					128
#define MODE_TLV_PAN_COORDINATOR		0X80
#define MODE_TLV_NOT_PAN_COORDINATOR	0X00
//DEVICE TYPE: RFD or FFD bit 1			0x64
#define MODE_TLV_RFD	0X40
#define MODE_TLV_FFD	0x00
//POWER SOURCE bit 2					0x32
#define MODE_TLV_POWER_MAINS 0x20
#define MODE_TLV_POWER_BATT  0x00
//RECEIVE ON WHEN IDLE bit 3			0x16
#define MODE_TLV_IDLE_RF_ON  0x10
#define MODE_TLV_IDLE_RF_OFF 0x00
//SECURITY CAPABILITY bit 6				0x02
#define MODE_TLV_SECURITY_CAPABILITY		0X02
#define MODE_TLV_NO_SECURITY_CAPABILITY		0X00
//ALLOCATE ADDRESS bit 7				0x01
#define MODE_TLV_ALLOCATE_ADDRESS			0X01
#define MODE_TLV_DONT_ALLOCATE_ADDRESS		0X00

/* Based on configuration and bit values, configure the value of the TLV */
#if MLE_DEVICE_CONFIG_ALTERNATE_PAN_COORDINATOR
#define PAN_COORDINATOR_BIT MODE_TLV_PAN_COORDINATOR
#else
#define PAN_COORDINATOR_BIT MODE_TLV_NOT_PAN_COORDINATOR
#endif

#if MLE_DEVICE_CONFIG_DEVICE_TYPE
#define DEVICE_TYPE_BIT MODE_TLV_RFD
#else
#define DEVICE_TYPE_BIT MODE_TLV_FFD
#endif

#if MLE_DEVICE_CONFIG_MAINS_POWERED
#define POWER_BIT MODE_TLV_POWER_MAINS
#else
#define POWER_BIT MODE_TLV_POWER_BATT
#endif

#if MLE_DEVICE_CONFIG_RF_ON_WHEN_IDLE
#define IDLE_RF_ON_BIT MODE_TLV_IDLE_RF_ON
#else
#define IDLE_RF_ON_BIT MODE_TLV_IDLE_RF_OFF
#endif

#if MLE_DEVICE_CONFIG_SECURITY_CAPABILITY
#define SECURITY_BIT MODE_TLV_SECURITY_CAPABILITY
#else
#define SECURITY_BIT MODE_TLV_NO_SECURITY_CAPABILITY
#endif

#if MLE_DEVICE_CONFIG_ALLOCATE_ADDRESS
#define ALLOCATE_ADDRESS_BIT MODE_TLV_ALLOCATE_ADDRESS
#else
#define ALLOCATE_ADDRESS_BIT MODE_TLV_DONT_ALLOCATE_ADDRESS
#endif
/* Then once you have all information you need, set the correct value for the TLV */
#define DEVICE_CONFIG (PAN_COORDINATOR_BIT | DEVICE_TYPE_BIT | POWER_BIT | IDLE_RF_ON_BIT | SECURITY_BIT | ALLOCATE_ADDRESS_BIT)
/*--------------------------------------------------------------------------------------------------------------------------------------------*/

#define MLE_ADDRESS_SIZE 2																						//MLE should actually be able to support addresses of up to 16 bytes.
#define MLE_LINK_QUALITY_NEIGHBOUR_DATA_SIZE (0x02 + MLE_ADDRESS_SIZE)


//if defined. maximum number of neighbours(find this in z1 conf file
//#ifdef NBR_TABLE_CONF_MAX_NEIGHBORS																			//no need to use this line as is already defined with conditional in nbr-table.h
#define MAX_VALUE_SIZE (0x05 + (NBR_TABLE_MAX_NEIGHBORS * MLE_LINK_QUALITY_NEIGHBOUR_DATA_SIZE))					//Link Quality is the tlv with the largest possible size
//Maybe change this to 255 or something for route tlv

/*
		Values are encoded using a type-length-value format, where the type
		and length are one byte each and the length field contains the length
		of the value in bytes. TLVs are stored serially with no padding
		between them. They are byte-aligned but are not aligned in any other
		way such as on 2 or 4 byte boundaries. All values in TLVs are in
		network byte order.

		0 					1					2					3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|   Type       |    Length     |    Value ...
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		Type 			An eight-bit unsigned integer giving the type of
						the value, from IANA registry Section 14.3.

		Length 			An eight-bit unsigned integer giving the length
						of the Value field in bytes.

		Value 			Length bytes of value, formatted as defined for
						the Type.

		With the exceptions of the Source Address TLV and Parameter TLV, an
		MLE message MUST NOT contain two or more TLVs of the same type. To
		allow devices to have multiple source addresses, an MLE message MAY
		contain two or more Source Address TLVs.
 */



/*
//TYPES
enum
{
		SourceAddress       = 0,    ///< Source Address TLV
        Mode                = 1,    ///< Mode TLV
        Timeout             = 2,    ///< Timeout TLV
        Challenge           = 3,    ///< Challenge TLV
        Response            = 4,    ///< Response TLV
        LinkFrameCounter    = 5,    ///< Link-Layer Frame Counter TLV
        LinkQuality         = 6,    ///< Link Quality TLV
        NetworkParameter    = 7,    ///< Network Parameter TLV
        MleFrameCounter     = 8,    ///< MLE Frame Counter TLV

};*/
typedef uint8_t mle_tlv_type_t;												//double check this syntax.  Does this make the previous enum use values with data size "uint8_t"?


/*
const MLE_TLV_Type SourceAddress       = 0;    ///< Source Address TLV
const MLE_TLV_Type Mode                = 1;    ///< Mode TLV
const MLE_TLV_Type Timeout             = 2;    ///< Timeout TLV
const MLE_TLV_Type Challenge           = 3;    ///< Challenge TLV
const MLE_TLV_Type Response            = 4;    ///< Response TLV
const MLE_TLV_Type LinkFrameCounter    = 5;    ///< Link-Layer Frame Counter TLV
const MLE_TLV_Type LinkQuality         = 6;    ///< Link Quality TLV
const MLE_TLV_Type NetworkParameter    = 7;    ///< Network Parameter TLV
const MLE_TLV_Type MleFrameCounter     = 8;    ///< MLE Frame Counter TLV
*/
/*
MLE_TLV_Type SourceAddress       = 0;    ///< Source Address TLV
MLE_TLV_Type Mode                = 1;    ///< Mode TLV
MLE_TLV_Type Timeout             = 2;    ///< Timeout TLV
MLE_TLV_Type Challenge           = 3;    ///< Challenge TLV
MLE_TLV_Type Response            = 4;    ///< Response TLV
MLE_TLV_Type LinkFrameCounter    = 5;    ///< Link-Layer Frame Counter TLV
MLE_TLV_Type LinkQuality         = 6;    ///< Link Quality TLV
MLE_TLV_Type NetworkParameter    = 7;    ///< Network Parameter TLV
MLE_TLV_Type MleFrameCounter     = 8;    ///< MLE Frame Counter TLV
*/
//
//typedef union mle_tlv_value{
//
//	uint8_t		u8;
//	uint16_t	u16;
//	uint32_t	u32;
//	uint64_t	u64;
//
//}mle_tlv_value_t;

typedef struct mle_tlv
{
	mle_tlv_type_t type;				//using previously defined data type
	uint8_t length;					//will contain the length of the value in bytes
	//mle_tlv_value_t value;				//uses the union
	//uint8_t *value;
	//uint8_t value[30];
	uint8_t value[MAX_VALUE_SIZE];		//eventually convert to this
	//uint8_t value[15];
} mle_tlv_t;

//


//These are the values for MLE
//Howevever in Contiki RADIO_PARAM_CHANNEL =1
//and RADIO_PARAM_PAN_ID =2
//Therefore will need a "+1" at receiver
//Alternatively if(0) ->  RADIO_PARAM_CHANNEL  !
#define MLE_NETWORK_PARAM_ID_CHANNEL			0x00
#define MLE_NETWORK_PARAM_ID_PAN_ID				0x01
#define MLE_NETWORK_PARAM_ID_PERMIT_JOINING		0x02
#define MLE_NETWORK_PARAM_ID_BEACON_PAYLOAD		0x03






typedef struct mle_network_parameters{

	uint8_t 	parameter_id;
	uint32_t 	delay;
	uint8_t		*value;									//string of this will be determined by particular param

}mle_network_parameter_t;




//function for parsing input to tlv format
void mle_tlv_reader(mle_tlv_t *tlv, uint8_t *input);

uint8_t get_tlv(mle_tlv_t *tlv,const uint8_t * input_data,uint8_t starting_position);

/////////////////////////////////////////////////////////////////////////////////////////
uint8_t new_mle_tlv_reader(uint8_t * input_data, uint8_t * challenge, uint8_t starting_position);
//////////////////////////////////////////////////////////////////////////


uint8_t mle_tlv_write(mle_tlv_type_t type, uint8_t *parsedtlv, uint8_t startingposition);

void mle_tlv_parser(mle_tlv_t * tlv, uint8_t tbp[], uint8_t position);

uint8_t source_address_function(uint8_t * value);
uint8_t mode_function(uint8_t * value);


uint8_t timeout_function(uint8_t * value);
uint8_t challenge_function(uint8_t * value);
uint8_t response_function(mle_tlv_type_t type, uint8_t *parsedtlv, uint8_t startingposition, mle_tlv_t challenge_tlv[]);

void linklayer_frame_counter_function(uint8_t * value);
uint8_t new_link_quality_function(uint8_t * value);
uint8_t link_quality_function(uint8_t * value);

uint8_t network_parameter_function(mle_tlv_type_t network_parameter, uint8_t *parsedtlv, uint8_t startingposition, uint8_t param_id, uint32_t delay, uint32_t value);

void mle_frame_counter_function(uint8_t * value);

uint8_t mle_tlv_search(mle_tlv_t tlv[], uint8_t no_of_tlv,  mle_tlv_type_t type);

//Reads tlv and returns values as pointers to the delay and radio parameter
int network_param_reader(mle_tlv_t * tlv, uint32_t * delay, uint32_t * pvalue);


void link_quality_reader(mle_tlv_t * tlv);


uint8_t mle_route_function(uint8_t * value);

void mle_route_reader(mle_tlv_t * tlv, uip_802154_shortaddr source_address);
//enum{								//will probably need to change this since enum is int type not uint8_t
//	Channel				= 0,
//	PANID				= 1,
//	PermitJoining		= 2,
//	BeaconPayload		= 3,
//
//};
//typedef uint8_t network_parameter_id_t;

#endif /* MLE_TLV_H_ */
