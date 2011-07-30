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

#ifndef STREAM_UTILITIES_H
#define STREAM_UTILITIES_H

#include "Poco/Buffer.h"
#include "Poco/Net/DialogSocket.h"
#include "stringUtils.h"


class SimpleIStreamStream
{
public:
	SimpleIStreamStream(std::istream& istream):
		istream(istream)
	{}

	inline std::streamsize read(char* s, std::streamsize n)
	{
		istream.read(s, n);
		return istream.gcount();
	}

	inline bool isOk() const
	{
		return istream;
	}
private:
	std::istream& istream;
};

class SimpleOStreamStream
{
public:
	SimpleOStreamStream(std::ostream& ostream):
		ostream(ostream)
	{ }

	inline void write(char* s, std::streamsize n)
	{
		ostream.write(s, n);
	}

	inline bool isOk() const
	{
		return ostream;
	}
private:
	std::ostream& ostream;
};

class SimpleDialogSocketStream
{
	public:
		SimpleDialogSocketStream(Poco::Net::DialogSocket& socket):
			socket(socket),
			ok(true)
		{}

		inline void write(char* s, std::streamsize n)
		{
			socket.sendBytes(s, n);
		}

		inline std::streamsize read(char* s, std::streamsize n)
		{
			const std::streamsize rv = socket.receiveRawBytes(s, n);

			if(rv == 0)
			{
				ok = false;
			}

			return rv;
		}

		inline bool isOk() const
		{
			return ok;
		}

	private:
		Poco::Net::DialogSocket& socket;
		bool ok;
};

class StreamUtilities
{
public:
	static std::streamsize copyStream(std::istream& istr, std::ostream& ostr, unsigned bufferSize, std::streamsize limit);

	static std::streamsize copyStream(
			std::istream& istr,
			Poco::Net::DialogSocket& outSocket,
			unsigned bufferSize,
			std::streamsize limit);

	static std::streamsize copyStream(
			Poco::Net::DialogSocket& inSocket,
			std::ostream& ostr,
			unsigned bufferSize,
			std::streamsize limit);

private:
	template<typename READCLASS, typename WRITECLASS>
		static std::streamsize copyStreamImpl(
				READCLASS& read,
				WRITECLASS& write,
				unsigned bufferSize,
				std::streamsize limit)
		{
			Poco::Buffer<char> buffer(bufferSize);
			std::streamsize len = 0;
			std::streamsize n = read.read(
					buffer.begin(),
					std::min(static_cast<std::streamsize>(bufferSize), limit));
			while (n > 0 && len < limit)
			{
				len += n;
				write.write(buffer.begin(), n);
				if (read.isOk() && write.isOk() && (limit - len) > 0)
				{
					n = read.read(buffer.begin(), std::min(limit-len, static_cast<std::streamsize>(bufferSize)));
				}
				else n = 0;
			}
			return len;
		}
};

#endif
