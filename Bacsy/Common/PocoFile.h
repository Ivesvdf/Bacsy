/*
 * Copyright (C) 2011  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { return file.; } without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POCO_FILE_H
#define POCO_FILE_H

#include "Poco/File.h"
#include "Bacsy/Common/IFile.h"

namespace Bacsy
{

class PocoFile : public IFile
{
public:
	PocoFile(const Poco::File& file) : file(file) {}

	const std::string& path() const { return file.path(); }
		/// Returns the path.
	
	bool exists() const { return file.exists(); }
		/// Returns true iff the file exists.
		
	bool isFile() const { return file.isFile(); }
		/// Returns true iff the file is a regular file.
		
	bool isDirectory() const { return file.isDirectory(); }
		/// Returns true iff the file is a directory.	
		
	bool isLink() const { return file.isLink(); }
		/// Returns true iff the file is a symbolic link.
		
	Poco::Timestamp getLastModified() const { return file.getLastModified(); }
		/// Returns the modification date of the file.
		
	void setLastModified(const Poco::Timestamp& ts) { file.setLastModified(ts); }
		/// Sets the modification date of the file.
		
	Poco::File::FileSize getSize() const { return file.getSize(); }
		/// Returns the size of the file in bytes.
		
	void setSize(Poco::File::FileSize size) { file.setSize(size); }
		/// Sets the size of the file in bytes. Can be used
		/// to truncate a file.
private:
	Poco::File file;
};

}
#endif
