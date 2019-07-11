#include <shadow-builder/ShadowBuilder.h>
#include <shadow-builder/CodeParser.h>
#include <shadow-builder/XMLConfiguration.h>

#include <common/BaseConfig.h>
#include <common/FileSystem.h>
#include <common/Status.h>


#include <tfa/TFABenchMarkingPlatform.h>
#include <tfa_pr/TFACore.h>

#include <iostream>

#include <sys/time.h>

ShadowBuilder::ShadowBuilder(int argc, char **argv)
	:
       	m_cmdOpts("shadow-builder","Benchmarking tooling"),
       	m_appArgc(argc), m_appArgv(argv),
	m_tfaPlat("", "", "", ""),
	m_tfaConf("test", m_tfaPlat)
{ 

}

const Status ShadowBuilder::init()
{
	Status st;

	if ((st = cmdOptsSetup()) != Status::SUCCESS) {
		return st;
	}

	if (m_appArgc < 2) {
		st = string("Error - missing argument" + 
				m_cmdOpts.help());
		st = Status::ERROR;
		return st;
	}

	if ((st = cmdOptsParse())) {
		return st;
	}

	return Status::returnStatusOkay();
}

const Status ShadowBuilder::cmdOptsSetup()
{
	m_cmdOpts.add_options()
	("s,cfg-shadowbuilder", "Path to the Shadow Builder configuration file",
	 cxxopts::value<std::string>())
	("t,cfg-tfa", "Path to the Trace Framework Abstraction configuration file",
	 cxxopts::value<std::string>());

	return Status::returnStatusOkay();
}

const Status ShadowBuilder::openXml(string& name, string& file, tinyxml2::XMLDocument& xmlDoc)
{
	tinyxml2::XMLError xError;
	Status st;

	if (file.empty()) {
		st = "Invalid path for " + name + 
			+ tinyxml2::XMLDocument::ErrorIDToName(xError) + " at " + file;

		st = Status::ERROR_XML_PATH_INVALID;
		return st;
	}

	if ((xError = xmlDoc.LoadFile(file.c_str()))) {
		std::cout << "Error" << std::endl;
		st = "Could not open configuration for " + name +  " " +
			+ tinyxml2::XMLDocument::ErrorIDToName(xError) + " at " + file;

		st = Status::ERROR_XML_CANNOT_OPEN;
		std::cout << st << std::endl;
		return st;
	}

#if DEBUG 
	tinyxml2::XMLPrinter printer;
	xmlDoc.Print(&printer);

	std::cout << "File openned" << std::endl;
	std::cout << printer.CStr() << std::endl;
#endif

	return st;
}

const Status ShadowBuilder::cmdOptsParse()
{
	Status st;
	auto result = m_cmdOpts.parse(m_appArgc, m_appArgv);

	if (result.count("cfg-shadowbuilder") != 1) {
		st = std::string("Need to provide cfg-shadowbuilder");
		st = Status::ERROR_XML_CMD_PARSE;
		return st;
	}


	if (result.count("cfg-tfa") != 1) {
		st = std::string("Need to provide cfg-shadowbuilder");
		st = Status::ERROR_XML_CMD_PARSE;
		return st;
	}

	string cfgSB = result["cfg-shadowbuilder"].as<string>();
	string cfgTFA = result["cfg-tfa"].as<string>();
	string appName = string("Shadow Builder");

	std::cout << cfgTFA << std::endl;
	if (cfgSB.empty()) {
		return Status::returnStatusError();
	}

	std::cout << cfgSB << std::endl;
	if (cfgSB.empty()) {
		return Status::returnStatusError();
	}

	if (openXml(appName, cfgSB, m_xmlSBCfg)) {
		return Status::returnStatusError();
	}

	if (openXml(appName, cfgTFA, m_xmlTFACfg)) {
		return Status::returnStatusError();
	}

	if (retrieveSBConf()) {
		return Status::returnStatusError();
	}
	/** Keep order as the TFAConfiguraiton needs the TFABenchMarkingPlatform
	 * object set */
	if (retrieveTFAConf()) {
		return Status::returnStatusError();
	}

	return st;
}

const Status ShadowBuilder::startInstrumenting()
{
	Status st;

	if (m_parser.setFilesToAnalyse(m_sourceFiles.getListOfFiles())) {
		std::cout << "Could not set the path within the parser " << std::endl;
		return Status::returnStatusError();
	}

	if (m_parser.startParsing()) {
		std::cout << "Could not parse" << std::endl;
		return Status::returnStatusError();
	}

	st = Status::SUCCESS;
	return st;
}

