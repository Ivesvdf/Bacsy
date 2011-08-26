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

#include "Poco/Thread.h"
#include "BacsyServer.h"
#include "MulticastResponder.h"
#include "CascadingFileConfiguration.h"
#include "ConfigurationFile.h"


int main()
{
	CascadingFileConfiguration configuration(".bacsy");
	StoreManager storeManager(configuration);

	BacsyServer server(storeManager);
	server.start();

	MulticastResponder responder;
	responder.start();

	while(true)
		Poco::Thread::sleep(10000);
}
