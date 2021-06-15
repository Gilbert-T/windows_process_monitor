#include "ProcessInfo.h"
std::string ProcessInfo::moduleName(void) {
	return m_module_name;
}
std::string ProcessInfo::fullName(void) {
	return m_full_name;
}
std::string ProcessInfo::pid(void) {
	return m_pid;
}
void ProcessInfo::set_moduleName(const std::string& s) {
	m_module_name = s;
}
void ProcessInfo::set_fullName(const std::string& s) {
	m_full_name = s;
}
void ProcessInfo::set_pid(const std::string &s) {
	m_pid = s;
}