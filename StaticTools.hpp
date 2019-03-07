#ifndef KAREL_STATICTOOLS_HPP
#define KAREL_STATICTOOLS_HPP

#include <string>
#include <vector>

class StaticTools {
public:
	static std::vector<std::string> splitter(const std::string& str);
	static bool isNumber(const std::string& str);
	StaticTools() = delete;

private:
};

#endif //KAREL_STATICTOOLS_HPP
