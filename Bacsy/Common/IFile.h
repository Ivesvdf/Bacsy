/*
 * Copyright (C) 2011  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY = 0; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IFILE_H
#define IFILE_H

#include "Poco/File.h"

namespace Bacsy
{
class IFile
{
public:
	virtual const std::string& path() const = 0;
		/// Returns the path.
	
	virtual bool exists() const = 0;
		/// Returns true iff the file exists.
		
	virtual bool isFile() const = 0;
		/// Returns true iff the file is a regular file.
		
	virtual bool isLink() const = 0;
		/// Returns true iff the file is a symbolic link.
		
	virtual bool isDirectory() const = 0;
		/// Returns true iff the file is a directory.
		
	virtual Poco::Timestamp getLastModified() const = 0;
		/// Returns the modification date of the file.
		
	virtual void setLastModified(const Poco::Timestamp& ts) = 0;
		/// Sets the modification date of the file.
		
	virtual Poco::File::FileSize getSize() const = 0;
		/// Returns the size of the file in bytes.
		
	virtual void setSize(Poco::File::FileSize size) = 0;
		/// Sets the size of the file in bytes. Can be used
		/// to truncate a file.
};

}
#endif
