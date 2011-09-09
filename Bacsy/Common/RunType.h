/*
 * Copyright (C) 2011  Ives van der Flaas
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

#ifndef RUNTYPE_H
#define RUNTYPE_H

#include <string>

namespace Bacsy
{
namespace Common
{

class RunType
{
public:
	std::string toString() const;
	static const RunType& fromString(const std::string& str);
	bool operator==(const RunType& other) const;

	static const RunType full;
	static const RunType fullfiles;
	static const RunType delta;
private:
	RunType(std::string textRepresentation);
	const std::string textRepresentation;
};

}
}
#endif