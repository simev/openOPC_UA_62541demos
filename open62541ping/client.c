#include <stdio.h>
#ifdef USE_AMALGAMATION
#include "open62541.h"
#else
#include <ua_client_highlevel.h>
#include <ua_config_default.h>
#include <ua_log_stdout.h>
#endif

int main(int argc, char *argv[])
{
    /* Create a client and connect */
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);

    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://192.0.2.1:4840");
    if(status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return status;
    }

    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    status = UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, "the.answer"), &value);
    if(status == UA_STATUSCODE_GOOD &&
       UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
        printf("the value is: %i\n", *(UA_Int32*)value.data);
    }

#if 0
    (*(UA_Int32*)value.data)++;

    status = UA_Client_writeValueAttribute(client, UA_NODEID_STRING(1, "the.answer"), &value);
    if(status != UA_STATUSCODE_GOOD) {
	    printf("error modify value\n");
    }
#endif

    /* Clean up */
    UA_Variant_deleteMembers(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
    return status;
}
