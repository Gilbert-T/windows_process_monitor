#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include "boost/program_options.hpp"
namespace po = boost::program_options;
class Options
{
public:
	Options(int ac, char ** av);
	std::string configPath(void);
	std::string listenAddress(void);
	int interval(void);
private:
	po::options_description m_desc;
	po::variables_map m_vm;
	std::string m_default_config_path;
	std::string m_web_listen_address;
	int m_interval;

};

