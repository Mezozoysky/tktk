/**
 *    LibLog
 * 
 *    LibLog - Copyright (C) 2016 Stanislav Demyanovich <stan@angrybubo.com>
 * 
 *    This software is provided 'as-is', without any express or
 *    implied warranty. In no event will the authors be held
 *    liable for any damages arising from the use of this software.
 * 
 *    Permission is granted to anyone to use this software for any purpose,
 *    including commercial applications, and to alter it and redistribute
 *    it freely, subject to the following restrictions:
 * 
 *    1.  The origin of this software must not be misrepresented;
 *        you must not claim that you wrote the original software.
 *        If you use this software in a product, an acknowledgment
 *        in the product documentation would be appreciated but
 *        is not required.
 * 
 *    2.  Altered source versions must be plainly marked as such,
 *        and must not be misrepresented as being the original software.
 * 
 *    3.  This notice may not be removed or altered from any
 *        source distribution.
 */

#ifndef LIBLOG_LIBLOG_HPP
#define LIBLOG_LIBLOG_HPP


#ifdef LIBLOG_LOG_ENABLED

    /* Severity levels
     * 0 - fatal
     * 1 - error
     * 2 - warning
     * 3 - info
     * 4 - debug
     * 5 - trace
     */
    #ifndef LIBLOG_LOG_LEVEL
        #define LIBLOG_LOG_LEVEL 3
    #endif //ndef LIBLOG_LOG_LEVEL

    #include <iostream>

    #define ll_fatal(MSG) ::std::cout << "FF:: " << MSG << std::endl

    #if LIBLOG_LOG_LEVEL > 0
        #define ll_error(MSG) do { ::std::cout << "EE:: " << MSG << std::endl; } while( 0 )
    #else //LIBLOG_LOG_LEVEL > 0
        #define ll_error(MSG) do {} while( 0 )
    #endif //LIBLOG_LOG_LEVEL > 0

    #if LIBLOG_LOG_LEVEL > 1
        #define ll_warning(MSG) do { ::std::cout << "WW:: " << MSG << std::endl; } while( 0 )
    #else //LIBLOG_LOG_LEVEL > 1
        #define ll_warning(MSG) do {} while( 0 )
    #endif //LIBLOG_LOG_LEVEL > 1

    #if LIBLOG_LOG_LEVEL > 2
        #define ll_info(MSG) do { ::std::cout << "II:: " MSG << std::endl; } while( 0 )
    #else //LIBLOG_LOG_LEVEL > 2
        #define ll_info(MSG) do {} while( 0 )
    #endif //LIBLOG_LOG_LEVEL > 2

    #if LIBLOG_LOG_LEVEL > 3
        #define ll_debug(MSG) do { ::std::cout << "DD:: " << MSG << std::endl; } while( 0 )
    #else //LIBLOG_LOG_LEVEL > 3
        #define ll_debug(MSG) do {} while( 0 )
    #endif //LIBLOG_LOG_LEVEL > 3

    #if LIBLOG_LOG_LEVEL > 4
        #define ll_trace(MSG) do { ::std::cout << "TT [\"" << __FILE__ << "\":" << __LINE__ << ", " << __FUNCTION__ << "(..)] :: " << MSG << std::endl; } while( 0 )
    #else //LIBLOG_LOG_LEVEL > 4
        #define ll_trace(MSG) do {} while( 0 )
    #endif //LIBLOG_LOG_LEVEL > 4

    #if LIBLOG_LOG_LEVEL > 5
        #pragma comment(message, "EE("__FILE__":"__LINE__"):: LibLog severity level > 5")
    #endif //LIBLOG_LOG_LEVEL > 5
#else //def LIBLOG_LOG_ENABLED
    #define ll_fatal(MSG) do {} while( 0 )
    #define ll_error(MSG) do {} while( 0 )
    #define ll_warning(MSG) do {} while( 0 )
    #define ll_info(MSG) do {} while( 0 )
    #define ll_debug(MSG) do {} while( 0 )
    #define ll_trace(MSG) do {} while( 0 )
#endif //def LIBLOG_LOG_ENABLED

#endif /* end of include guard: LIBLOG_LIBLOG_HPP */
