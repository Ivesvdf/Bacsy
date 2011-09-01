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
#include "Bacsy/Streams/StreamUtils.h"

namespace bacsy
{

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
		SimpleIStream& in,
		SimpleOStream& out,
		unsigned bufferSize,
		std::streamsize limit)
{
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

std::streamsize StreamUtilities::copyStream(
			Poco::Net::DialogSocket& inSocket,
			SimpleOStream& ostream,
			unsigned bufferSize,
			std::streamsize limit)
{
	SimpleDialogSocketStream in(inSocket);
	return copyStreamImpl(in, ostream, bufferSize, limit);
}

SimpleIStreamStream::SimpleIStreamStream(std::istream& istream):
		istream(istream)
{
}

std::streamsize SimpleIStreamStream::read(char* s, std::streamsize n)
{
	istream.read(s, n);
	return istream.gcount();
}

bool SimpleIStreamStream::isOk() const
{
	return istream;
}

SimpleOStreamStream::SimpleOStreamStream(std::ostream& ostream):
		ostream(ostream)
{
}

void SimpleOStreamStream::write(const char* const s, std::streamsize n)
{
	ostream.write(s, n);
}

bool SimpleOStreamStream::isOk() const
{
	return ostream;
}

SimpleDialogSocketStream::SimpleDialogSocketStream(Poco::Net::DialogSocket& socket):
	socket(socket),
	ok(true)
{}

void SimpleDialogSocketStream::write(const char* const s, std::streamsize n)
{
	socket.sendBytes(s, n);
}

std::streamsize SimpleDialogSocketStream::read(char* s, std::streamsize n)
{
	const std::streamsize rv = socket.receiveRawBytes(s, n);

	if(rv == 0)
	{
		ok = false;
	}

	return rv;
}

bool SimpleDialogSocketStream::isOk() const
{
	return ok;
}

SimpleTee::SimpleTee()
{
}

SimpleTee::SimpleTee(std::vector<SimpleOStream*> outputs):
	outputs(outputs)
{
}

void SimpleTee::addOutput(SimpleOStream& ostream)
{
	outputs.push_back(&ostream);
}

void SimpleTee::write(const char* const s, std::streamsize n)
{
	for(std::vector<SimpleOStream*>::iterator it = outputs.begin();
			it != outputs.end();
			it++)
	{
		(*it)->write(s, n);
	}
}

bool SimpleTee::isOk() const
{
	bool ok = true;

	for(std::vector<SimpleOStream*>::const_iterator it = outputs.begin();
			it != outputs.end();
			it++)
	{
		if(!(*it)->isOk())
		{
			ok = false;
			break;
		}
	}

	return ok;
}

void MD5OutputStream::write(char* s, std::streamsize n)
{
	engine.update(s, n);
}

bool MD5OutputStream::isOk() const
{
	return true;
}

const Poco::MD5Engine& MD5OutputStream::getEngine() const
{
	return engine;
}

}
