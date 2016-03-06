//  tktk
//
//  tktk - Copyright (C) 2016 Stanislav Demyanovich <stan@angrybubo.com>
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
/// \brief This file provides asset::Asset type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ASSET_ASSET_HPP
#define TKTK_ASSET_ASSET_HPP

#include <tktk/asset/Config.hpp>
#include <string>
#include <memory>

namespace tktk
{
namespace asset
{

/// \brief Represents the base asset abstraction
///
/// Description stub
class AssetBase
{
public:
    AssetBase() noexcept = default;
    virtual ~AssetBase() noexcept = default;

    /// \brief Constructor
    /// \param[in] filename Name of the asset file
    AssetBase( const std::string& filename ) noexcept
    : mFilename{ filename }
    {
    }

    /// \brief Get the asset file name
    /// \returns the name
    inline std::string getFilename() const noexcept
    {
        return ( mFilename );
    }

    virtual bool load() noexcept = 0;
    virtual bool create() noexcept = 0;
    virtual bool save() const noexcept = 0;

private:

    std::string mFilename;
};

template < typename DataT >
class Asset
: public AssetBase
{
public:
    /// \brief Wraps actual asset data type
    using DataTypeT = DataT;
    /// \brief Wraps shared asset data type
    using SharedDataTypeT = std::shared_ptr< DataTypeT >;

    /// \brief Constructor
    /// \param[in] filename Name of the asset file
    Asset( const std::string& filename ) noexcept
    : AssetBase( filename )
    {
    }

    /// \brief Virtual destructor for further inheritance
    virtual ~Asset() noexcept
    {
    }

    /// \brief Gets the shared pointer specialized with actual asset data type to the asset data
    inline std::shared_ptr< DataTypeT > getData() const noexcept
    {
        return ( mData );
    }

    virtual bool load() noexcept override
    {
        return ( false );
    }

    virtual bool create() noexcept override
    {
        return ( false );
    }

    virtual bool save() const noexcept override
    {
        return ( false );
    }

protected:
    std::shared_ptr< DataTypeT > mData{ nullptr };
};

} // namespace asset
} // namespace tktk

#endif //TKTK_ASSET_ASSET_HPP