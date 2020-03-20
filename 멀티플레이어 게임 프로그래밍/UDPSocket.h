#pragma once
#include <iostream>
#include "SocketAddress.h"
using namespace std;

class UDPSocket {
public:
	~UDPSocket();
	int Bind(const SocketAddress& inToAddress);
	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
	int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);

	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) :socket(inSocket) {}
	SOCKET socket;
};

using UDPSocketPtr = shared_ptr<UDPSocket>;