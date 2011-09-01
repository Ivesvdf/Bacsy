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

#ifndef LOADSIGNATURESTREAM_H
#define LOADSIGNATURESTREAM_H

#include "Bacsy/Streams/RsyncStream.h"

namespace bacsy
{

/**
 * Loads a Signature to an rs_signature_t object. This object is *owned* by
 * this LoadSignatureStream. Don't free the little bastard yourself or die
 * horribly. 
 */
class LoadSignatureStream : public RsyncStream<1024>
{
public:
	LoadSignatureStream();
	~LoadSignatureStream();

	rs_signature_t* getSignature() const;
private:
	SimpleOStreamStream dumpStream;
	rs_signature_t* signature;
};

}
#endif
