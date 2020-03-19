#define WIN32_LEAN_AND_MEAN
#include "SocketUtil.h"
#include <ws2tcpip.h>
using namespace std;

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	auto udpSocket = SocketUtil::CreateUDPSocket(SocketAddressFamily::INET);
	SocketAddress socketAddress = SocketAddress(0, 0);
	udpSocket->Bind(socketAddress);
	auto data = "Hello";
	udpSocket->SendTo(data, sizeof(data), socketAddress);
	auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//문자열 초기화
	InetPton(AF_INET, (PCWSTR)"192.168.219.110", &addr.sin_addr);
	bind(sock, (sockaddr*)&addr, sizeof(addr));
	addrinfo* addrInfo;
	getaddrinfo("google.co.kr", "http", nullptr, &addrInfo);
	freeaddrinfo(addrInfo);
	shutdown(sock, SD_SEND);
	closesocket(sock);
	return 0;
}