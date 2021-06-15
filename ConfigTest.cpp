#include "config.h"

int main(int argc, char** argv) {
	std::shared_ptr<Config> cfg = std::make_shared<Config>("config.yaml");
	auto cmd = cfg->cmdName();
	auto full = cfg->fullName();

	return 0;
}