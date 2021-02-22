#include <common/Status.h>

/** Does nothing right now */

Status::Status(const string& errorMsg, ErrorCodes errorCode): 
	m_errorCode(errorCode), 
	m_errorMsg(errorMsg)
{
}

const Status Status::returnStatusOkay()
{
	return Status();
}

const Status Status::returnStatusError(const string& str, ErrorCodes errorCode)
{
	Status st(str, errorCode);
	return st;
}

void Status::setErrorMsg(const string errorMsg)
{
	m_errorMsg = errorMsg;
}

void Status::setErrorCode(const ErrorCodes errorCode)
{
	m_errorCode = errorCode;
}

bool Status::operator!=(const int errorCode) const
{
	return !(m_errorCode == errorCode);
}


bool Status::operator==(const int errorCode)
{
	return (m_errorCode == (ErrorCodes) errorCode);
}

bool Status::operator!=(const ErrorCodes errorCode) const
{

	return !(m_errorCode == errorCode);
}

bool Status::operator==(const ErrorCodes errorCode)
{
	return (m_errorCode == errorCode);
}

Status& Status::operator=(ErrorCodes errorCode)
{
	m_errorCode = errorCode;
	return *this;
}

bool Status::operator==(const string& errorMsg)
{
	return (m_errorMsg == errorMsg);
}

ostream& operator<<(ostream &flux,  const Status& st)
{
	flux << st.m_errorMsg;
	return flux;
}
