#ifndef MBMESN_ERRORSTATUS_H_INCLUDED
#define MBMESN_ERRORSTATUS_H_INCLUDED

typedef enum
{
    MBMESN_OK = 0,
    MBMESN_FUNCTION_CODE_ERROR = 1,						// Unknown function code found in request
    MBMESN_DATA_ADDRESS_ERROR = 2,						// The data address in the query is not an allowable address for the slave
    MBMESN_QUANTITY_OF_DATA_ERROR = 3,				// Bytes count and quantity of registers do not match in request
    MBMESN_FUNCTION_PROCESSING_ERROR = 4,			// An error occured while the slave was processing the request
    MBMESN_ERROR,															// An error occured which is not covered by other status
    MBMESN_TIMEOUT,														// Timeout while waiting for incoming message
    MBMESN_SLAVE_DEVICE_ERROR,									// Response received from wrong slave (master in serial mode)
    MBMESN_CRC_ERROR,													// Error in frame check sequence in serial mode
	MBMESN_TCP_CONNECTION_CLOSED,						// The TCP connexion has been closed
} MBMESN_status_t;

typedef MBMESN_status_t MBMESN_errorstatus_t;
#endif // MBMESN_ERRORSTATUS_H_INCLUDED
