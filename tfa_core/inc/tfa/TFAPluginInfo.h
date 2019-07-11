#ifndef __TFA_CORE_TFAPLUGININFO_H__
#define __TFA_CORE_TFAPLUGININFO_H__

#include <tfa/TFABenchMarkingPlatform.h>
#include <tfa/ITFACommentListener.h>
#include <iostream>
#include <vector>

class TFAInfoPlugin {
public:
	TFAInfoPlugin(const std::string& pluginName, 
			const TFABenchMarkingPlatform& plat):
					mPluginName(pluginName),
					mTBMPlatorm(plat) {;}

	void setBMPlatform(const TFABenchMarkingPlatform& tBMPlatform);

	const TFABenchMarkingPlatform& getTFABMPlat() const
	{
		return mTBMPlatorm;
	}

	void addCommentBM(const std::vector<ITFACommentListener *>& icl)
	{
		mComments.insert(mComments.end(), icl.begin(), icl.end());
	}

	const TFABenchMarkingPlatform& getBMPlatform()
	{
		return mTBMPlatorm;
	}

	friend std::ostream& operator<< (std::ostream& out,
		       	const TFAInfoPlugin& ip)
	{
		out << ip.mTBMPlatorm << std::endl;
		for (const ITFACommentListener* icl: ip.mComments) {
			out << *icl << std::endl;
		}

		return out;
	}

private:
	mutable TFABenchMarkingPlatform mTBMPlatorm;
	std::vector<ITFACommentListener *> mComments;
	const std::string mPluginName;
};

#endif /* __TFA_CORE_TFAPLUGININFO_H__*/
