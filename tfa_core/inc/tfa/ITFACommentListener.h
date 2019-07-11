#ifndef __TFA_CORE_TFACOMMENTLISTENER_H__
#define __TFA_CORE_TFACOMMENTLISTENER_H__

#include <common/Status.h>
#include <common/StringMan.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

class TFACommentInfo {
public:
	TFACommentInfo(std::string& comment)
		: m_CommentRaw(comment)
	{
		//Just take comments without parameter if any
		size_t pos = comment.find("(");
		size_t pos_end = comment.rfind(")");

		if (pos == std::string::npos) {
			m_CommentNoParam = comment;
			return;
		}

		if (pos_end == std::string::npos) {
			return;
		}

		pos++;
		size_t length_substr = pos_end - pos;

		m_CommentNoParam = comment.substr(0, pos - 1);
		if ((pos + 1) < comment.length()) {
			std::string temp = comment.substr(pos,
						length_substr);
			std::cout << "The parameters are " << temp << std::endl;
			common::StringMan::split(temp, ',', m_Params);
		}
	}

	const std::string& getCommentRaw() const
      			 	{ return m_CommentRaw;}

	const std::string& getCommentNoParam() const 
				{ return m_CommentNoParam;}

	const std::vector<std::string>& getParams() const
       				{ return m_Params;}

	int32_t getArgCount() const { return m_Params.size();}

private:
	std::string m_CommentRaw;
	std::string m_CommentNoParam;
	std::vector<std::string> m_Params;
};


class ITFACommentListener {
public:
	/**
	 * \brief This constructor is the only way to create a comment listener.
	 * 	The string provided will be the string that the plugin will
	 * 	respond to when the tag received from the tfa_core the same as
	 * 	the comment string.
	 *
	 * \param const std::string& comment: a reference string comment/tag
	 * 	that the module will listen to.
	 */
	explicit ITFACommentListener(const std::string& comment);

	virtual ~ITFACommentListener();

	/**
	 * \brief This function will be called by the TFACore when a comment 
	 * 	starting with Benchmarking:: will parsed.
	 */
	virtual Status runnableComments(const TFACommentInfo& comment,
					std::string& replacement) = 0;

	bool operator==(const std::string& str) const;

	bool operator!=(const std::string& str) const
	{
		return !(operator==(str));
	}

	const string& getCommentSensitivity() const
	{
		return mListeningComment;
	}

	friend std::ostream& operator<<(std::ostream& out,
			ITFACommentListener const&  icl);

protected:
	std::string mListeningComment;

};

#endif /*  __TFA_CORE_TFACOMMENTLISTENER_H__ */
