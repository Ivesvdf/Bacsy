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

#include <stdexcept>
#include "Bacsy/Common/RunType.h"

namespace Bacsy
{
namespace Common
{

const RunType RunType::full = RunType("full");
const RunType RunType::fullfiles = RunType("fullfiles");
const RunType RunType::delta = RunType("delta");


RunType::RunType(std::string textRepresentation):
	textRepresentation(textRepresentation)
{

}

std::string RunType::toString() const
{
	return textRepresentation;
}

const RunType& RunType::fromString(const std::string& str)
{
	if(str == full.toString())
	{
		return full;
	}
	else if(str == fullfiles.toString())
	{
		return fullfiles;
	}
	else if(str == delta.toString())
	{
		return delta;
	}
	else
	{
		throw std::runtime_error("Invalid RunType string: " + str);
	}
}

bool RunType::operator==(const RunType& other) const
{
	return textRepresentation == other.textRepresentation;
}

}
}
