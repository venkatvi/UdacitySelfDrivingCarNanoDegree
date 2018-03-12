#include "Utils.h"

const std::string DataUtils::Utils::getData(const std::string& s){
	auto found_null = s.find(mEOFDelimiter_.c_str());
	auto positionStart = s.find_first_of(mDataStartDelimiter_.c_str());
	auto positionEnd = s.find_first_of(mDataEndDelimiter_.c_str());
	if (found_null != std::string::npos) {
	  return mDefaultString_;
	}
	else if (positionStart != std::string::npos && positionEnd != std::string::npos) {
	  return s.substr(positionStart, positionEnd - positionStart + 1);
	}
	return mDefaultString_;	
}

bool DataUtils::Utils::hasData(const char* pData, const size_t pLength){
	// "42" at the start of the message means there's a websocket message event.
	// The 4 signifies a websocket message
	// The 2 signifies a websocket event
	return (pLength && pLength > 2 && pData[0] == '4' && pData[1] == '2');
}