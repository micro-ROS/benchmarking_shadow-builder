#include <string>
#include <common/Status.h>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

#include "clang/AST/RawCommentList.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/ADT/StringRef.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <algorithm>
#include <iostream>

#include <shadow-builder/CodeParser.h>
#include <tfa_pr/TFACore.h>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

ClangParser Parser::mparser;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory ClangBenchCategory("clang-benchmarking-tool options: -b path\n");
static Rewriter m_rewriter;

class FindCommentsConsumer : public clang::ASTConsumer
{
public:
	explicit FindCommentsConsumer(ASTContext *Context) {
        m_rewriter.setSourceMgr(Context->getSourceManager(), 
				Context->getLangOpts());
}

	virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		SourceManager& sm = Context.getSourceManager();
		std::string replacement;

		RawCommentList& commentList = Context.getRawCommentList();
		const auto& FileID = sm.getMainFileID();
		auto comments = commentList.getCommentsInFile(FileID);

		if (comments == nullptr) {
			return;
		}

		for (auto const& comment: *comments) {
			if (comment.second == nullptr)
				continue;

			std::string raw = comment.second->getRawText(sm);
			std::string brief = comment.second->getBriefText(Context);
			std::cout << raw << std::endl;

			if (TFACore::getReplacement(raw, replacement))
				continue;

			m_rewriter.ReplaceText(
					comment.second->getSourceRange(),
					StringRef(replacement));

			replacement = "";
		}

		m_rewriter.getEditBuffer(
				m_rewriter.getSourceMgr().getMainFileID()).write(errs());
		m_rewriter.overwriteChangedFiles();
	}
};

class FindCommentsAction : public clang::ASTFrontendAction 
{
public:
	virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
			clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
		return std::unique_ptr<clang::ASTConsumer>(
				new FindCommentsConsumer(&Compiler.getASTContext()));
	}
};

Status ClangParser::initAndStartCodeparser()
{
	const char *toolArgs[] = { "/home/amalki/prjs/microROS/soft/apps/pipeline-benchmarking/"
				   "build/src/build/shadow-builder/CoderParser_01_test" };
	const char *compArgs[] = { "--", "-nobuiltininc", "-nostdinc", "-nostdinc" , "-nodefaultlibs", "-nodefaultlibs",
       				"-nodefaultlibs"};

	/**TODO ARRAY_SIZE */
	int argc = m_sourcesPaths->size() + sizeof(toolArgs)/sizeof(toolArgs[0]) 
		+ sizeof(compArgs) / sizeof(compArgs[0]);

	std::cout << "Clang will be receiving " << argc << " arguments" << std::endl;
	const char **argV = (const char **) malloc(sizeof(char *) * (argc));

	for (unsigned int i = 0;  i < (sizeof(toolArgs)/sizeof(toolArgs[0])); i++)
	{
		argV[i] = toolArgs[i];
	}

	for (unsigned int i = 0;  i < (sizeof(compArgs)/sizeof(compArgs[0])) ; i++)
	{
		argV[argc - (sizeof(compArgs)/sizeof(compArgs[0])) + i] = compArgs[i];
	}

	for (unsigned int i = 0; i < m_sourcesPaths->size(); i++) {
		argV[i + sizeof(toolArgs)/sizeof(toolArgs[0])] = m_sourcesPaths->at(i)->getCurrentPath().c_str();
		std::cout <<  "Parser::Getting " <<  argV[i + 1] << std::endl;
	}

#if 0
	for (unsigned int i = 0 ; i < argc; i++) {
		std::cout << "argv[" << i <<"] = " << argV[i] << std::endl;
	}
#endif
	
	CommonOptionsParser OptionsParser(argc, argV, ClangBenchCategory);
	/** Todo get compilation option automatically and source file from
	 * File vector */

	ClangTool m_clangTool(OptionsParser.getCompilations(),
		       OptionsParser.getSourcePathList());

	std::cout << "Going to parser the file" << std::endl;	
	for (File *file: *m_sourcesPaths)
	{
		std::cout << file->getCurrentPath() << std::endl;
	}

	m_clangTool.run(newFrontendActionFactory<FindCommentsAction>().get());
	/* Finished!!!! shall we compile? */

	free(argV);
	return Status::returnStatusOkay();
}
