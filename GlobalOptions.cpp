#include "GlobalOptions.h"
GlobalOptions* GlobalOptions::m_instance = nullptr;
GlobalOptions* GlobalOptions::instance(int ac, char** av) {
	if (m_instance == nullptr) {
		m_instance = new GlobalOptions(ac, av);
	}
	return m_instance;
}
GlobalOptions::GlobalOptions(int ac, char** av) : m_options(new Options(ac, av)){

}
std::string GlobalOptions::configPath(void) {
	return m_options->configPath();
}
std::string GlobalOptions::listenAddress(void) {
	return m_options->listenAddress();
}

int GlobalOptions::interval(void) {
	return m_options->interval();
}
