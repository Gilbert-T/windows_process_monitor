#pragma once
#include "yaml-cpp/yaml.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>

class Config
{
	friend std::ostream& operator<<(std::ostream& os, const Config& cfg);
public:
	typedef   std::vector<std::string> ProcessTypes;
	
	Config(const std::string &configFilename);
	std::shared_ptr<ProcessTypes> cmdName(void);
	//std::shared_ptr<ProcessTypes> fullName(void);
	~Config() = default;
private:
	void getValues(const YAML::Node& node, std::shared_ptr <ProcessTypes>);
	
	std::shared_ptr<ProcessTypes> m_cmdName;
	//std::shared_ptr<ProcessTypes> m_fullName;
	YAML::Node m_config;
};

