#ifndef _OSCOPE_VERSION_H_
#define _OSCOPE_VERSION_H_

#include <string>

namespace oscope {

	using namespace std;

	struct Version {
		uint8_t major;
		uint8_t minor;
		string git_revision;

	public:
		static Version instance();
	};

};

#endif // _OSCOPE_VERSION_H_
