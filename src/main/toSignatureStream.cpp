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

#include "Poco/Buffer.h"
#include "toSignatureStream.h"
#include "stringUtils.h"
#include "woodcutter/woodcutter.h"

ToSignatureStream::ToSignatureStream(SimpleOStream& output, const size_t checksumLength):
	job(rs_sig_begin(RS_DEFAULT_BLOCK_LEN,checksumLength)),
	closed(false),
	output(output)
{

}

ToSignatureStream::~ToSignatureStream()
{
	if(!closed)
		close();
	rs_job_free(job);
}

void ToSignatureStream::close()
{
	if(closed)
		return;

	char outputBuffer[1024];

	buffers.next_in = 0;
	buffers.avail_in = 0;

	buffers.next_out = outputBuffer;
	buffers.avail_out = 1024;

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
			buffers.avail_out = 1024;
		}
	}
	while(result != RS_DONE);

	closed = true;
}

void ToSignatureStream::write(const char* const c, std::streamsize size)
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

bool ToSignatureStream::isOk() const
{
	return true;
}
