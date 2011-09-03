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

#ifndef DUMMY_FILE_H
#define DUMMY_FILE_H

#include "Bacsy/Common/IFile.h"

namespace Bacsy
{

class DummyFile : public IFile
{
public:
	DummyFile(const std::string& path) :
		pathString(path),
		bExists(true),
		bIsFile(true),
		bIsDir(false),
		bIsLink(false)
	{}

	void setPath(const std::string& inPath) { pathString = inPath; }
	const std::string& path() const { return pathString; }
		/// Returns the path.
	
	void setExists(bool e){ bExists = e; }

	bool exists() const { return bExists; }
		/// Returns true iff the file exists.
		
	void setIsFile(bool e){ bIsFile = e; }
	bool isFile() const { return bIsFile; }
		/// Returns true iff the file is a regular file.
		
	void setIsDirectory(bool e){ bIsDir = e; }
	bool isDirectory() const { return bIsDir; }
		/// Returns true iff the file is a directory.	
		
	void setIsLink(bool e){ bIsLink = e; }
	bool isLink() const { return bIsLink; }
		/// Returns true iff the file is a symbolic link.
		
	Poco::Timestamp getLastModified() const { return lastModified; }
		/// Returns the modification date of the file.
		
	void setLastModified(const Poco::Timestamp& ts) { lastModified = ts; }
		/// Sets the modification date of the file.
		
	Poco::File::FileSize getSize() const { return size; }
		/// Returns the size of the file in bytes.
		
	void setSize(Poco::File::FileSize isize) { size = isize; }
		/// Sets the size of the file in bytes. Can be used
		/// to truncate a file.
private:
	std::string pathString;
	bool bExists;
	bool bIsFile;
	bool bIsDir;
	bool bIsLink;
	Poco::Timestamp lastModified;
	Poco::File::FileSize size;
};

}
#endif
