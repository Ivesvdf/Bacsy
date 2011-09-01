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

#include <cstring>
#include <algorithm>
#include "Streams/SimpleBufferStream.h"

namespace bacsy
{

SimpleBufferStream::SimpleBufferStream():
	maxSize(512),
	currentSize(0),
	readOffset(0),
	buffer(new char[maxSize])
{
	
}

SimpleBufferStream::~SimpleBufferStream()
{
	delete[] buffer;
}

char* SimpleBufferStream::getBuffer(unsigned int *n)
{
	*n = currentSize;
	return buffer;
}

void SimpleBufferStream::write(const char* const c, std::streamsize size)
{
	if(currentSize + size > maxSize)
	{
		maxSize = 2*(maxSize + size);
		char* newBuffer = new char[maxSize];

		memcpy(newBuffer, buffer, currentSize);

		delete[] buffer;
		buffer = newBuffer;
	}

	memcpy(buffer+currentSize, c, size);
	currentSize += size;
}

bool SimpleBufferStream::isOk() const
{
	return true;
}

std::streamsize SimpleBufferStream::read(char* c, std::streamsize max)
{
	if(readOffset >= currentSize)
		return 0;

	const std::streamsize bytesToCopy = std::min(max, std::streamsize(currentSize-readOffset));
	memcpy(c, buffer + readOffset, bytesToCopy);

	readOffset += bytesToCopy;
	return bytesToCopy;
}

}
