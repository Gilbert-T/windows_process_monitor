#pragma once
#include "config.h"
class globalConfig
{
public:
	globalConfig(const globalConfig& other) = delete;
	globalConfig& operator=(const globalConfig& rhs) = delete;
	static globalConfig* instance(const std::string &configFilename);
	Config* config(void);
	std::shared_ptr <Config::ProcessTypes> cmdName(void);
	//std::shared_ptr <Config::ProcessTypes> fullName(void);

private:
	globalConfig(const std::string &configFilename);
	static globalConfig* m_instance;
	Config* m_config;
};

