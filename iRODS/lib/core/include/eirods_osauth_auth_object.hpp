/* -*- mode: c++; fill-column: 132; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#ifndef __OSAUTH_AUTH_OBJECT_H__
#define __OSAUTH_AUTH_OBJECT_H__

#include "eirods_error.hpp"
#include "eirods_auth_object.hpp"

#include "rcConnect.hpp"

// =-=-=-=-=-=-=-
// boost includes
#include <boost/shared_ptr.hpp>

namespace eirods {

    /// =-=-=-=-=-=-=-
    /// @brief constant defining the native auth scheme string
    const std::string AUTH_OSAUTH_SCHEME( "osauth" );

    /// =-=-=-=-=-=-=-
    /// @brief object for a native irods authenticaion sceheme
    class osauth_auth_object : public auth_object {
    public:
        /// =-=-=-=-=-=-=-
        /// @brief Ctor
        osauth_auth_object(rError_t* _r_error);
        osauth_auth_object(const osauth_auth_object&);
        virtual ~osauth_auth_object();

        /// =-=-=-=-=-=-=-
        /// @brief assignment operator
        virtual osauth_auth_object&  operator=(const osauth_auth_object& );

        /// =-=-=-=-=-=-=-
        /// @brief Comparison operator
        virtual bool operator==(const osauth_auth_object& ) const;

        /// =-=-=-=-=-=-=-
        /// @brief Plugin resolution operation
        virtual error resolve(
                          const std::string&, // interface for which to resolve
                          plugin_ptr& );      // ptr to resolved plugin
  
        /// =-=-=-=-=-=-=-
        /// @brief serialize object to key-value pairs
        virtual error get_re_vars(keyValPair_t&); 

        /// =-=-=-=-=-=-=-
        /// @brief accessors
        std::string user_name() const { return user_name_; }
        std::string zone_name() const { return zone_name_; }
        std::string digest   () const { return digest_;    }
 
        /// =-=-=-=-=-=-=-
        /// @brief mutators
        void user_name( const std::string& _un ) { user_name_ = _un; }
        void zone_name( const std::string& _zn ) { zone_name_ = _zn; }
        void digest   ( const std::string& _dd ) { digest_    = _dd; }
        
        private:
        /// =-=-=-=-=-=-=-
        /// @brief user name - from rcConn
        std::string user_name_;
        
        /// =-=-=-=-=-=-=-
        /// @brief zone name - from rcConn
        std::string zone_name_;
        
        /// =-=-=-=-=-=-=-
        /// @brief md5 digest computed
        std::string digest_;
            
    }; // class osauth_auth_object

    /// @brief Helpful typedef
    typedef boost::shared_ptr<osauth_auth_object> osauth_auth_object_ptr;
    
}; // namespace eirods

#endif // __OSAUTH_AUTH_OBJECT_H__