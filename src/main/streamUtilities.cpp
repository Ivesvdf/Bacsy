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

#include <algorithm>
#include <istream>
#include <ostream>
#include "woodcutter/woodcutter.h"
#include "streamUtilities.h"



std::streamsize StreamUtilities::copyStream(
		std::istream& istr,
		std::ostream& ostr,
		unsigned bufferSize,
		std::streamsize limit)
{
	SimpleIStreamStream in(istr);
	SimpleOStreamStream out(ostr);
	return copyStreamImpl(in, out, bufferSize, limit);
}


std::streamsize StreamUtilities::copyStream(
		Poco::Net::DialogSocket& inSocket,
		std::ostream& ostr,
		unsigned bufferSize,
		std::streamsize limit)
{
	SimpleDialogSocketStream in(inSocket);
	SimpleOStreamStream out(ostr);
	return copyStreamImpl(in, out, bufferSize, limit);
}

std::streamsize StreamUtilities::copyStream(
		std::istream& istr,
		Poco::Net::DialogSocket& outSocket,
		unsigned bufferSize,
		std::streamsize limit)
{
	SimpleIStreamStream in(istr);
	SimpleDialogSocketStream out(outSocket);
	return copyStreamImpl(in, out, bufferSize, limit);
}