const Status ShadowBuilder::retrieveSBConf()
{
	std::string OS = "*";
	std::string CPUArch = "*";
	std::string CPUName = "*";
	std::string BoardName = "*";
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64], buf[64];
	std::vector<FSElementFilter *> filters; 

	tinyxml2::XMLElement* session_child = m_xmlSBCfg.FirstChildElement("session");
	if (!session_child) {
		return Status::returnStatusError();
	}

	if (!session_child->Attribute(SB_XML_SESSION_NAME_ATTR)) {
		std::cout << "Session name not provided" << std::endl;
		return Status::returnStatusError();
	}
	m_getSessionName = session_child->Attribute(SB_XML_SESSION_NAME_ATTR);

	if (!session_child->Attribute(SB_XML_SESSION_OUTPUT_ROOT_FOLDER_ATTR)) {
		std::cout << "Session output_root_folder not provided" << std::endl;
		return Status::returnStatusError();
	}
	m_getOutputFolder = session_child->Attribute(SB_XML_SESSION_OUTPUT_ROOT_FOLDER_ATTR);

	if (session_child->Attribute(SB_XML_SESSION_OS_ATTR)) {
		OS = session_child->Attribute(SB_XML_SESSION_OS_ATTR);
	}

	if (session_child->Attribute(SB_XML_SESSION_CPU_ARCH_ATTR)) {
		CPUArch = session_child->Attribute(SB_XML_SESSION_CPU_ARCH_ATTR);
	}

	if (session_child->Attribute(SB_XML_SESSION_CPU_NAME_ATTR)) {
		CPUName = session_child->Attribute(SB_XML_SESSION_CPU_NAME_ATTR);
	}

	m_tfaPlat = TFABenchMarkingPlatform(OS, CPUArch, CPUName, BoardName);

	if (session_child->Attribute(SB_XML_SESSION_BOARD_NAME_ATTR)) {
		BoardName = session_child->Attribute(SB_XML_SESSION_BOARD_NAME_ATTR);
	}

	const tinyxml2::XMLElement* sources = session_child->FirstChildElement("sources");
	if (!sources) {
		std::cout << "no sources provided" << std::endl;
		return Status::returnStatusError();
	}

	const tinyxml2::XMLElement* child_prjsrc = sources->FirstChildElement("prj_src");
	if (!child_prjsrc) {
		std::cout << " prj_src not provided" << std::endl;
		return Status::returnStatusError();
	}

	if (!child_prjsrc->GetText())
	{
		return Status::returnStatusError();
	}

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y_%m_%d_%H_%M_%S", nowtm);

	m_newSrcFolder = m_getOutputFolder + "/" + m_getSessionName + "_" + tmbuf + "/";
	std::string cmd = "mkdir -p " + std::string(child_prjsrc->GetText())  + " " +
						m_newSrcFolder;

	if (system(cmd.c_str())) {
		std::cout << "Cannot execute command " << cmd << std::endl;
		return Status::returnStatusError();
	}

	cmd = "cp -r " + std::string(child_prjsrc->GetText()) + "/* " + m_newSrcFolder;

	if (system(cmd.c_str())) {
		std::cout << "Cannot execute command " << cmd << std::endl;
		return Status::returnStatusError();
	}

	if (m_sourceFiles.addPath(m_newSrcFolder, true)) {
		std::cout << "Issue getting test" << std::endl;
		return Status::returnStatusError();
	}
	

	for( const tinyxml2::XMLElement* child  = child_prjsrc->NextSiblingElement();
			child; 
			child = child->NextSiblingElement())
	{
		bool isExcluded;

		if (!strcmp(child->Name(), "include"))
			isExcluded = false;
		else if (!strcmp(child->Name(), "exclude"))
			isExcluded = true;
		else {
			std::cout << "Unknown child" << std::endl;
			continue;
		}

		const char *type = child->Attribute("type");
		if (!strcmp("ext", type)) {
			filters.emplace_back(
					new FSElementFilterExt(child->GetText(), isExcluded)
					);
		} else if (!strcmp("folder", type)) {
			filters.emplace_back(
					new FSElementFilterPath(m_newSrcFolder + child->GetText(), isExcluded)
					);
		} else {
			std::cout << "unknown filter type" << std::endl;
		}
	}

	m_sourceFiles.addPath(m_newSrcFolder);
	m_sourceFiles.searchFiles(filters);


	return Status::returnStatusOkay();
}

const Status ShadowBuilder::retrieveTFAConf()
{
	tinyxml2::XMLElement* plugins = m_xmlTFACfg.FirstChildElement("plugins");
	std::vector<FSElementFilter *> filters = { new FSElementFilterExt(".so", false) };

	if (!plugins)
	{
		std::cout << "no sources provided" << std::endl;
		return Status::returnStatusError();
	}

	if (!plugins->FirstChildElement("plugin"))
	{
		m_tfaConf = TFAConfiguration(std::string(BASE_CFG_TFA_PLUGIN_DEFAULT_FOLDER), m_tfaPlat);
		return TFACore::initTFACore(m_tfaConf);
	}

	/** TODO support different path */
	for( const tinyxml2::XMLElement* child  = plugins->FirstChildElement("plugin");
			child; 
			child = child->NextSiblingElement())
	{
		m_tfaConf = TFAConfiguration(child->GetText(), m_tfaPlat);
		return TFACore::initTFACore(m_tfaConf);
	}

	return Status::returnStatusError();
}


