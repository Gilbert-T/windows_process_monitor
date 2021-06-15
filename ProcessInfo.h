#pragma once
#include <string>
class ProcessInfo
{
public:
	ProcessInfo() = default;
	//get
	std::string moduleName(void);
	std::string fullName(void);
	std::string pid(void);

	//set
	void set_moduleName(const std::string& s);
	void set_fullName(const std::string& s);
	void set_pid(const std::string& s);
private:
	std::string m_module_name;
	std::string m_full_name;
	std::string m_pid;
};

