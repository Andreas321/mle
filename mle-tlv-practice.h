/*
 * MLE-TLV-Practice.h
 *
 *  Created on: Feb 10, 2017
 *      Author: user
 */

#ifndef MLE_TLV_PRACTICE_H_
#define MLE_TLV_PRACTICE_H_






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

};
typedef uint8_t MLE_TLV_Type;												//double check this syntax.  Does this make the previous enum use values with data size "uint8_t"?


typedef struct MLE_TLV
{
	MLE_TLV_Type type;				//using previously defined data type
	uint8_t length;					//will contain the length of the value in bytes
	uint8_t *value;					//THIS NEEDS TO CHANGE!
} MLE_tlv_t;


MLE_TLV_getType(/*input*/){				//will be referenced from somewhere handling the command types, UDP port 521/520, need to decide on the type of the input buffer maybe check UDP files?

	MLE_tlv_t tlv;


	tlv->type = input[0];				//will this require conversion to uint8_t?


	return tlv->type;
}


MLE_TLV_getValue(/*input*/){


	MLE_tlv_t tlv;
	uint8_t i;
	uint8_t value;								//NEED TO DECIDE SIZE

	tlv->length = input[1];

	for(i = 2; i < length ; i++){				//can int and uint8_t be used in the same for loop condition?  Do i need to convert length to int?
		//length should be 4 in this case
		value = (value << 8) | input[i];		//clever! Shift 8 bits then apply the input.
	}


	return tlv->value;
}




#endif /* MLE_TLV_PRACTICE_H_ */
