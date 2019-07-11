#ifndef __COMMON_COMMIPC_H__
#define __COMMON_COMMIPC_H__

/** This element is in charge between of communication between the Shadow Bulder
 * and TFA-core 
 **/

#include <zmq.hpp>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef PORT_COMM_CONFIG
#define PORT_COMM_CONFIG 15200
#endif /* PORT_COMM_CONFIG */

#ifndef PORT_SB_TO_TFA_CORE
#define PORT_SB_TO_TFA_CORE 15200
#endif /* PORT_SB_TO_TFA_CORE */

#ifndef	CHANNEL_RX_COUNT
#define CHANNEL_RX_COUNT 1
#endif /* CHANNEL_RX_COUNT */

#ifndef	CHANNEL_TX_COUNT
#define CHANNEL_TX_COUNT 1
#endif /* CHANNEL_TX_COUNT */


#define CHANNEL_RX_INDEX	0
#define CHANNEL_TX_INDEX	(CHANNEL_RX_INDEX + CHANNEL_RX_COUNT)

#define CHANNEL_RX_TX_TOTAL_COUNT \
		(CHANNEL_TX_COUNT + CHANNEL_RX_COUNT)

class MessageIPC
{
public:
	MessageIPC(std::string& data, std::string& channel, std::string const& req = "ACK");

	std::string& getChannel() { return mChannel; }
	std::string& getData() { return mMsgIPC; }
	std::string& getResp() { return mMsgResp; }

protected:
	std::string mMsgIPC;
	std::string mMsgResp;
	std::string mChannel;
};

class CommIPC
{
public:
	CommIPC(unsigned short port, std::string const& addr);
	void CommIPCLock() { mLock.lock();}
	void CommIPCUnLock() { mLock.unlock();}
protected:
	std::shared_ptr<zmq::context_t> mCommIPCCtx;
	std::shared_ptr<zmq::socket_t> mCommIPCSocket;
	std::string mAddr;
	unsigned short mPort; 
	std::mutex mLock;
};

class CommIPCReq: public CommIPC
{
public:	
	CommIPCReq(unsigned short port);
	bool CommIPCSendRecv(std::string& msg);
};

class CommIPCRep: public CommIPC {
public:	
	CommIPCRep(unsigned short port);
	bool CommIPCRecv(std::string& msg, std::string& resp);
};

class CommIPCManager
{
public:
	static bool initIPCManager();
	static bool sendRecvMessage(MessageIPC& msg);
	static bool respMessage(MessageIPC& msg);

private:
	std::shared_ptr<CommIPC>& mFindInstanceSendRecv(std::string& channel);
	std::shared_ptr<CommIPC>& mFindInstanceRecv(std::string& channel);

	std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>>& 
		mCreateChannels(std::string& channel);

	std::unordered_map<std::string,
	       	std::shared_ptr<std::vector<std::shared_ptr<CommIPC>>>> mChannelIPCCommMap;

	/** Lock avoiding creation of same channel twice */
	std::mutex mChannelLock;

	static unsigned short mCurrentPort;
	static CommIPCManager *minstance;
};

#endif /* __COMMON_COMMIPC_H__ */
