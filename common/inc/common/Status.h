#ifndef __STATUS_H__
#define __STATUS_H__

#include <string>
#include <iostream>
#include <limits.h>

using namespace std;

class Status {
public:
	typedef enum {
		ERROR = INT_MIN,
		ERROR_XML_PATH_INVALID,
		ERROR_XML_CANNOT_OPEN,
		ERROR_XML_CMD_PARSE,

		SUCCESS = 0,
	} ErrorCodes;

	Status(const string& errorMsg = string("Success"), ErrorCodes errorCode = SUCCESS);

	static const Status returnStatusOkay();
	static const Status returnStatusError(const string& str = string("Error"));

	void setErrorCode(const ErrorCodes errorCode);
	void setErrorMsg(const string errorMsg);

	friend ostream& operator<<(ostream &flux, const Status& st);
	bool operator!=(const int errorCode) const;
	bool operator==(const int errorCode);
	bool operator!=(const ErrorCodes errorCode) const;
	bool operator==(const ErrorCodes errorCode);
	/** More for debug purposes */
	bool operator==(const string& errorMsg);
	Status& operator=(ErrorCodes errorCode);
	Status& operator=(string& errorMsg);

	operator bool() const { return (m_errorCode != SUCCESS); }
private:
	string m_errorMsg;
	ErrorCodes m_errorCode;
};

#endif /* __STATUS_H__ */
