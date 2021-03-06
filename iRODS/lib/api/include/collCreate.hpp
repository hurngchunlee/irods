/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* collCreate.h
 */

#ifndef COLL_CREATE_HPP
#define COLL_CREATE_HPP

/* This is a high level type API call */

#include "rods.hpp"
#include "rcMisc.hpp"
#include "procApiRequest.hpp"
#include "apiNumber.hpp"
#include "dataObjInpOut.hpp"

#if defined(RODS_SERVER)
#define RS_COLL_CREATE rsCollCreate
/* prototype for the server handler */
int
rsCollCreate( rsComm_t *rsComm, collInp_t *collCreateInp );
int
remoteCollCreate( rsComm_t *rsComm, collInp_t *collCreateInp );
int
l3Mkdir( rsComm_t *rsComm, dataObjInfo_t *dataObjInfo );
#else
#define RS_COLL_CREATE NULL
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* prototype for the client call */
int
rcCollCreate( rcComm_t *conn, collInp_t *collCreateInp );

/* rcCollCreate - Create a iRODS collection.
 * Input -
 *   rcComm_t *conn - The client connection handle.
 *   collInp_t *collInp - generic coll input. Relevant items are:
 *      collName - the collection to be registered.
 *      condInput - condition input (optional) - currently not used.
 *
 * OutPut -
 *   int status - status of the operation.
 */
#ifdef __cplusplus
}
#endif
#endif	/* COLL_CREATE_H */
