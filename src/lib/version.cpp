#include <oscope/version.h>
#include <pthread.h>

namespace oscope {
pthread_once_t version_once = PTHREAD_ONCE_INIT;

static Version sVersion;

void initialize_version() {
	sVersion.minor = 0;
	sVersion.major = 1;
	sVersion.git_revision = "###GIT_REVISION###";
}

Version Version::instance() {
	pthread_once(&version_once, initialize_version);

	return sVersion;
}


}

