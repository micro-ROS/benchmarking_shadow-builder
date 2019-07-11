#include <CommIPC.h>
#include <functional>

class ConsoleCmds {
public:
	ConsoleCmds(std::function<int(int argc, char **argv)>& cb, std::string& cmd);
	std::string& getCmdString();
	std::function<int(int argc, char **argv)> getCallback();

private:
	std::string mCmdString;
	std::function<int(int argc, char **argv)> mFunctionCb;
};

class Console {
public:
	bool ConsoleAttach();
private:
	Console();

	bool ConsoleInit();
	bool ConsoleLoop();	

	static Console *mConsoleAction;
	static bool mConsoledAttached; 




};
