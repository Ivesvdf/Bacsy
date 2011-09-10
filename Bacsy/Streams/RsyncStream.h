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

#ifndef BACSY_RSYNC_STREAM_H
#define BACSY_RSYNC_STREAM_H

#include "Bacsy/Streams/StreamUtils.h"
#include "librsync.h"
#include "woodcutter/woodcutter.h"
#include "Bacsy/Streams/NullStream.h"

namespace Bacsy
{
namespace Streams
{

using namespace Common;


template<int buffersize = 1024>
class RsyncStream : public SimpleOStream
{
public:
	RsyncStream(SimpleOStream& output, rs_job_t* job);
	~RsyncStream();
	virtual void write(const char* const c, std::streamsize size);
	virtual bool isOk() const;

	virtual void close();

protected:
	rs_job_t* job;

private:
	rs_buffers_s buffers;
	bool closed; 

	SimpleOStream& output;
};

template<int buffersize>
RsyncStream<buffersize>::RsyncStream(SimpleOStream& output, rs_job_t* job):
	job(job),
	closed(false),
	output(output)
{

}

template<int buffersize>
RsyncStream<buffersize>::~RsyncStream()
{
	if(!closed)
		close();
	rs_job_free(job);
}

template<int buffersize>
void RsyncStream<buffersize>::close()
{
	if(closed)
		return;

	char outputBuffer[buffersize];

	buffers.next_in = 0;
	buffers.avail_in = 0;

	buffers.next_out = outputBuffer;
	buffers.avail_out = buffersize;

	buffers.eof_in = 1;

	int result;
	do
	{
		result = rs_job_iter(job, &buffers);

		if(result != RS_DONE && result != RS_BLOCKED)
		{
			LOGE("Something went wrong while using librsync - return value = " + StringUtils::toString(result));
		}
		else
		{
			output.write(outputBuffer, buffers.next_out - outputBuffer);
			buffers.next_out = outputBuffer;
			buffers.avail_out = buffersize;
		}
	}
	while(result != RS_DONE);

	closed = true;
}

template<int buffersize>
void RsyncStream<buffersize>::write(const char* const c, std::streamsize size)
{
	// Allocate a buffer as big as the input; this should *always* be big
	// enough for a signature. 
	// Poco::Buffer<char> outputBuffer(size);
	char outputBuffer[1024];

	buffers.next_in = const_cast<char*>(c);
	buffers.avail_in = size;

	buffers.next_out = outputBuffer;
	buffers.avail_out = 1024;

	buffers.eof_in = 0;

	do
	{
		const int result = rs_job_iter(job, &buffers);

		if(result != RS_DONE && result != RS_BLOCKED)
		{
			LOGE("Something went wrong while using librsync - return value = " + StringUtils::toString(result));
		}
		else
		{
			output.write(outputBuffer, buffers.next_out - outputBuffer);
			buffers.next_out = outputBuffer;
			buffers.avail_out = 1024;
		}
	}
	while(buffers.next_in - c < size);
}

template<int buffersize>
bool RsyncStream<buffersize>::isOk() const
{
	return true;
}

}
}
#endif
