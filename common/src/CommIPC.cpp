#include <common/CommIPC.h>
#include <common/zHelpers.hpp>

#include <exception>
#include <memory>
#include <iostream>


#define IPCCOMREQ_ADDR "tcp://localhost:"
#define IPCCOMREP_ADDR "tcp://*:"

CommIPCManager *CommIPCManager::minstance = NULL;
unsigned short CommIPCManager::mCurrentPort = 0;

MessageIPC::MessageIPC(std::string& data, std::string& channel, std::string const& req):
       	mMsgIPC(data), mChannel(channel), mMsgResp(req)
{
}

CommIPC::CommIPC(unsigned short port, std::string const& addr): mPort(port)
{
	// TODO: Feature: Added configuration using configuration file 
	mAddr = addr + std::to_string(port);
	std::cout << "Created address to " << mAddr << std::endl;
        mCommIPCCtx = std::make_shared<zmq::context_t>(1);

	if (mCommIPCCtx == nullptr) {
		throw std::runtime_error("Could not create IPC Ctx\n");
	}
}

CommIPCRep::CommIPCRep(unsigned short port): CommIPC(port, IPCCOMREP_ADDR)
{
	mCommIPCSocket = std::make_shared<zmq::socket_t>(*mCommIPCCtx, ZMQ_REP);

	if (mCommIPCSocket == nullptr) {
		throw std::runtime_error("Could not create request Socket\n");
	}

        std::cout << "Going to bind to socket "  << mAddr << std::endl;
        mCommIPCSocket->bind(mAddr);
        std::cout << "Binded to " << mAddr << std::endl;
}

bool CommIPCRep::CommIPCRecv(std::string& msg , std::string& resp)
{
	static unsigned int i = 0;
	std::cout << "Going to receive" << std::endl;
	msg = s_recv(*mCommIPCSocket);

	i++;
	if (!s_send(*mCommIPCSocket, resp)) {
		std::cout << "Could not send to socket addr " << mAddr << std::endl;
		return false;
	}

	return true;
}

CommIPCReq::CommIPCReq(unsigned short port): CommIPC(port, IPCCOMREQ_ADDR)
{
	mCommIPCSocket = std::make_shared<zmq::socket_t>(*mCommIPCCtx, ZMQ_REQ);

	if (mCommIPCSocket == nullptr) {
		throw std::runtime_error("Could not create request Socket\n");
	}
}

bool CommIPCReq::CommIPCSendRecv(std::string& msg)
{
	mCommIPCSocket->connect(mAddr);
        std::cout << "Connected to " << mAddr << std::endl;

	if (!s_send(*mCommIPCSocket, msg)) {
		std::cout << "Could not send to socket addr " << mAddr << std::endl;
		return false;
	}

	msg = s_recv(*mCommIPCSocket);
	return true;
}

bool CommIPCManager::initIPCManager()
{
	if (minstance) {
		std::cout << "IPC Manager already instantiated" << std::endl;
		return false;
	}

	minstance = new CommIPCManager;
	mCurrentPort = PORT_SB_TO_TFA_CORE;

	/* Fatal Error */
	if (minstance == nullptr) {
		throw std::runtime_error("Could not create IPC Manager\n");
	}

	return true;
}

bool CommIPCManager::sendRecvMessage(MessageIPC& msg)
{
	minstance->mChannelLock.lock();
	CommIPC *comIPC = minstance->mFindInstanceSendRecv(msg.getChannel()).get();
	CommIPCReq *comIPCReq =  static_cast<CommIPCReq *>(comIPC);
	minstance->mChannelLock.unlock();

	comIPC->CommIPCLock();
	if (!comIPCReq->CommIPCSendRecv(msg.getData())) {
		std::cout << "Error while sending message on channel" << std::endl;
	}
	comIPC->CommIPCUnLock();

	return true;
}

bool CommIPCManager::respMessage(MessageIPC& msg)
{
	minstance->mChannelLock.lock();
	CommIPC *comIPC = minstance->mFindInstanceRecv(msg.getChannel()).get();
	CommIPCRep *comIPCRep =	static_cast<CommIPCRep *>(comIPC);
	minstance->mChannelLock.unlock();

	comIPC->CommIPCLock();
	if (!comIPCRep->CommIPCRecv(msg.getData(), msg.getResp())) {
		std::cout << "Error while receiving message on channel" << std::endl;
	}
	comIPC->CommIPCUnLock();

	return true;
}

std::shared_ptr<CommIPC>& CommIPCManager::mFindInstanceRecv(std::string& channel)
{
	std::unordered_map<std::string,
	       	std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>>>::iterator it = 
		mChannelIPCCommMap.find(channel);

	if (it == mChannelIPCCommMap.end()) {
		std::cout << "Channel: " << channel << " not found" << std::endl;
		return (mCreateChannels(channel)->at(CHANNEL_RX_INDEX));
	}

	return (it->second->at(CHANNEL_RX_INDEX));
}

std::shared_ptr<CommIPC>& CommIPCManager::mFindInstanceSendRecv(std::string& channel)
{
	std::unordered_map<std::string,
		std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>>>::iterator it = 
		mChannelIPCCommMap.find(channel);

	if (it == mChannelIPCCommMap.end()) {
		std::cout << "Channel: " << channel << " not found" << std::endl;
		return (mCreateChannels(channel)->at(CHANNEL_TX_INDEX));
	} else {
		std::cout << "Found Channel" << channel << std::endl;
	}

	return (it->second->at(CHANNEL_TX_INDEX));
}

std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>>&
CommIPCManager::mCreateChannels(std::string& channel)
{
	std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>> 
		comPtr = std::make_shared<std::vector<std::shared_ptr<CommIPC>>>();

	std::cout << "Creating Channel: " << channel << std::endl;
	for (unsigned int i = 0; i < CHANNEL_RX_COUNT; i++) {
       		comPtr->push_back(std::make_shared<CommIPCRep>(mCurrentPort));
	}

	for (unsigned int i = CHANNEL_RX_COUNT;
			i < (CHANNEL_RX_COUNT + CHANNEL_TX_COUNT); i++) {
       		comPtr->push_back(std::make_shared<CommIPCReq>(mCurrentPort));
	}

	mChannelIPCCommMap.insert({{channel, comPtr}});

	std::cout << "Channel " << channel << " created" << std::endl;
	mCurrentPort++;

	return mChannelIPCCommMap[channel];
}
