
/*
 * FileUtil.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <iostream>
#include <fstream>
#include <string>
#include "../Includes.h"

using namespace std;

namespace ARK {
	namespace Util {
		/*!
		 * \brief File operations functionality. Framework File IO goes through the Resource class.
		 *
		 * This remains public for custom file types.
		 *
		 * @see Resource
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class FileUtil {
			private:
			public:
				static string prependPlatform(string filename);
				static bool file_put_contents(string filename, string contents);
				static string getCurrentDirectory();
				static string getSeparator();
				static string getOSDirectory();
				static void openBrowserToURL(string url);
		};
	}
}

#endif /* FILEUTIL_H_ */
