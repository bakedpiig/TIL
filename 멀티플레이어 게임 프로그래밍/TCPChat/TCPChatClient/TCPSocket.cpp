#include "TCPSocket.h"
#include "SocketUtil.h"

int TCPSocket::Connect(const SocketAddress& inAddress) {
	int error = connect(socket, &inAddress.sockAddr, inAddress.GetSize());
	if (error >= 0)
		return NO_ERROR;

	SocketUtil::ReportError(L"TCPSocket::Connect");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Bind(const SocketAddress& inToAddress) {
	int error = bind(socket, &inToAddress.sockAddr, inToAddress.GetSize());
	if (error >= 0)
		return NO_ERROR;

	SocketUtil::ReportError(L"TCPSocket::Bind");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Listen(int inBackLog) {
	int error = listen(socket, inBackLog);
	if (error >= 0)
		return NO_ERROR;

	SocketUtil::ReportError(L"TCPSocket::Listen");
	return -SocketUtil::GetLastError();
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress) {
	int length = inFromAddress.GetSize();
	auto newSocket = accept(socket, &inFromAddress.sockAddr, &length);

	if (newSocket != INVALID_SOCKET)
		return TCPSocketPtr(new TCPSocket(newSocket));

	SocketUtil::ReportError(L"TCPSocket::Accept");
	return nullptr;
}

int TCPSocket::Send(const void* inData, int inLen) {
	int bytesSentCount = send(socket, static_cast<const char*>(inData), inLen, 0);

	if (bytesSentCount >= 0)
		return bytesSentCount;

	SocketUtil::ReportError(L"TCPSocket::Send");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Receive(void* inData, int inLen) {
	int bytesReceivedCount = recv(socket, static_cast<char*>(inData), inLen, 0);

	if (bytesReceivedCount >= 0)
		return bytesReceivedCount;

	SocketUtil::ReportError(L"TCPSocket::Receive");
	return -SocketUtil::GetLastError();
}

int TCPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking) {
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(socket, FIONBIO, &arg);

	if (result != SOCKET_ERROR)
		return NO_ERROR;

	SocketUtil::ReportError(L"TCPSocket::SetNonBlockingMode");
	return SocketUtil::GetLastError();
}

TCPSocket::~TCPSocket() {
	closesocket(socket);
}