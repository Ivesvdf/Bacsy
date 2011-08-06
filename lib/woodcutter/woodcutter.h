//#define DISABLE_LOG

#ifndef WOODCUTTER_H
#define WOODCUTTER_H

#ifdef DISABLE_LOG
#define LOG(severity,msg) //
#else
#define LOG(severity,msg) Woodcutter::log(msg, severity, __LINE__, __FILE__);
#endif

#define LOGI(msg) LOG(INFO,msg)
#define LOGW(msg) LOG(WARNING,msg)
#define LOGE(msg) LOG(ERROR,msg)
#define LOGF(msg) LOG(FATAL,msg)

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

#undef ERROR

const int INFO = 0, WARNING = 1, ERROR = 2, FATAL = 3;

const std::string sevletters[4] = {
				"INFO ", 
				"WARN ", 
				"ERR  ",
				"FATAL "};
const std::string sevstartcolors[4] = {"\033[;1;32m","\033[;1;35m","\033[;1;31m", "\033[;1;31;6m"};

class Woodcutter
{
	public:
		static void log(std::string msg, int severity, int line, std::string file)
		{
			if(severity >= minloglevel())
			{
				// Cut off path
				size_t fwp = file.find_last_of("/");
				size_t bwp = file.find_last_of("\\");

				if(fwp != std::string::npos || bwp != std::string::npos)
				{
					size_t last = std::string::npos;

					if(fwp != std::string::npos && bwp != std::string::npos)
						last = (fwp > bwp) ? fwp : bwp;
					else if(fwp != std::string::npos)
						last = fwp;
					else if(bwp != std::string::npos)
						last = bwp;

					// You could test if the compiled file was named / or \ but
					// then if that's the case well then you deserve this.
					file = file.substr(last+1, file.length() - last - 1);
				}

				std::ostream* outputstream = output();
				if(colors())
					(*outputstream) << sevstartcolors[severity];
				(*outputstream) << sevletters[severity] << file << ":" << line << "\t" << msg << std::endl;
				if(colors())
					(*outputstream) << "\033[;1;0m";
			}

			if(severity == FATAL)
				abort();
		}

		/**
		 * Call without parameters to return current minimum loglevel or call
		 * with exactly one integere parameter to set the minimum loglevel.
		 */
		static int minloglevel(int minlog=-1)
		{ 
			static int minloglevel = 0;
			if(minlog >= 0)
				minloglevel = minlog;
			return minloglevel;
		}

		/**
		 * Call without parameters to return the current outputstream used or
		 * call with exactly one ostream adress to set the stream we'll
		 * output all log data to
		 */
		static std::ostream* output(std::ostream* stream = 0)
		{
			static std::ostream* theStream = &std::cerr;

			if(stream != 0)
				theStream = stream;

			return theStream;
		}
			
		static void setColors(bool c = true)
		{
			colors((c == false) ? 0 : 1);
			(*(output())) << "\033[;1;0m";
		}

	private:
		static bool colors(int c = -1)
		{
			static bool color = false;

			if(c != -1)
				color = (c == 0) ? false : true;

			return color;
		}

};

/*
 * Example usage:
 * int main()
 * {
 *   Woodcutter::minloglevel(INFO);
 *   Woodcutter::setColors(true);
 *   LOGI("some info");
 *   LOG(INFO, std::string("ey") 
 *   		+ std::string(
 *   			"oi"));
 * 
 *   LOG(WARNING, "The flux capacitor is in the red!");
 *   LOG(FATAL, "The flux capacitor just blew!");
 *   LOGE("Shorthand for Error logging");
 * 
 *   std::ofstream f("output.txt");
 *   Woodcutter::output(&f);
 *   LOGI("From now on this will be in a file");
 *   LOGF("Fatal!");
 *   LOGI("This will no longer be executed");
 * }
*/

#endif

