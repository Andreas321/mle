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

typedef union mle_tlv_value{

	uint8_t		u8;
	uint16_t	u16;
	uint32_t	u32;
	uint64_t	u64;

}mle_tlv_value_t;

typedef struct mle_tlv
{
	mle_tlv_type_t type;				//using previously defined data type
	uint8_t length;					//will contain the length of the value in bytes
	//uint32_t value;					//THIS NEEDS TO CHANGE SIZE , why does oma have this as a pointer? is it to point to a certain part of input buffer to determine size? Possibly void *?
	//or uint8_t *value
	//mle_tlv_value_t value;				//uses the union
	uint8_t *value;
} mle_tlv_t;




//function for parsing input to tlv format
void mle_tlv_reader(mle_tlv_t *tlv, uint8_t *input);

uint8_t mle_tlv_write(mle_tlv_type_t type, uint8_t *parsedtlv);

void mle_tlv_parser(mle_tlv_t * tlv, uint8_t tbp[]);

void source_address_function(uint8_t * value);
uint8_t mode_function();
uint32_t timeout_function();
uint32_t challenge_function();
uint32_t response_function();
uint32_t linklayer_frame_counter_function();
void link_quality_function();
void parameter_function();
uint32_t mle_frame_counter_function();


enum{								//will probably need to change this since enum is int type not uint8_t
	Channel				= 0,
	PANID				= 1,
	PermitJoining		= 2,
	BeaconPayload		= 3,

};
typedef uint8_t network_parameter_id_t;

#endif /* MLE_TLV_H_ */
