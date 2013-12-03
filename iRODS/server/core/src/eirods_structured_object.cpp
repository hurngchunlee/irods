/* -*- mode: c++; fill-column: 132; c-basic-offset: 4; indent-tabs-mode: nil -*- */

// =-=-=-=-=-=-=-
// eirods includes
#include "eirods_structured_object.hpp"
#include "eirods_resource_manager.hpp"

extern eirods::resource_manager resc_mgr;

namespace eirods {

    // =-=-=-=-=-=-=-
    // public - ctor
    structured_object::structured_object() :
        file_object(),
        sub_file_path_(""),
        offset_(0),
        spec_coll_(0),
        data_type_(""),
        opr_type_(0) {
    } // structured_object

    // =-=-=-=-=-=-=-
    // public - cctor
    structured_object::structured_object( const structured_object& _rhs ) : 
        file_object( _rhs ) {
        addr_          = _rhs.addr_;
        sub_file_path_ = _rhs.sub_file_path_;
        offset_        = _rhs.offset_;
        spec_coll_     = _rhs.spec_coll_;
        data_type_     = _rhs.data_type_;
        opr_type_      = _rhs.opr_type_;

    } // cctor 

    // =-=-=-=-=-=-=-
    // public - cctor
    structured_object::structured_object( subFile_t& _sub ) :
        file_object(),
        sub_file_path_(""),
        offset_(0),
        spec_coll_(0),
        data_type_(""),
        opr_type_(0) {

        // =-=-=-=-=-=-=-
        // pull out subFile attributes
        addr_          = _sub.addr;
        sub_file_path_ = _sub.subFilePath;
        offset_        = _sub.offset;
        spec_coll_     = _sub.specColl;
        mode( _sub.mode );
        flags( _sub.flags );

        // =-=-=-=-=-=-=-
        // file* functions will fail with an empty physical_path_
        physical_path_ = _sub.subFilePath;
        logical_path(spec_coll_->objPath);

    } // structured_object

    // =-=-=-=-=-=-=-
    // public - dtor
    structured_object::~structured_object() {
    } // dtor

    // =-=-=-=-=-=-=-
    // public - assignment operator
    structured_object& structured_object::operator=( const structured_object& _rhs ) {
        // =-=-=-=-=-=-=-
        // call base class assignment first
        file_object::operator=( _rhs );

        addr_          = _rhs.addr_;
        sub_file_path_ = _rhs.sub_file_path_;
        offset_        = _rhs.offset_;
        spec_coll_     = _rhs.spec_coll_;
        data_type_     = _rhs.data_type_;
        opr_type_      = _rhs.opr_type_;

        return *this;

    }  // operator=

    // =-=-=-=-=-=-=-
    // plugin - resolve resource plugin for this object
    error structured_object::resolve( 
        const std::string& _interface, 
        plugin_ptr&        _ptr ) {
        // =-=-=-=-=-=-=-
        // check to see if this is for a resource plugin
        // resolution, otherwise it is an error
        if( RESOURCE_INTERFACE != _interface ) {
            std::stringstream msg;
            msg << "structured_object does not support a [";
            msg << _interface;
            msg << "] for plugin resolution";
            return ERROR( SYS_INVALID_INPUT_PARAM, msg.str() );
        }
 
        // =-=-=-=-=-=-=-
        // try to find the resource based on the type 
        resource_ptr resc_ptr;
        eirods::error err = resc_mgr.resolve( 
                                         "struct file", 
                                         resc_ptr );
        if( err.ok() ) {
            _ptr = boost::dynamic_pointer_cast< resource >( resc_ptr );
            return SUCCESS();

        } else {
            // =-=-=-=-=-=-=-
            // otherwise create a resource and add properties from this object
            error init_err = resc_mgr.init_from_type( 
                                          "structfile", 
                                          "struct file", 
                                          "struct_file_inst", 
                                          "empty context", 
                                          resc_ptr );
            if( !init_err.ok() ) {
                return PASSMSG( "failed to load resource plugin", init_err );
            
            }

        } // if !ok

        // =-=-=-=-=-=-=-
        // found ourselves a plugin, fill in the properties
        rodsServerHost_t* tmpRodsServerHost = 0;
        int status = resolveHost( &addr_, &tmpRodsServerHost );
        if( status < 0 ) {
            std::stringstream msg;
            msg << "resolveHost error for [";
            msg << addr_.hostAddr;
            return ERROR( status, msg.str() );
        }
        
        resc_ptr->set_property< rodsServerHost_t* >( RESOURCE_HOST, tmpRodsServerHost );

        resc_ptr->set_property<long>( RESOURCE_ID, -1 );
        resc_ptr->set_property<long>( RESOURCE_FREESPACE, -1 );
        resc_ptr->set_property<long>( RESOURCE_QUOTA, -1 );
        
        resc_ptr->set_property<int>( RESOURCE_STATUS, INT_RESC_STATUS_UP );
        
        resc_ptr->set_property<std::string>( RESOURCE_ZONE,      addr_.zoneName );
        resc_ptr->set_property<std::string>( RESOURCE_NAME,      "structfile" );
        resc_ptr->set_property<std::string>( RESOURCE_LOCATION,  addr_.hostAddr );
        resc_ptr->set_property<std::string>( RESOURCE_TYPE,      "structfile" );
        resc_ptr->set_property<std::string>( RESOURCE_CLASS,     "cache" );
        resc_ptr->set_property<std::string>( RESOURCE_PATH,      physical_path_ );
        resc_ptr->set_property<std::string>( RESOURCE_INFO,      "blank info" );
        resc_ptr->set_property<std::string>( RESOURCE_COMMENTS,  "blank comments" );
        resc_ptr->set_property<std::string>( RESOURCE_CREATE_TS, "create?" );
        resc_ptr->set_property<std::string>( RESOURCE_MODIFY_TS, "modify?" );

        _ptr = boost::dynamic_pointer_cast< resource >( resc_ptr );
        return SUCCESS();

    } // resolve
    
