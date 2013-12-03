/* -*- mode: c++; fill-column: 132; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* fileDriver.c - The general driver for all file types. */

#include "fileDriver.hpp"
#include "eirods_log.hpp"
#include "eirods_resource_plugin.hpp"
#include "eirods_data_object.hpp"
#include "eirods_stacktrace.hpp"

#include "eirods_resource_constants.hpp"
#include "eirods_resource_manager.hpp"

// =-=-=-=-=-=-=-
// Top Level Inteface for Resource Plugin POSIX create
eirods::error fileCreate( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        eirods::log( ret_err );
        return PASSMSG( "failed to resolve resource", ret_err );
    }
           
    // =-=-=-=-=-=-=-
    // make the call to the "create" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_CREATE, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'create'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }


} // fileCreate

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX open
eirods::error fileOpen( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }
    
    // =-=-=-=-=-=-=-
    // make the call to the "open" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_OPEN, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'open'", ret_err );
    
    } else {
        return CODE( ret_err.code() );

    }

} // fileOpen

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX read
eirods::error fileRead( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    void*                          _buf, 
    int                            _len ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "read" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< void*, int >( _comm, eirods::RESOURCE_OP_READ, _object, _buf, _len );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'read'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileRead

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX write
eirods::error fileWrite( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    void*                          _buf, 
    int                            _len ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }
    
    // =-=-=-=-=-=-=-
    // make the call to the "write" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< void*, int >( _comm, eirods::RESOURCE_OP_WRITE, _object, _buf, _len );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'write'", ret_err );
    } else {
        std::stringstream msg;
        msg << "Write successful.";
        return PASSMSG(msg.str(), ret_err);
    }

} // fileWrite

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX close
eirods::error fileClose( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "close" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_CLOSE, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'close'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileClose

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX unlink
eirods::error fileUnlink( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "unlink" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_UNLINK, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'unlink'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileUnlink

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX stat
eirods::error fileStat( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    struct stat*                   _statbuf ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "stat" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< struct stat* >( _comm, eirods::RESOURCE_OP_STAT, _object, _statbuf );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'stat'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileStat

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX lseek
eirods::error fileLseek( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    long long                      _offset, 
    int                            _whence ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "lseek" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< long long, int >( _comm, eirods::RESOURCE_OP_LSEEK, _object, _offset, _whence );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'lseek'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileLseek

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX mkdir
eirods::error fileMkdir( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "mkdir" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_MKDIR, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'mkdir'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileMkdir


// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX chmod
eirods::error fileChmod( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object,
    int                            _mode) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "chmod" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_CHMOD, _object, _mode );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'chmod'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileChmod

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX rmdir
eirods::error fileRmdir( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "rmdir" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_RMDIR, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'rmdir'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileRmdir

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX opendir
eirods::error fileOpendir( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "opendir" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_OPENDIR, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'opendir'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }
    
} // fileOpendir

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX closedir
eirods::error fileClosedir( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "closedir" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_CLOSEDIR, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'closedir'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }
    
} // fileClosedir

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX readdir
eirods::error fileReaddir( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    struct rodsDirent**            _dirent_ptr ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "readdir" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< struct rodsDirent** >( _comm, eirods::RESOURCE_OP_READDIR, _object, _dirent_ptr );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'readdir'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileReaddir

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin POSIX rename
eirods::error fileRename( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    const std::string&             _new_file_name ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "rename" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call<  const char* >( _comm, eirods::RESOURCE_OP_RENAME,  _object, _new_file_name.c_str() );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'rename'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileRename

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin freespace
eirods::error fileGetFsFreeSpace( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "freespace" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_FREESPACE, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'stage'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }

} // fileGetFsFreeSpace

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin truncate
eirods::error fileTruncate( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object ) {
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "truncate" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call( _comm, eirods::RESOURCE_OP_TRUNCATE, _object );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'truncate'", ret_err );
    } else {
        return CODE( ret_err.code() );
    }
   
} // fileTruncate

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin StageToCache
eirods::error fileStageToCache( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    const std::string&             _cache_file_name ) {
    // =-=-=-=-=-=-=-
    // trap empty file name
    if( _cache_file_name.empty() ) {
        eirods::error ret_err = ERROR( SYS_INVALID_INPUT_PARAM, "fileStageToCache - File Name is Empty." );
        eirods::log( ret_err );
        return ret_err;
    }
    
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr );
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "stagetocache" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< const char* >( _comm, eirods::RESOURCE_OP_STAGETOCACHE, _object, _cache_file_name.c_str() );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'stagetocache'", ret_err );
    } else {
        return SUCCESS();
    }

} // fileStageToCache

// =-=-=-=-=-=-=-
// Top Level Interface for Resource Plugin SyncToArch
eirods::error fileSyncToArch( 
    rsComm_t*                      _comm, 
    eirods::first_class_object_ptr _object, 
    const std::string&             _cache_file_name ) {
    // =-=-=-=-=-=-=-
    // trap empty file name
    if( _cache_file_name.empty() ) {
        eirods::error ret_err = ERROR( SYS_INVALID_INPUT_PARAM, "fileSyncToArch - File Name is Empty." );
        eirods::log( ret_err );
        return ret_err;
    }
    
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the path
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    eirods::error ret_err = _object->resolve( eirods::RESOURCE_INTERFACE, ptr );
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to resolve resource", ret_err );
    }

    // =-=-=-=-=-=-=-
    // make the call to the "synctoarch" interface
    resc    = boost::dynamic_pointer_cast< eirods::resource >( ptr );
    ret_err = resc->call< const char* >( _comm, eirods::RESOURCE_OP_SYNCTOARCH, _object, _cache_file_name.c_str() );

    // =-=-=-=-=-=-=-
    // pass along an error from the interface or return SUCCESS
    if( !ret_err.ok() ) {
        return PASSMSG( "failed to call 'synctoarch'", ret_err );
    } else {
        return SUCCESS();
    }

} // fileSyncToArch

