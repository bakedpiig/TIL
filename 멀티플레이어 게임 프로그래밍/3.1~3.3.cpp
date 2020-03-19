#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

//sockaddr의 자료형 안전성을 위한 클래스
class SocketAddress {
private:
	sockaddr sockAddr;
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort) {
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress(const sockaddr& inSockAddr) {
		memcpy(&sockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t getSize() const { return sizeof(sockaddr); }

private:
	sockaddr_in* GetAsSockAddrIn() {
		return reinterpret_cast<sockaddr_in*>(&sockAddr);
	}
};

//소켓 주소 공유할 때 메모리 정리 필요 없음
using SocketAddressPtr = shared_ptr<SocketAddress>;


class SocketAddressFactory {
public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString) {
		auto pos = inString.find_last_of(':');
		string host, service;
		if (pos != string::npos) {
			host = inString.substr(0, pos);
			service = inString.substr(pos + 1);
		}
		else {
			host = inString;
			service = "0";
		}

		addrinfo hint;
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;

		addrinfo* result = nullptr;
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
		addrinfo* initResult = result;

		if (error != 0 && result != nullptr) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		while (!result->ai_addr && result->ai_next) {
			result = result->ai_next;
		}

		if (!result->ai_addr) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

		freeaddrinfo(initResult);
		return toRet;
	}
};

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
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