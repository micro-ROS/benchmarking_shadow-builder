#include <common/StringMan.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace common;

#define START_BM_COMMENT "/**"
#define END_BM_COMMENT "*/"

void StringMan::removeCommentBeacons(std::string& inStr)
{
	size_t pos_start = inStr.find(START_BM_COMMENT);
	size_t pos_end = inStr.find(END_BM_COMMENT);

	if (pos_start == std::string::npos ||
			pos_end == std::string::npos) {
		cout << "[StrMan] could not find commentary beacons" << endl;
		return;
	}
	pos_start += sizeof(START_BM_COMMENT) - 1;
	size_t length_to_end = pos_end - pos_start;
	inStr = inStr.substr(pos_start, length_to_end);

	cout << "[StrMan]: String without becons "  << inStr << endl;
}

void StringMan::trimSpace(string& inStr)
{
	size_t prevspacepos = inStr.find_first_not_of(" ") ;
	size_t curpos;
	size_t strlen = inStr.length();
	
	string comtemp = inStr;
	inStr = "";
	
	for (curpos = prevspacepos; curpos <= strlen; curpos++) {
		if (comtemp[curpos] == ' ') {
			continue;
		}
		inStr += comtemp[curpos];
	}

	cout << "Length of string is now -> " << inStr.length() << endl;
	cout << "[StrMan]: String trimed "  << inStr << endl;
}

bool StringMan::split(const string& inStr,
		      const char delim,
		      vector<string>& strings)
{
	bool rc = false;
	istringstream ss(inStr);
	string token;

	while (getline(ss, token, delim)) {
		rc = true;
		strings.push_back(token);
	}

	return rc;
}

bool StringMan::split(const string& inStr,
		      const string& delim,
		      vector<string>& strings)
{
	size_t prev_pos = 0;
	size_t pos =  inStr.find(delim);
	string token;

	if (pos == string::npos) {
	/** Could not find any deliminiters */
		return false;
	}

	do {
		token = std::string(inStr.substr(prev_pos, pos - prev_pos));
		strings.push_back(token);
		prev_pos = pos + 2;
		pos = inStr.find(delim, prev_pos);
	} while(pos != std::string::npos);

	if (prev_pos <= inStr.length()) {
		token = std::string(inStr.substr(prev_pos));
		strings.push_back(token);
	}

	return true;
}
