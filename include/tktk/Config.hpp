//  tktk
//
//  tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
// 
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief This file provides global configuration defines for tktk libraries
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_CONFIG_HPP
#define TKTK_CONFIG_HPP


#define TKTK_VERSION_MAJOR 0
#define TKTK_VERSION_MINOR 0
#define TKTK_VERSION_MICRO 0

//Define like that via cMake or uncomment two lines below to enable library logging
//#define TKTK_LL_ENABLED
//#define TKTK_LL_LEVEL 5

#ifdef TKTK_LL_ENABLED
    #define LIBLOG_LOG_ENABLED
    #ifndef TKTK_LL_LEVEL
        #define LIBLOG_LOG_LEVEL 3
    #else //ndef TKTK_LL_LEVEL
        #define LIBLOG_LOG_LEVEL TKTK_LL_LEVEL
    #endif //ndef TKTK_LL_LEVEL
#endif //TKTK_LL_ENABLED
#include <tktk/LibLog.hpp>


#endif /* end of include guard: TKTK_CONFIG_HPP */

/// \mainpage tktk documentation
/// \tableofcontents
/// tktk is a couple of libraries designed to help in solving common game development tasks; it is written in c++.
/// \section introSec Introduction
/// \subsection introNamingSubsec Naming
/// \b `tktk` is the abbreviation for 'Triko Toolkit'
///
/// \subsection introOverviewSubsec Overview
/// Toolkit consists of several libraries
/// \li \b tktk-util Provides useful types like memory pools, signal, etc
/// \li \b tktk-ecs Provides general entity-component system implementation
/// \li \b tktk-phase Provides phase management for games and interactive applications
/// \li \b tktk-asset Provides general asset storage/access
