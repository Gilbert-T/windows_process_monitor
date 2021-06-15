#pragma once
#include "Options.h"
class GlobalOptions
{
public:
	static GlobalOptions* instance(int ac, char** av);
	std::string configPath(void);
	std::string listenAddress(void);
	int interval(void);
private:
	GlobalOptions(int ac, char** av);
	static GlobalOptions* m_instance;
	Options* m_options;
};

