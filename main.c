#include <stdio.h>
#include <stdlib.h>
#include "MBMESN_TCP.h"
#include "MBMESN_SlavePDU.h"
#define RCVBUFSIZE 300

int main()
{
    int clientId;
    uint8_t  data[RCVBUFSIZE];
    uint32_t  nbBytesRead;
    uint32_t  nbBytesToSend;
    uint8_t dataToSend[RCVBUFSIZE];
    uint8_t mbap_Header[7];
    uint16_t length;                    /**Length of PDU*/
    uint16_t lengthOfData=0;
    uint16_t lengthOfDataToSend=0;





    /** start TCP Server */
    MBMESN_StartTcpServer();

    /** An infinite loop for run forever **/
    for(;;){

                            /**Get client id*/

                            MBMESN_GetClient(&clientId);


                            /** Read from client **/

        MBMESN_tcpReadFromClient(clientId, data , RCVBUFSIZE, &nbBytesRead);
        lengthOfData += nbBytesRead;


        while(nbBytesRead > 0){
                /**Initialize length of data to send to 0 for each respond to client*/
                lengthOfDataToSend=0;


                /**A loop while to get at least 7 bytes */
                while(lengthOfData < 7){
                  MBMESN_tcpReadFromClient(clientId, data + lengthOfData , RCVBUFSIZE, &nbBytesRead);
                  lengthOfData += nbBytesRead;
                }

                /** Extract the MBAP Header */
                for(int i=0;i<7;i++)mbap_Header[i]=data[i];

                /**Convert length of data*/

                length = 256*mbap_Header[4] + mbap_Header[5];

                /** Add 6 bytes of the first MBAP Header to simplify the treatment */
                length += 6;

                /** We loop while we receive the exact length of data*/

                while( lengthOfData < length){

                        /**Read the complete PDU */

                    MBMESN_tcpReadFromClient(clientId, data + lengthOfData , length - lengthOfData , &nbBytesRead);

                }

                /**We add the header at response data */
                for(int i=0;i<7;i++)dataToSend[i]=mbap_Header[i];
                lengthOfDataToSend += 7;

                /** We treat the request */

                HandleMBRequest(data+7,length-7,dataToSend+lengthOfDataToSend,&nbBytesToSend);

                lengthOfDataToSend += nbBytesToSend;


                 /** Initialize the length of data */

                 dataToSend[4] =  (uint8_t )((nbBytesToSend + 1)/256);
                 dataToSend[5] =  (uint8_t)((nbBytesToSend + 1)%256);


                /** Write to client **/

                MBMESN_tcpWriteToClient(clientId, dataToSend,lengthOfDataToSend);

                /** See if there is more data */
                MBMESN_tcpReadFromClient(clientId, data , RCVBUFSIZE, &nbBytesRead);



        }



                            /** Close client connection  **/

                            MBMESN_CloseConnectionWithClient(clientId);
    }



    printf("Hello world!\n");
    return 0;
}
