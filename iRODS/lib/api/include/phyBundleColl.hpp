/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

#ifndef PHY_BUNDLE_COLL_HPP
#define PHY_BUNDLE_COLL_HPP

/* This is a Object File I/O call */

// =-=-=-=-=-=-=-
// stl includes

#ifdef __cplusplus
#include <string>
#endif

#include "rods.hpp"
#include "rcMisc.hpp"
#include "procApiRequest.hpp"
#include "apiNumber.hpp"
#include "structFileExtAndReg.hpp"
#include "miscUtil.hpp"

#define BUNDLE_RESC	"bundleResc"
#define TAR_BUNDLE_TYPE	"tar bundle"
#define BUNDLE_STR     "bundle" // JMC - backport 4658
#define BUNDLE_RESC_CLASS	"bundle"

#define MAX_BUNDLE_SIZE		4	/* 4 G */
#define MAX_SUB_FILE_CNT	5120 // JMC - backport 4480

typedef struct BunReplCache {
    rodsLong_t  dataId;
    char objPath[MAX_NAME_LEN];         /* optional for ADMIN_KW */
    char chksumStr[NAME_LEN]; // JMC - backport 4528
    int srcReplNum;
    struct BunReplCache *next;
} bunReplCache_t;

typedef struct BunReplCacheHeader {
    int numSubFiles;
    rodsLong_t totSubFileSize;
    bunReplCache_t *bunReplCacheHead;
} bunReplCacheHeader_t;

typedef struct CurSubFileCond {
    char collName[MAX_NAME_LEN];
    char dataName[MAX_NAME_LEN];
    rodsLong_t dataId;
    char subPhyPath[MAX_NAME_LEN];
    char cachePhyPath[MAX_NAME_LEN];
    int cacheReplNum;
    rodsLong_t subFileSize;
    int bundled;
} curSubFileCond_t;

#if defined(RODS_SERVER)
#define RS_PHY_BUNDLE_COLL rsPhyBundleColl
/* prototype for the server handler */
int
rsPhyBundleColl( rsComm_t *rsComm,
                 structFileExtAndRegInp_t *phyBundleCollInp );
int
remotePhyBundleColl( rsComm_t *rsComm,
                     structFileExtAndRegInp_t *phyBundleCollInp, rodsServerHost_t *rodsServerHost );
int
_rsPhyBundleColl( rsComm_t *rsComm, structFileExtAndRegInp_t *phyBundleCollInp,
                  const char *_resc_name );

#ifdef __cplusplus
int
createPhyBundleDataObj( rsComm_t *rsComm, char *collection, const std::string& _resc_name,
                        const char* rescHier, dataObjInp_t *dataObjInp, char *dataType ); // JMC - backport 4658
#endif

int
createPhyBundleDir( rsComm_t *rsComm, char *bunFilePath,
                    char *outPhyBundleDir, char* hier );
int
rsMkBundlePath( rsComm_t *rsComm, char *collection, char *outPath,
                int myRanNum );
int
replDataObjForBundle( rsComm_t *rsComm, char *collName, char *dataName,
                      const char *rescName, char* rescHier, char* destRescHier, int adminFlag, dataObjInfo_t *outCacheObjInfo );
int
isDataObjBundled( collEnt_t *collEnt );
int
setSubPhyPath( char *phyBunDir, rodsLong_t dataId, char *subBunPhyPath );
int
addSubFileToDir( curSubFileCond_t *curSubFileCond,
                 bunReplCacheHeader_t *bunReplCacheHeader );
int
replAndAddSubFileToDir( rsComm_t *rsComm, curSubFileCond_t *curSubFileCond,
                        const char *myRescName, char *phyBunDir, bunReplCacheHeader_t *bunReplCacheHeader );
int
bundleAndRegSubFiles( rsComm_t *rsComm, int l1descInx, char *phyBunDir,
                      char *collection, bunReplCacheHeader_t *bunReplCacheHeader, int chksumFlag ); // JMC - backport 4528
int
phyBundle( rsComm_t *rsComm, dataObjInfo_t *dataObjInfo, char *phyBunDir,
           char *collection, int oprType ); // JMC - backport 4643
#else
#define RS_PHY_BUNDLE_COLL NULL
#endif

/* prototype for the client call */
int
rcPhyBundleColl( rcComm_t *conn,
                 structFileExtAndRegInp_t *phyBundleCollInp );

#endif	/* PHY_BUNDLE_COLL_H */
