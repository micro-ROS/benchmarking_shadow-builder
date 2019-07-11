#ifndef __SHADOW_BUILDER_H__
#define __SHADOW_BUILDER_H__

#include <shadow-builder/CodeParser.h>

#include <common/Status.h>
#include <common/FileSystem.h>

#include <tfa_pr/TFACore.h>
#include <tfa/TFABenchMarkingPlatform.h>

/* External libraries */
#include <cxxopts.hpp>
#include <tinyxml2.h>


/**
 * \brief This class in in charge of initialise the itself and the TFA.
 * 	In addition it will retrieve the run the element of the
 * 	TFA and its componenent.
 */
class ShadowBuilder {
public:
	ShadowBuilder(int argc = 0, char **argv = NULL);

	/**
	 * \brief This function is in charge of initializing the software
	 * 	it should be called from the main application function.
	 */
	const Status init();

	/**
	 * \brief This function is in charge of instrumenting the code
	 * as specified in the configuration.
	 * return Status status indicatin what happened.
	 */
	const Status startInstrumenting();

private:
	/** 
	 * \brief This function set up the argument options used in the
	 * 	application.
	 * \return Status status indicating what happened.
	 */ 
	const Status cmdOptsSetup();

	/** 
	 * \brief This function allow to retrieve the information from the 
	 * 	configuration from the argument.
	 * \return Status status indicating what happened 
	 */ 
	const Status cmdOptsParse();

	/**
	 * \brief This function is in charge of opening the XML FILE 
	 * \param name The name of the module loading the xml file
	 * \param file The XML
	 */
	const Status openXml(string& name, 
			string& file, tinyxml2::XMLDocument& xmlDoc);

	/** 
	 * 
	 */ 
	const Status retrieveSBConf();

	/** 
	 * 
	 */ 
	const Status retrieveTFAConf();

	/**
	 * \brief This function is just createing the output folders that will
	 * hold the results.
	 */
	const Status createOutputTree();

	cxxopts::Options m_cmdOpts;
	tinyxml2::XMLDocument m_xmlSBCfg;	
	tinyxml2::XMLDocument m_xmlTFACfg;	

	TreeFS m_sourceFiles;
	TreeFS m_pluginTFA;

	std::string m_getOutputFolder;
	std::string m_getSessionName;
	std::string m_newSrcFolder;

	Parser m_parser;
	TFABenchMarkingPlatform m_tfaPlat;
	TFAConfiguration m_tfaConf;

	int m_appArgc;
	char **m_appArgv;
};

#endif /* __SHADOW_BUILDER_H__ */  
