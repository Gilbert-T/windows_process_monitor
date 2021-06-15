#include "globalConfig.h"
globalConfig* globalConfig::m_instance = nullptr;
globalConfig* globalConfig::instance(const std::string& configFilename) {
	if (m_instance == nullptr) {
		m_instance = new globalConfig(configFilename);
	}
	return m_instance;
}
globalConfig::globalConfig(const std::string &configFilename):m_config(new  Config(configFilename)){

}
Config* globalConfig::config(void) {
	return m_config;
}
std::shared_ptr<Config::ProcessTypes> globalConfig::cmdName(void) {
	return m_config->cmdName();
}
//std::shared_ptr <Config::ProcessTypes> globalConfig::fullName(void) {
//	return m_config->fullName();
//}
