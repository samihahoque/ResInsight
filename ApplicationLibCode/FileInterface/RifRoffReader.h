/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020 Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <exception>
#include <map>
#include <string>

#include <QString>

//==================================================================================================
///
//==================================================================================================
class RifRoffReaderException : public std::exception
{
public:
    RifRoffReaderException( const std::string& message )
        : message( message )
    {
    }

    std::string message;
};

//==================================================================================================
///
//==================================================================================================
class RifRoffReader
{
public:
    // Throws RifRoffReaderException on error
    static void readCodeNames( const QString& filename, const QString& parameterTagName, std::map<int, QString>& codeNames );
};