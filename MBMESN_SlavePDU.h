#ifndef MBMESN_SLAVEPDU
#define MBMESN_SLAVEPDU

#include <stdint.h>
#include <string.h>
#include "MBMESN_errorstatus.h"

#define NB_OF_MBREGISTERS 8
#define FIRST_MBREGISTER_ADDRESS 0

MBMESN_status_t HandleMBRequest(uint8_t * requestPDU, uint16_t lengtOfRequestPDU, uint8_t * responsePDU, uint16_t *pLengthOfResponsePDU);
MBMESN_status_t HandleMBFunction03(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * pNbOfRegisters);
MBMESN_status_t HandleMBFunction06(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * lengthOfResponsParameters);
void HandleMBError(MBMESN_status_t status, uint8_t * responseParameters);

#endif /* MBMESN_SLAVEPDU */
