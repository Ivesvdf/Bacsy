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

#include <tclap/CmdLine.h>
#include "Poco/Thread.h"
#include "Server/BacsyServer.h"
#include "Server/MulticastResponder.h"
#include "Server/CascadingStoreConfiguration.h"
#include "Common/ConfigurationFile.h"
#include "info.h"
#include "Common/ArgParsingUtils.h"
#include "Common/Environment.h"

using namespace bacsy;


int main(int argc, char **argv)
{
	try 
	{  
		TCLAP::CmdLine cmd("Server for the Bacsy backup system", ' ', bacsyVersion);

		TCLAP::ValueArg<std::string> configArg(
				"c",
				"configdir",
				"Directory in which to look for configuration files.",
				false,
				Environment::getDefaultConfigurationDirectory(),
				"string");
		TCLAP::MultiArg<std::string> defArg(
				"D",
				"definition",
				"Add a definition to the sources configuration. Format: [section]key=value",
				false,
				"string");

		cmd.add(configArg);
		cmd.add(defArg);

		cmd.parse(argc, argv);

		std::string configdir = configArg.getValue();
		std::vector<std::string> definitions = defArg.getValue();

		CascadingStoreConfiguration configuration(configdir);
		ConfigurationFile& sourcesFile = configuration.getConfig();

		ArgParsingUtils::processDefinitions(definitions, sourcesFile);

		if(!configuration.isLoaded())
		{
			LOGF("No " + configdir + "stores.config file found.");
		}

		StoreManager storeManager(configuration);

		BacsyServer server(storeManager);
		server.start();

		MulticastResponder responder;
		responder.start();

		while(true)
			Poco::Thread::sleep(10000);
	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
	}

	return 0;
}
