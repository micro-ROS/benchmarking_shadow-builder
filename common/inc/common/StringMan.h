#pragma once

#include <string>
#include <vector>

namespace common {

class StringMan
{
public:

	/**
	 * \brief Remove comment beacons "\/\**" and "*\/"
	 *	the string is expected to be without space
	 */
	static void removeCommentBeacons(std::string& inStr);

	/**
	 * \brief Trim space for later parsing 
	 * \param inStr reference on the string to trim.
	 */
	static void trimSpace(std::string& inStr);

	/**
	 * \brief Split the string strininStr by a char delimiter "delim"
	 * \param inStr: input char to split
	 * \param delim: the delimiter that serves as a splitter.
	 * \param strings: reference from string vector
	 *
	 * \param return true if the split was split, false if the delim
	 * 		was not found.
	 */
	static bool split(const std::string& inStr,
			             const char delim,
			             std::vector<std::string>& strings);


	/**
	 * \brief Split the string strininStr by a string delimiter "delim"
	 * \param inStr: input string to split
	 * \param delim: the delimiter that serves as a splitter.
	 * \param strings: reference from string vector
	 *
	 * \param return true if the split was split, false if the delim
	 * 		was not found.
	 */
	static bool split(const std::string& inStr,
			       const std::string& delim,
			       std::vector<std::string>& strings);
};

}; // common namespace