    // =-=-=-=-=-=-=-
    // public - get vars from object for rule engine 
    error structured_object::get_re_vars( 
        keyValPair_t& _kvp ) {
        file_object::get_re_vars( _kvp );
        // =-=-=-=-=-=-=-
        // host addr
        addKeyVal( &_kvp, HOST_ADDR_KW, addr_.hostAddr );
        addKeyVal( &_kvp, ZONE_NAME_KW, addr_.zoneName );
     
        std::stringstream pn;
        pn << addr_.portNum;
        addKeyVal( &_kvp, PORT_NUM_KW, pn.str().c_str() );

        // =-=-=-=-=-=-=-
        // misc attributes
        addKeyVal( &_kvp, SUB_FILE_PATH_KW, sub_file_path_.c_str() );

        std::stringstream off;
        off << offset_;
        addKeyVal( &_kvp, OFFSET_KW, off.str().c_str() );
        
        addKeyVal( &_kvp, DATA_TYPE_KW,     data_type_.c_str() );

        std::stringstream opr;
        opr << opr_type_;
        addKeyVal( &_kvp, OPR_TYPE_KW, opr.str().c_str() );

        // =-=-=-=-=-=-=-
        // spec coll 
        if( spec_coll_ ) {
            std::stringstream coll_class;
            coll_class << spec_coll_->collClass;
            addKeyVal( &_kvp, SPEC_COLL_CLASS_KW, coll_class.str().c_str() );

            std::stringstream type;
            type << spec_coll_->type;
            addKeyVal( &_kvp, SPEC_COLL_TYPE_KW, type.str().c_str() );

            addKeyVal( &_kvp, SPEC_COLL_OBJ_PATH_KW,  spec_coll_->objPath  );
            addKeyVal( &_kvp, SPEC_COLL_RESOURCE_KW,  spec_coll_->resource );
            addKeyVal( &_kvp, SPEC_COLL_RESC_HIER_KW, spec_coll_->rescHier );
            addKeyVal( &_kvp, SPEC_COLL_PHY_PATH_KW,  spec_coll_->phyPath  );
            addKeyVal( &_kvp, SPEC_COLL_CACHE_DIR_KW, spec_coll_->cacheDir );

            std::stringstream dirty;
            dirty << spec_coll_->cacheDirty;
            addKeyVal( &_kvp, SPEC_COLL_CACHE_DIRTY, dirty.str().c_str() );
            
            std::stringstream repl;
            repl << spec_coll_->replNum;
            addKeyVal( &_kvp, SPEC_COLL_REPL_NUM, repl.str().c_str() );
       
        }
         
        return SUCCESS();

    } // get_re_vars 

}; // namespace eirods


