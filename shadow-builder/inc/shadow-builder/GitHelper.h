#ifndef __REPOHELPER_H__
#define __REPOHELPER_H__

#include <libgit2.h>

#define DEFAULT_TMP_PATH "/tmp/"

class IRepo 
{
public:
	virtual Status setRepoReference() { return Status::returnStatusOk() } 
	virtual Status setRepoDest(const string& dest) = 0;
	virtual Status setRepoURL(const string& ref) = 0;
	virtual Status setRepo(const string& url) = 0;

protected:
	std::string m_URL;
	std::string m_Reference;
	std::string m_Destination;
}


class GitRepo: public IRepo {
public:
	GitRepo(): mURL(""), mReference(""), mDestination("") { }

	Status setRepoDest(const string& dest) {
	       					m_Destination = DEFAULT_TMP_PATH +  dest;
						return Status::returnStatusOk();
       					}

	Status setRepoReference(const string& ref) {
						m_Reference = DEFAULT_TMP_PATH + ref;
						return Status::returnStatusOk();
					}

	Status setRepoURL(const string& url) { 
						m_URL = url;
						return Status::returnStatusOk();
					}

	Status repoRetrieve() {
		Status st;
		git_repository *repo = NULL;
		git_object *treeish = NULL;
		int error;
		git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
		opts.checkout_strategy = GIT_CHECKOUT_SAFE;

		if ((st = minitLibGit())) {
			return st;
		}

		error = git_clone(&repo, url.c_str(), path.c_str(), NULL);
		if (error) {
			st = string("Could not clone the repo " + url + " into " + 
					path);
			return st;
		}

		/* Checkout from HEAD, something like `git checkout HEAD` */
		error = git_checkout_head(repo, &opts);	
		if (error) {
			st = string("Not head ref for " + url + " into ");
			return st;
		}

		error = git_oid_fromstr(&oid, sha);
		if (!error) {

		} else {
			error = git_revparse(&treeish, repo, m_Reference.c_str());
			if (error) {
				st = string("Could not get ref " + m_Reference + " for " +
						m_URL);
				return st;
			}

			error = git_checkout_tree(repo, treeish, &opts);
			if (error) {
				st = string("Could not checkout ref " + m_Reference 
						+ " for " +  m_URL);
				return st;
			}
		}

		return st;
	}


private
	static Status minitLibGit() {
		if (m_IsLibGitInitialized) {
			if (git_libgit2_init();) 
		}
	}

	static bool m_IsLibGitInitialized;
};

GitRepo::m_IsLibGitInitialized = false

#endif /* __GITHELPER_H__ */
