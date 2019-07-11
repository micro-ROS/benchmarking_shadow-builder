#include <common/CommIPC.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <unistd.h>

bool initComm ()
{
	return CommIPCManager::initIPCManager();
}

void *client(unsigned int id, std::mutex& std_lock)
{
	std::string mychannel("channel1");
	unsigned int i = 0;

	while (true) {
		std::string mydata("[From threadid " + std::to_string(id) +
			       	"] Hello you x");
		MessageIPC msg(mydata, mychannel);
		if (!CommIPCManager::sendRecvMessage(msg)) {
			std::cout << "Error while sending" << std::endl;
			return NULL;
		}

		std_lock.lock();
		std::cout << "Success receiving response msg:" << msg.getData() << std::endl;
		std_lock.unlock();
	}


		return NULL;
}

void *serv(unsigned int id, std::mutex& std_lock)
{
	std::string mydata;
	std::string mychannel("channel1");
	std::string myresp("[thread_id = " + std::to_string(id) + "] Hello to you too");

	MessageIPC msg(mydata, mychannel, myresp);

	if (!CommIPCManager::respMessage(msg)) {
		std_lock.lock();
		std::cout << "Error while receiving" << std::endl;
		std_lock.unlock();
		return NULL;
	}

	std_lock.lock();
	std::cout << "Success receiving msg:" << msg.getData() << std::endl;
	std_lock.unlock();

	return NULL;
}


int main(int argc, char **argv)
{
	std::vector<std::thread> client_v;
	std::vector<std::thread> server_v;
	std::mutex stdout_lock;
	std::mutex access;

	if (!initComm()) {
		std::cout << "ERROR Initializing" << std::endl;
	}


	for (unsigned int i = 0; i < 10	; i++)
		server_v.push_back(std::move(std::thread(serv, i, std::ref(stdout_lock))));
                                       	
	for (unsigned int i = 0; i < 1	; i++)
		client_v.push_back(std::move(std::thread(client, i, std::ref(stdout_lock))));
                                       	
                                       
                                       	
	std::vector<std::thread> moved_client_v = std::move(client_v);
	for (std::thread & th: moved_client_v) {
		th.join();
	}

	std::vector<std::thread> moved_server_v = std::move(server_v);
	for (std::thread & th: moved_server_v) {
		th.join();
	}

	return 0;
}
