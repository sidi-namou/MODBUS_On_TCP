#include <stdio.h>
#include "MBMESN_SlavePDU.h"
#include "MBMESN_errorstatus.h"

uint16_t MB_Registers[NB_OF_MBREGISTERS];


MBMESN_status_t HandleMBRequest(uint8_t * requestPDU, uint16_t lengthOfRequestPDU, uint8_t * responsePDU, uint16_t *pLengthOfResponsePDU)
{
	uint16_t lengthOfResponseParameters = 0;
	MBMESN_status_t status = MBMESN_ERROR;

	  printf("Request received");

		switch (requestPDU[0]) {
		case 3:
			printf("Handling function 3.\n");
			status = HandleMBFunction03(requestPDU+1,responsePDU+1, &lengthOfResponseParameters);
			break;
		case 6:
			printf("Handling function 6.\n");
			status = HandleMBFunction06(requestPDU+1, responsePDU+1, &lengthOfResponseParameters);
			break;
		default:
			printf("Unknown function.\n");
			status = MBMESN_FUNCTION_CODE_ERROR;
		}
		if (status == MBMESN_OK)
		{
			printf("Request handled properly, building response.\n");
			responsePDU[0] = requestPDU[0];
			*pLengthOfResponsePDU = lengthOfResponseParameters + 1;
		}
		else
			if (status != MBMESN_ERROR)
			{
				responsePDU[0] = requestPDU[0] | 0x80;
				HandleMBError(status,responsePDU+1);
				*pLengthOfResponsePDU = 2;
			}
	return status;
}

MBMESN_status_t HandleMBFunction03(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * pLengthOfResponseParameters)
{
	uint16_t regAddress;
	uint16_t nbRegisters;
	int i = 0;

	regAddress = requestParameters[1]+(((uint16_t)requestParameters[0]) << 8);
	nbRegisters = requestParameters[3]+(((uint16_t)requestParameters[2]) << 8);
	*pLengthOfResponseParameters = 1+2*nbRegisters;
	responseParameters[0] = nbRegisters*2;

	if ((nbRegisters < 1) || (nbRegisters > 0x7D))
		return MBMESN_QUANTITY_OF_DATA_ERROR;
	else
	{
		if ((regAddress >= FIRST_MBREGISTER_ADDRESS) && (regAddress + nbRegisters <= FIRST_MBREGISTER_ADDRESS + NB_OF_MBREGISTERS))
		{
			for (i=0;i<nbRegisters;i++)
			{
				responseParameters[1+2*i] = (uint8_t) (MB_Registers[i+regAddress]>>8);
				responseParameters[1+2*i+1] = (uint8_t) (MB_Registers[i+regAddress]);
			}
			return MBMESN_OK;
		}
		else
			return MBMESN_DATA_ADDRESS_ERROR;
	}
}

MBMESN_status_t HandleMBFunction06(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * pLengthOfResponseParameters)
{
	uint16_t regAddress;
	uint16_t value;

	regAddress = requestParameters[1]+(((uint16_t)requestParameters[0]) << 8);
	value = requestParameters[3]+(((uint16_t)requestParameters[2]) << 8);
	if ((regAddress >= FIRST_MBREGISTER_ADDRESS) && (regAddress < FIRST_MBREGISTER_ADDRESS + NB_OF_MBREGISTERS))
	{
		MB_Registers[regAddress - FIRST_MBREGISTER_ADDRESS] = value;
		memcpy(responseParameters,requestParameters,4);
		*pLengthOfResponseParameters = 4;
		return MBMESN_OK;
	}
	else
		return MBMESN_DATA_ADDRESS_ERROR;
}

void HandleMBError(MBMESN_status_t status, uint8_t * responseParameters)
{
	switch (status) {
	case MBMESN_FUNCTION_CODE_ERROR:
		responseParameters[0] = 1;
		break;
	case MBMESN_DATA_ADDRESS_ERROR:
		responseParameters[0] = 2;
		break;
	case MBMESN_QUANTITY_OF_DATA_ERROR:
		responseParameters[0] = 3;
		break;
	default:
		responseParameters[0] = 4;
		break;
	}
}

