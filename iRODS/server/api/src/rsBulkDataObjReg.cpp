/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* rsBulkDataObjReg.c. See bulkDataObjReg.h for a description of
 * this API call.*/

#include "apiHeaderAll.hpp"
#include "dataObjOpr.hpp"
#include "objMetaOpr.hpp"
#include "icatHighLevelRoutines.hpp"

#include "irods_stacktrace.hpp"
#include "irods_file_object.hpp"

int
rsBulkDataObjReg( rsComm_t *rsComm, genQueryOut_t *bulkDataObjRegInp,
                  genQueryOut_t **bulkDataObjRegOut ) {
    sqlResult_t *objPath;
    int status;
    rodsServerHost_t *rodsServerHost = NULL;

    if ( bulkDataObjRegInp->rowCnt <= 0 ) {
        return 0;
    }

    if ( ( objPath =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_NAME ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_NAME failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    status = getAndConnRcatHost( rsComm, MASTER_RCAT, ( const char* )objPath->value,
                                 &rodsServerHost );
    if ( status < 0 || rodsServerHost == NULL ) { // JMC cppcheck
        return status;
    }

    if ( rodsServerHost->localFlag == LOCAL_HOST ) {
#ifdef RODS_CAT
        status = _rsBulkDataObjReg( rsComm, bulkDataObjRegInp,
                                    bulkDataObjRegOut );
#else
        status = SYS_NO_RCAT_SERVER_ERR;
#endif
    }
    else {
        status = rcBulkDataObjReg( rodsServerHost->conn, bulkDataObjRegInp,
                                   bulkDataObjRegOut );
    }

    return status;
}

int
_rsBulkDataObjReg( rsComm_t *rsComm, genQueryOut_t *bulkDataObjRegInp,
                   genQueryOut_t **bulkDataObjRegOut ) {
#ifdef RODS_CAT
    dataObjInfo_t dataObjInfo;
    sqlResult_t *objPath, *dataType, *dataSize, *rescName, *rescHier, *filePath,
                *dataMode, *oprType, *replNum, *chksum;
    char *tmpObjPath, *tmpDataType, *tmpDataSize, *tmpRescName, *tmpRescHier, *tmpFilePath,
         *tmpDataMode, *tmpOprType, *tmpReplNum, *tmpChksum;
    sqlResult_t *objId;
    char *tmpObjId;
    int status, i;

    if ( ( rescHier =
                getSqlResultByInx( bulkDataObjRegInp, COL_D_RESC_HIER ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_D_RESC_HIER failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( objPath =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_NAME ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_NAME failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( dataType =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_TYPE_NAME ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_TYPE_NAME failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }
    if ( ( dataSize =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_SIZE ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_SIZE failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( rescName =
                getSqlResultByInx( bulkDataObjRegInp, COL_D_RESC_NAME ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_D_RESC_NAME failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( filePath =
                getSqlResultByInx( bulkDataObjRegInp, COL_D_DATA_PATH ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_D_DATA_PATH failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( dataMode =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_MODE ) ) == NULL ) {
        rodsLog( LOG_NOTICE,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_MODE failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( oprType =
                getSqlResultByInx( bulkDataObjRegInp, OPR_TYPE_INX ) ) == NULL ) {
        rodsLog( LOG_ERROR,
                 "rsBulkDataObjReg: getSqlResultByInx for OPR_TYPE_INX failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    if ( ( replNum =
                getSqlResultByInx( bulkDataObjRegInp, COL_DATA_REPL_NUM ) ) == NULL ) {
        rodsLog( LOG_ERROR,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_DATA_REPL_NUM failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    chksum = getSqlResultByInx( bulkDataObjRegInp, COL_D_DATA_CHECKSUM );

    /* the output */
    initBulkDataObjRegOut( bulkDataObjRegOut );
    if ( ( objId =
                getSqlResultByInx( *bulkDataObjRegOut, COL_D_DATA_ID ) ) == NULL ) {
        rodsLog( LOG_ERROR,
                 "rsBulkDataObjReg: getSqlResultByInx for COL_D_DATA_ID failed" );
        return UNMATCHED_KEY_OR_INDEX;
    }

    ( *bulkDataObjRegOut )->rowCnt = bulkDataObjRegInp->rowCnt;
    for ( i = 0; i < bulkDataObjRegInp->rowCnt; i++ ) {
        tmpObjPath = &objPath->value[objPath->len * i];
        tmpDataType = &dataType->value[dataType->len * i];
        tmpDataSize = &dataSize->value[dataSize->len * i];
        tmpRescName = &rescName->value[rescName->len * i];
        tmpRescHier = &rescHier->value[rescHier->len * i];
        tmpFilePath = &filePath->value[filePath->len * i];
        tmpDataMode = &dataMode->value[dataMode->len * i];
        tmpOprType = &oprType->value[oprType->len * i];
        tmpReplNum =  &replNum->value[replNum->len * i];
        tmpObjId = &objId->value[objId->len * i];

        bzero( &dataObjInfo, sizeof( dataObjInfo_t ) );
        dataObjInfo.flags = NO_COMMIT_FLAG;
        rstrcpy( dataObjInfo.objPath, tmpObjPath, MAX_NAME_LEN );
        rstrcpy( dataObjInfo.dataType, tmpDataType, NAME_LEN );
        dataObjInfo.dataSize = strtoll( tmpDataSize, 0, 0 );
        rstrcpy( dataObjInfo.rescName, tmpRescName, NAME_LEN );
        rstrcpy( dataObjInfo.rescHier, tmpRescHier, MAX_NAME_LEN );
        rstrcpy( dataObjInfo.filePath, tmpFilePath, MAX_NAME_LEN );
        rstrcpy( dataObjInfo.dataMode, tmpDataMode, SHORT_STR_LEN );
        dataObjInfo.replNum = atoi( tmpReplNum );
        if ( chksum != NULL ) {
            tmpChksum = &chksum->value[chksum->len * i];
            if ( strlen( tmpChksum ) > 0 ) {
                rstrcpy( dataObjInfo.chksum, tmpChksum, NAME_LEN );
            }
        }

        dataObjInfo.replStatus = NEWLY_CREATED_COPY;
        if ( strcmp( tmpOprType, REGISTER_OPR ) == 0 ) {
            status = svrRegDataObj( rsComm, &dataObjInfo );
        }
        else {
            status = modDataObjSizeMeta( rsComm, &dataObjInfo, tmpDataSize );
        }
        if ( status >= 0 ) {
            snprintf( tmpObjId, NAME_LEN, "%lld", dataObjInfo.dataId );

            // =-=-=-=-=-=-=-
            // added due to lack of notificaiton of new data object
            // to resource hiers during operation.  ticket 1753
            irods::file_object_ptr file_obj(
                new irods::file_object(
                    rsComm,
                    &dataObjInfo ) );

            irods::error ret = fileModified( rsComm, file_obj );
            if ( !ret.ok() ) {
                std::stringstream msg;
                msg << __FUNCTION__;
                msg << " - Failed to signal resource that the data object \"";
                msg << dataObjInfo.objPath;
                msg << "\" was registered";
                ret = PASSMSG( msg.str(), ret );
                irods::log( ret );
                return ret.code();
            }

        }
        else {
            rodsLog( LOG_ERROR,
                     "rsBulkDataObjReg: RegDataObj or ModDataObj failed for %s,stat=%d",
                     tmpObjPath, status );
            chlRollback( rsComm );
            freeGenQueryOut( bulkDataObjRegOut );
            *bulkDataObjRegOut = NULL;
            return status;
        }
    }
    status = chlCommit( rsComm );

    if ( status < 0 ) {
        rodsLog( LOG_ERROR,
                 "rsBulkDataObjReg: chlCommit failed, status = %d", status );
        freeGenQueryOut( bulkDataObjRegOut );
        *bulkDataObjRegOut = NULL;
    }
    return status;
#else
    return SYS_NO_RCAT_SERVER_ERR;
#endif

}

int
modDataObjSizeMeta( rsComm_t *rsComm, dataObjInfo_t *dataObjInfo,
                    char *strDataSize ) {
    modDataObjMeta_t modDataObjMetaInp;
    keyValPair_t regParam;
    char tmpStr[MAX_NAME_LEN];
    int status;

    bzero( &modDataObjMetaInp, sizeof( modDataObjMetaInp ) );
    bzero( &regParam, sizeof( regParam ) );
    addKeyVal( &regParam, DATA_SIZE_KW, strDataSize );
    addKeyVal( &regParam, ALL_REPL_STATUS_KW, "" );
    snprintf( tmpStr, MAX_NAME_LEN, "%d", ( int ) time( NULL ) );
    addKeyVal( &regParam, DATA_MODIFY_KW, tmpStr );
    if ( strlen( dataObjInfo->chksum ) > 0 ) {
        addKeyVal( &regParam, CHKSUM_KW, dataObjInfo->chksum );
    }
    addKeyVal( &regParam, IN_PDMO_KW, "" );
    modDataObjMetaInp.dataObjInfo = dataObjInfo;
    modDataObjMetaInp.regParam = &regParam;

    status = _rsModDataObjMeta( rsComm, &modDataObjMetaInp );

    clearKeyVal( &regParam );

    return status;
}

