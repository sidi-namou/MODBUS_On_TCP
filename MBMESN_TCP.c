#include <stdio.h>      /** for printf() and fprintf() */
#include <winsock.h> /** for recv() and send() */
#include "MBMESN_TCP.h"
#include <stdio.h>  /** for perror() */
#include <stdlib.h> /** for exit() */



#define MAXPENDING 1    /** Maximum outstanding connection requests */
#define echoServPort 502     /** Server port by default */


    static int servSock;                    /** Socket descriptor for server */

    static struct sockaddr_in echoServAddr; /** Local address */
    static struct sockaddr_in echoClntAddr; /** Client address */

    static unsigned int clntLen;            /** Length of client address data structure */
    static WSADATA wsaData;                 /** Structure for WinSock setup communication */



    static void DieWithError(char *errorMessage);


    /**
This function is used to start TCP Server on Port 502 which correspond to MODBUS
This function accept no arguments
returns value -->
MBMESN_TCP_CONNECTION_CLOSED when the TCP Server encountered a problem
MBMESN_OK when the TCP Server started successfully
*/

MBMESN_errorstatus_t MBMESN_StartTcpServer(){


    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /** Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    /** Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
         DieWithError("socket() failed");
         return  MBMESN_ERROR;
    }


    /** Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /** Zero out structure */
    echoServAddr.sin_family = AF_INET;                /** Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /** Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /** Local port */

    /** Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
        DieWithError("bind() failed");
        return  MBMESN_ERROR;
    }


    /** Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0){
         DieWithError("listen() failed");
          return  MBMESN_ERROR;
    }


        return MBMESN_OK;



}

/**
    This function is used to wait for a client to connect and attribute an identifier to this client
    It needs a reference to an int32_t variable in order to store the identifier of this client


**/


MBMESN_errorstatus_t MBMESN_GetClient(int32_t * clientId){

        /** Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /** Wait for a client to connect */
        if ((*clientId = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0){
           DieWithError("accept() failed");
           return  MBMESN_ERROR;
        }


        /** clntId is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        return  MBMESN_OK;


}

/**
 This function is used to close connection with client
 arguments --> clientId,
 returns value -->
 MBMESN_TCP_CONNECTION_CLOSED when client socket is closed
 MBMESN_ERROR otherwise.
**/

MBMESN_errorstatus_t MBMESN_CloseConnectionWithClient(int32_t clientId){

                    if(closesocket(clientId) == 0)    return MBMESN_TCP_CONNECTION_CLOSED;

                    else DieWithError("close() failed");

                    return MBMESN_ERROR;
}

/**
    This function is used to send data to client identified by clientId
    Arguments--> int32_t clientId
    uint8_t * dataToSend a pointer to data
    uint32_t lengthOfData.

    Return Value :

    MBMESN_OK    otherwise.
*/


MBMESN_errorstatus_t MBMESN_tcpWriteToClient(int32_t clientId, uint8_t* dataToSend,uint32_t lengthOfData){

             /** Echo message to client **/

             if (send(clientId, dataToSend, lengthOfData, 0) != lengthOfData)DieWithError("send() failed");

              return   MBMESN_OK;





}

/**
    This function is used to receive data from client identified by clientId
    Arguments--> int32_t clientId
    uint8_t * data a pointer to data
    uint32_t lengthOfData.
    uint32_t * nbBytesRead, write here number of bytes send by the client
    Return Value :

    MBMESN_OK    otherwise.
*/

MBMESN_errorstatus_t MBMESN_tcpReadFromClient(int32_t clientId, uint8_t * data, uint16_t lengthOfData, uint32_t * nbBytesRead){



                               /** Receive message from client */

                                if ((*nbBytesRead = recv(clientId, data, lengthOfData, 0)) < 0)
                                    DieWithError("recv() failed");

                                return MBMESN_OK;


}

/**
This function is used to print message error in the console and exit the program.
It requires only a string of error message.
It returns no argument
**/
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}


