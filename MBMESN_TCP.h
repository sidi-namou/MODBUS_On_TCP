#ifndef MBMESN_TCP
#define MBMESN_TCP

#include <stdint.h>

#include "MBMESN_errorstatus.h"


MBMESN_errorstatus_t MBMESN_StartTcpServer();
MBMESN_errorstatus_t MBMESN_GetClient(int32_t * clientId);
MBMESN_errorstatus_t MBMESN_CloseConnectionWithClient(int32_t clientId);
MBMESN_errorstatus_t MBMESN_tcpWriteToClient(int32_t clientId, uint8_t* dataToSend,uint32_t lengthOfData);
MBMESN_errorstatus_t MBMESN_tcpReadFromClient(int32_t clientId, uint8_t * data, uint16_t lengthOfData, uint32_t * nbBytesRead);

#endif // MBMESN_TCP
