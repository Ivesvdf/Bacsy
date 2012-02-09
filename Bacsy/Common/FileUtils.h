/*
 * Copyright (C) 2012  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BACSY_FILEUTILS_H
#define BACSY_FILEUTILS_H

#include "Poco/Path.h"
#include "Poco/File.h"

namespace Bacsy
{
namespace Common
{
class FileUtils
{
	public:
	/**
	 * Exchanges the contents of files by moving and renaming files.
	 */
	static void swapContents(Poco::File one, Poco::File two);

};

}
}

#endif 
