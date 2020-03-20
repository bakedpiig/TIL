#include "SocketUtil.h"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily) {
	auto sock = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
	if (sock != INVALID_SOCKET)
		return UDPSocketPtr(new UDPSocket(sock));

	ReportError(L"SocketUtil::CreateUDPSocket");
	return nullptr;
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily) {
	auto sock = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
	if (sock != INVALID_SOCKET)
		return TCPSocketPtr(new TCPSocket(sock));

	ReportError(L"SocketUtil::CreateTCPSocket");
	return nullptr;
}

int SocketUtil::GetLastError() {
	return WSAGetLastError();
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const vector<TCPSocketPtr>* inSockets) {
	if (inSockets) {
		FD_ZERO(&outSet);
		for (const TCPSocketPtr& socket : *inSockets)
			FD_SET(socket->socket, &outSet);
		return &outSet;
	}
	return nullptr;
}

void SocketUtil::FillVectorFromSet(vector<TCPSocketPtr>* outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inSet) {
	if (inSockets && outSockets) {
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets) {
			if (FD_ISSET(socket->socket, &inSet))
				outSockets->push_back(socket);
		}
	}
}

int SocketUtil::Select(const vector<TCPSocketPtr>* inReadSet, vector<TCPSocketPtr>* outReadSet,
	const vector<TCPSocketPtr>* inWriteSet, vector<TCPSocketPtr>* outWriteSet,
	const vector<TCPSocketPtr>* inExceptSet, vector<TCPSocketPtr>* outExceptSet) {
	fd_set read, write, except;
	fd_set* readPtr = FillSetFromVector(read, inReadSet);
	fd_set* writePtr = FillSetFromVector(write, inWriteSet);
	fd_set* exceptPtr = FillSetFromVector(except, inExceptSet);
	int toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);

	if (toRet > 0) {
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, write);
		FillVectorFromSet(outExceptSet, inExceptSet, except);
	}

	return toRet;
}