#include "Options.h"
Options::Options(int ac, char** av) : m_desc("Allowed options") {
    m_desc.add_options()
        ("help,h", "produce help message")
        ("configfile,F", po::value<std::string>(&m_default_config_path)->default_value("config.yaml"), "specify config file path(YAML format)")
        ("web.listen-address", po::value<std::string>(&m_web_listen_address)->default_value("0.0.0.0:18080"), "Address to listen on")
        ("interval", po::value<int>(&m_interval)->default_value(15), "Interval for polling process status")
        ;

    po::store(po::parse_command_line(ac, av, m_desc), m_vm);
    po::notify(m_vm);

    if (m_vm.count("help")) {
        std::cout << m_desc << "\n";
        exit(1);
    }
}
std::string Options::configPath(void) {
    return m_default_config_path;
}
std::string Options::listenAddress(void) {
    return m_web_listen_address;
}
int Options::interval(void) {
    return m_interval;
}
