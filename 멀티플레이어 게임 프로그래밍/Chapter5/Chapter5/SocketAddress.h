#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
using namespace std;

class SocketAddress {
private:
	sockaddr sockAddr;
public:
	SocketAddress() { }
	SocketAddress(ADDRESS_FAMILY inFamily, uint32_t inAddress, uint16_t inPort) {
		GetAsSockAddrIn()->sin_family = inFamily;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress(const sockaddr& inSockAddr) {
		memcpy(&sockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	friend class UDPSocket;
	friend class TCPSocket;
	sockaddr_in* GetAsSockAddrIn() {
		if (sockAddr.sa_family == AF_INET)
			return reinterpret_cast<sockaddr_in*>(&sockAddr);
		else
			return nullptr;
	}
};

using SocketAddressPtr = shared_ptr<SocketAddress>;