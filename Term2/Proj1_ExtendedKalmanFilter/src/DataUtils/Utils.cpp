#include "Utils.h"

const std::string DataUtils::Utils::GetData(const std::string& s) {
	auto found_null = s.find(m_EOF_delim_.c_str());
	auto posStart = s.find_first_of(m_start_delim_.c_str());
	auto posEnd = s.find_first_of(m_end_delim_.c_str());
	if (found_null != std::string::npos) {
		return mDefaultString_; // empty string if delimiters are not found
	}
	else if (posStart != std::string::npos && posEnd != std::string::npos) {
		// extract substring from posStart to posEnd
		return s.substr(posStart, posEnd - posStart + 1);
	}
	return mDefaultString_;	// empty string if delimiters are not found
}
bool DataUtils::Utils::HasData(const char* pData, const size_t pLength) {
	// "42" at the start of the message means there's a websocket message event.
	// The 4 signifies a websocket message
	// The 2 signifies a websocket event
	return (pLength && pLength > 2 && pData[0] == '4' && pData[1] == '2');
}