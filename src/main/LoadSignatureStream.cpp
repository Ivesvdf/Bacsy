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

#include "LoadSignatureStream.h"

LoadSignatureStream::LoadSignatureStream():
	RsyncStream<1024>(dumpStream, rs_loadsig_begin(&signature)),
	dumpStream(std::cerr)
{

}

LoadSignatureStream::~LoadSignatureStream()
{
	rs_free_sumset(signature);
}

rs_signature_t* LoadSignatureStream::getSignature() const
{
	return signature;
}
