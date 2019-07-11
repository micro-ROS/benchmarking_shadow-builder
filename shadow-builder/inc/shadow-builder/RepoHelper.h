#ifndef __REPOHELPER_H__
#define __REPOHELPER_H__

#include <git2.h>
#include <common/Status.h>
#include <iostream>

#define DEFAULT_TMP_PATH ""

class IRepo 
{
public:
	IRepo(): m_URL(""), m_Reference(""), m_Destination("") { }
	virtual ~IRepo() {} 

	virtual Status setRepoReference() { return Status::returnStatusOkay(); } 
	virtual Status setRepoDest(const string& dest) = 0;
	virtual Status setRepoURL(const string& ref) = 0;
	virtual Status repoRetrieve() = 0;

protected:
	std::string m_URL;
	std::string m_Reference;
	std::string m_Destination;
};


class GitRepo: public IRepo {
public:
	GitRepo(): IRepo() { }
	Status setRepoDest(const string& dest) {
	       					m_Destination = DEFAULT_TMP_PATH +  dest;
						return Status::returnStatusOkay();
       					}

	Status setRepoReference(const string& ref) {
						m_Reference = ref;
						return Status::returnStatusOkay();
					}
	Status setRepoURL(const string& url) { 
						m_URL = url;
						return Status::returnStatusOkay();
					}

	Status repoRetrieve() {
		Status st;
		git_repository *repo = NULL;
		git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
		git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
  		checkout_opts.checkout_strategy= GIT_CHECKOUT_FORCE;
		int error;

		if ((st = mInitLibGit())) {
			goto shutdown_libgit2;
		}

		clone_opts.checkout_branch = m_Reference.c_str();
		error = git_clone(&repo, m_URL.c_str(), m_Destination.c_str(), &clone_opts);
		if (error) {
			st = Status::ERROR;
			st = string("Could not clone the repo " + m_URL + " into " + 
					m_Destination);
			goto shutdown_libgit2;
		}

	shutdown_libgit2:
		git_libgit2_shutdown();
		return st;
	}


private:
	Status mInitLibGit() {
		Status st;
		const git_error *err;

		if (true == m_IsLibGitInitialized) {
			st = Status::ERROR;
			st = string("Library already initialized\n");
			return st;
		}

		git_libgit2_init();
		m_IsLibGitInitialized = true;

		return st;
	}

	bool m_isRepoInit;
	static bool m_IsLibGitInitialized;
};

bool GitRepo::m_IsLibGitInitialized = false;

#endif /* __GITHELPER_H__ */
