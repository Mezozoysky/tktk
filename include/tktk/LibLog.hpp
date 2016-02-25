/**
 *    tktk
 * 
 *    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
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

#ifndef TKTK_LIBLOG_HPP
#define TKTK_LIBLOG_HPP

#ifdef LIBLOG_LOG_ENABLED
    #include <liblog/liblog.hpp>
    // liblog named defines
    // no defines - using default naming
#else //def LIBLOG_LOG_ENABLED
    #define DO_NOTHING do {} while( 0 )
    // liblog empty named defines
    #define ll_fatal(MSG) DO_NOTHING
    #define ll_error(MSG) DO_NOTHING
    #define ll_warning(MSG) DO_NOTHING
    #define ll_info(MSG) DO_NOTHING
    #define ll_debug(MSG) DO_NOTHING
    #define ll_trace(MSG) DO_NOTHING
#endif //def LIBLOG_LOG_ENABLED


#endif /* end of include guard: TKTK_LIBLOG_HPP */
