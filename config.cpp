#include "config.h"
Config::Config(const std::string& configFilename)
    : m_cmdName(std::make_shared<ProcessTypes>())
    //, m_fullName(std::make_shared<ProcessTypes>())
{
    try {
        YAML::Node m_config = YAML::LoadFile(configFilename);
        YAML::Node cmdName = m_config["cmdName"];
        //YAML::Node fullName = m_config["fullName"];
        getValues(cmdName, m_cmdName);
/*        getValues(fullName, m_fullName);    */ 
        std::cout << *this << std::endl;
    }
    catch (YAML::ParserException& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }
}
void Config::getValues(const YAML::Node& node, std::shared_ptr<ProcessTypes> Values) {
    for (YAML::Node::const_iterator it = node.begin(); it != node.end(); it++) {
        Values->push_back(it->as<std::string>());
    }
}
std::ostream& operator<<(std::ostream& os, const Config& cfg) {
    os << "Process Name to monitor:" << std::endl;
    for (const auto& i : *(cfg.m_cmdName)) {
        os <<"\t" << i << std::endl;
    }
  /*  os << "fullName:" << std::endl;
    for (const auto& i : *(cfg.m_fullName)) {
        os << "\t" << i << std::endl;
    }*/
    return os;
}
std::shared_ptr<Config::ProcessTypes> Config::cmdName(void) {
    return m_cmdName;
}
//std::shared_ptr <Config::ProcessTypes> Config::fullName(void) {
//    return m_fullName;
//}