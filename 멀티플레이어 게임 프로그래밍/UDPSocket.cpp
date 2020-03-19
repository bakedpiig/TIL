#include "UDPSocket.h"
#include "SocketUtil.h"

int UDPSocket::Bind(const SocketAddress& inBindAddress) {
	int error = bind(socket, &inBindAddress.sockAddr, inBindAddress.GetSize());
	if (error == 0)
		return NO_ERROR;

	SocketUtil::ReportError(L"UDPSocket::Bind");
	return SocketUtil::GetLastError();
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo) {
	int byteSentCount = sendto(socket, static_cast<const char*>(inData), inLen, 0, &inTo.sockAddr, inTo.GetSize());
	if (byteSentCount >= 0)
		return byteSentCount;

	SocketUtil::ReportError(L"UDPSocket::SendTo");
	return SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inMaxLength, SocketAddress& outFrom) {
	int fromLength = outFrom.GetSize();
	int readByteCount = recvfrom(socket, static_cast<char*>(inBuffer), inMaxLength, 0, &outFrom.sockAddr, &fromLength);
	if (readByteCount >= 0)
		return readByteCount;

	SocketUtil::ReportError(L"UDPSocket::ReceiveFrom");
	return -SocketUtil::GetLastError();
}

UDPSocket::~UDPSocket() {
	closesocket(socket);
}