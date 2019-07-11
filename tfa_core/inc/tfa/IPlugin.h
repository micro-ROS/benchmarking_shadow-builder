#ifndef __TFA_CORE_IPLUGIN_H__
#define __TFA_CORE_IPLUGIN_H__

#include <string>
#include <tfa/TFAPluginInfo.h>
#include <tfa/ITFACommentListener.h>
#include <vector>


class IPlugin {
public:
	IPlugin(): mInfos(NULL) {}
	virtual ~IPlugin() {}

	virtual TFAInfoPlugin& getInfoPlugins() = 0;
	virtual bool initializePlugin() = 0;

	bool dispatchComment(const TFACommentInfo& cleanComment,
			     std::string& replacement)
	{
		for (ITFACommentListener* icl: iclVect) {
			std::cout << "[IPlugin] comment expected " << icl->getCommentSensitivity()
				<< " currently parsing" << cleanComment.getCommentNoParam() << std::endl;

			if (*icl != cleanComment.getCommentNoParam())  {
				std::cout << icl->getCommentSensitivity() << std::endl;
				continue;
			}

			if (icl->runnableComments(cleanComment,
							replacement)) {
				std::cout <<
					"Error while running comment "
					<< cleanComment.getCommentRaw()
					<< std::endl;
				continue;
			}

			return true;
		}

		return false;
	}

protected:
	TFAInfoPlugin* mInfos;
	TFABenchMarkingPlatform *tbp;
	std::vector<ITFACommentListener *> iclVect;
};

typedef IPlugin* create_t();
typedef void destroy_t(IPlugin*);

#endif /* __TFA_CORE_IPLUGIN_H__ */
