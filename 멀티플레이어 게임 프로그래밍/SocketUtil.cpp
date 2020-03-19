#include "SocketUtil.h"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily) {
	auto sock = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
	if (sock != INVALID_SOCKET)
		return UDPSocketPtr(new UDPSocket(sock));

	ReportError(L"SocketUtil::CreateUDPSocket");
	return nullptr;
}