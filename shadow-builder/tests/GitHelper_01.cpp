#include <shadow-builder/RepoHelper.h>
#include <common/Status.h>

int main(int argc, char **argv)
{
	GitRepo repo;
	Status st;
	repo.setRepoDest("/tmp/test/");
	repo.setRepoReference("br2");
	repo.setRepoURL("https://github.com/thockin/test.git");

	st = repo.repoRetrieve();

	if (st != Status::SUCCESS) {
		std::cout << st << std::endl;
	}

	std::cout << st << std::endl;
	return 0;
}
