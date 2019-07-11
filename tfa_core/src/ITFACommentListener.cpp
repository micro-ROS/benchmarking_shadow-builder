#include <tfa/ITFACommentListener.h>
#include <common/StringMan.h>


ITFACommentListener::ITFACommentListener(const std::string& comment)
	:
	mListeningComment(comment)
{
}

ITFACommentListener::~ITFACommentListener() {}

bool ITFACommentListener::operator==(const std::string& str) const
{
	return (str == mListeningComment);
#if 0
	if (A.mListeningComments.size() != mListeningComments.size())
		return false;

	if (!std::equal(A.mListeningComments.begin(), A.mListeningComments.end(),
		 mListeningComments.begin()))
		return false;
#endif
}

std::ostream& operator<<(std::ostream& out, ITFACommentListener const& icl)
{
	out << icl.mListeningComment;
	return out;
}

#if 0
bool ITFACommentListener::splitComments(std::string& comment,
					std::vector<std::string>& vec)
{
	bool rc = false;

	common::StringMan::trimSpace(comment);
	rc = common::StringMan::split(comment, std::string("::"), vec);

	if (!rc) {
		/** Benchmarking commands needs to follow a strict format
		 * 	Benchamarking::jj:aa: 
		 */
		return false;
	}

	for (string& strCom: vec) {
		if (strCom.find(":") == std::string::npos)
		{
			/** TODO Warning / Error print */
			std::cout << "Malformed benchmarking comment" << std::endl;
			return false;
		}
	}

	return rc;
}
#endif