// =-=-=-=-=-=-=-
// File registered with the database
eirods::error fileRegistered(
    rsComm_t*                      _comm,
    eirods::first_class_object_ptr _object ) {
    eirods::error result = SUCCESS();
    eirods::error ret;
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    ret = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret.ok() ) {
        std::stringstream msg;
        msg << __FUNCTION__;
        msg << " - Failed to resolve resource.";
        result = PASSMSG(msg.str(), ret);

    } else {
        // =-=-=-=-=-=-=-
        // make the call to the "registered" interface
        resc = boost::dynamic_pointer_cast< eirods::resource >( ptr );
        ret  = resc->call( _comm, eirods::RESOURCE_OP_REGISTERED, _object );
        if( !ret.ok() ) {
            std::stringstream msg;
            msg << __FUNCTION__;
            msg << " - Failed to call registered interface.";
            result = PASSMSG(msg.str(), ret);
        }
    }

    return result;
} // fileRegistered

// =-=-=-=-=-=-=-
// File unregistered with the database
eirods::error fileUnregistered(
    rsComm_t*                      _comm,
    eirods::first_class_object_ptr _object ) {
    eirods::error result = SUCCESS();
    eirods::error ret;
    // =-=-=-=-=-=-=-
    // retrieve the resource name given the object
    eirods::plugin_ptr   ptr;
    eirods::resource_ptr resc;
    ret = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
    if( !ret.ok() ) {
        std::stringstream msg;
        msg << __FUNCTION__;
        msg << " - Failed to resolve resource.";
        result = PASSMSG(msg.str(), ret);
    } else {
    
        // =-=-=-=-=-=-=-
        // make the call to the "open" interface
        resc = boost::dynamic_pointer_cast< eirods::resource >( ptr );
        ret  = resc->call( _comm, eirods::RESOURCE_OP_UNREGISTERED, _object );
        if( !ret.ok() ) {
            std::stringstream msg;
            msg << __FUNCTION__;
            msg << " - Failed to call unregistered interface.";
            result = PASSMSG(msg.str(), ret);
        }
    }

    return result;
} // fileUnregistered

// =-=-=-=-=-=-=-
// File modified with the database
eirods::error fileModified(
    rsComm_t*                      _comm,
    eirods::first_class_object_ptr _object ) {
    eirods::error result = SUCCESS();
    eirods::error ret;
    // =-=-=-=-=-=-=-
    // downcast - this must be called on a decendant of data object
    eirods::data_object_ptr data_obj = boost::dynamic_pointer_cast< eirods::data_object >( _object );
    std::string resc_hier = data_obj->resc_hier();
    if(!resc_hier.empty()) {
        // =-=-=-=-=-=-=-
        // retrieve the resource name given the object
        eirods::plugin_ptr   ptr;
        eirods::resource_ptr resc;
        ret = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
        if( !ret.ok() ) {
            std::stringstream msg;
            msg << __FUNCTION__;
            msg << " - Failed to resolve resource.";
            result = PASSMSG(msg.str(), ret);
        } else {
        
            // =-=-=-=-=-=-=-
            // make the call to the "open" interface
            resc = boost::dynamic_pointer_cast< eirods::resource >( ptr );
            ret  = resc->call( _comm, eirods::RESOURCE_OP_MODIFIED, _object );
            if( !ret.ok() ) {
                std::stringstream msg;
                msg << __FUNCTION__;
                msg << " - Failed to call modified interface.";
                result = PASSMSG(msg.str(), ret);
            }
        }
    } else {
        // NOOP okay for struct file objects
    }

    return result;

} // fileModified

// =-=-=-=-=-=-=-
// File modified with the database
eirods::error fileNotify(
    rsComm_t*                      _comm,
    eirods::first_class_object_ptr _object,
    const std::string&             _operation ) {
    eirods::error result = SUCCESS();
    eirods::error ret;
    // =-=-=-=-=-=-=-
    // downcast - this must be called on a decendant of data object
    eirods::data_object_ptr data_obj = boost::dynamic_pointer_cast< eirods::data_object >( _object );
    std::string resc_hier = data_obj->resc_hier();
    if(!resc_hier.empty()) {
        // =-=-=-=-=-=-=-
        // retrieve the resource name given the object
        eirods::plugin_ptr   ptr;
        eirods::resource_ptr resc;
        ret = _object->resolve( eirods::RESOURCE_INTERFACE, ptr ); 
        if( !ret.ok() ) {
            std::stringstream msg;
            msg << "Failed to resolve resource.";
            result = PASSMSG(msg.str(), ret);
        } else {
        
            // =-=-=-=-=-=-=-
            // make the call to the "open" interface
            resc = boost::dynamic_pointer_cast< eirods::resource >( ptr );
            ret  = resc->call< const std::string* >( 
                       _comm, 
                       eirods::RESOURCE_OP_NOTIFY, 
                       _object,
                       &_operation );
            if( !ret.ok() ) {
                std::stringstream msg;
                msg << "Failed to call notify interface.";
                result = PASSMSG(msg.str(), ret);
            }
        }
    } else {
        // NOOP okay for struct file objects
    }

    return result;

} // fileNotify