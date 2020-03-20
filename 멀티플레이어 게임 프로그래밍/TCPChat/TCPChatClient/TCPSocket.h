#pragma once
#include "SocketAddress.h"

class TCPSocket;
using TCPSocketPtr = shared_ptr<TCPSocket>;

class TCPSocket {
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog = 32);
	TCPSocketPtr Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);
	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : socket(inSocket) {}
	SOCKET socket;
};