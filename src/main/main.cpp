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

#include <fstream>
#include "Poco/Thread.h"
#include "bacsyServer.h"
#include "target.h"
#include "configurationFile.h"



int main()
{
	BacsyServer server;
	server.start();

	std::ifstream filestream(".bacsy/targets.config");
	Target target("basicTarget", ConfigurationFile(filestream));
	target.start();
	
	while(true)
		Poco::Thread::sleep(10000);
}
