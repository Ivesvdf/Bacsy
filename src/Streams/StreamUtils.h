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

#include <limits>
#include "Poco/Buffer.h"
#include "Poco/MD5Engine.h"
#include "Poco/Net/DialogSocket.h"
#include "Common/StringUtils.h"

namespace bacsy
{

class SimpleIStream
{
public:
	virtual std::streamsize read(char* s, std::streamsize n)=0;
	virtual bool isOk() const = 0;
};

class SimpleOStream
{
public:
	virtual void write(const char* const s, std::streamsize n)=0;
	virtual bool isOk() const = 0;
};

class SimpleIOStream : public SimpleIStream, public SimpleOStream
{

};

class SimpleIStreamStream : public SimpleIStream
{
public:
	SimpleIStreamStream(std::istream& istream);
	std::streamsize read(char* s, std::streamsize n);
	bool isOk() const;
private:
	std::istream& istream;
};

class SimpleOStreamStream : public SimpleOStream
{
public:
	SimpleOStreamStream(std::ostream& ostream);
	void write(const char* const s, std::streamsize n);
	bool isOk() const;
private:
	std::ostream& ostream;
};

class SimpleDialogSocketStream : public SimpleIOStream
{
	public:
		SimpleDialogSocketStream(Poco::Net::DialogSocket& socket);
		void write(const char* const s, std::streamsize n);
		std::streamsize read(char* s, std::streamsize n);
		bool isOk() const;

	private:
		Poco::Net::DialogSocket& socket;
		bool ok;
};

class SimpleTee: public SimpleOStream
{
public: 
	SimpleTee();

	SimpleTee(std::vector<SimpleOStream*> outputs);

	void addOutput(SimpleOStream& ostream);
	virtual void write(const char* const s, std::streamsize n);
	virtual bool isOk() const;
private:
	std::vector<SimpleOStream*> outputs;
};

class MD5OutputStream : public SimpleOStream
{
public:
	virtual void write(char* s, std::streamsize n);

	virtual bool isOk() const;
	const Poco::MD5Engine& getEngine() const;

private:
	Poco::MD5Engine engine;
};

class StreamUtilities
{
public:
	static std::streamsize copyStream(std::istream& istr, std::ostream& ostr, unsigned bufferSize, std::streamsize limit);

	static std::streamsize copyStream(
			std::istream& istr,
			Poco::Net::DialogSocket& outSocket,
			unsigned bufferSize = 8192,
			std::streamsize limit = std::numeric_limits<std::streamsize>::max());

	static std::streamsize copyStream(
			Poco::Net::DialogSocket& inSocket,
			std::ostream& ostr,
			unsigned bufferSize = 8192,
			std::streamsize limit = std::numeric_limits<std::streamsize>::max());

	static std::streamsize copyStream(
			Poco::Net::DialogSocket& inSocket,
			SimpleOStream& ostream,
			unsigned bufferSize = 8192,
			std::streamsize limit = std::numeric_limits<std::streamsize>::max());

	static std::streamsize copyStream(
			SimpleIStream& in,
			SimpleOStream& out,
			unsigned bufferSize = 8192,
			std::streamsize limit = std::numeric_limits<std::streamsize>::max());
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

}
#endif
