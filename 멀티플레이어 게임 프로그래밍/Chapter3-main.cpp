#define WIN32_LEAN_AND_MEAN
#include "SocketUtil.h"
#include "RoboCat.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include <ws2tcpip.h>
using namespace std;

void SendRoboCat(int inSocket, const RoboCat* inRoboCat) {
	OutputMemoryStream stream;
	inRoboCat->Write(stream);
	send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0);
}

const uint32_t kMaxPacketSize = 1478;

void ReceiveRoboCat(int inSocket, RoboCat* outRoboCat) {
	char* temporaryBuffer = static_cast<char*>(malloc(kMaxPacketSize));
	size_t receivedByteCount = recv(inSocket, temporaryBuffer, kMaxPacketSize, 0);

	if (receivedByteCount > 0) {
		InputMemoryStream stream(temporaryBuffer, static_cast<uint32_t>(receivedByteCount));
		outRoboCat->Read(stream);
	}
	else
		free(temporaryBuffer);
}

constexpr int GOOD_SEGMENT_SIZE = 1500;
bool isGameRunning;

void DoGameLoop() {
	auto sock = SocketUtil::CreateUDPSocket(INET);
	sock->SetNonBlockingMode(true);

	while (isGameRunning) {
		char data[1500];
		SocketAddress socketAddress;

		int bytesReceived = sock->ReceiveFrom(data, sizeof(data), socketAddress);
		if (bytesReceived > 0)
			ProcessReceivedData(data, bytesReceived, socketAddress);

		DoGameFrame();
	}
}

void DoTCPLoop() {
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 48000);
	if (listenSocket->Bind(receivingAddress) != NO_ERROR)
		return;

	vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	vector<TCPSocketPtr> readableSockets;

	while (isGameRunning) {
		if (!SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr))
			continue;

		for (const TCPSocketPtr& socket : readableSockets) {
			if (socket == listenSocket)
			{
				SocketAddress newClientAddress;
				auto newSocket = listenSocket->Accept(newClientAddress);
				readBlockSockets.push_back(newSocket);
				ProcessNewClient(newSocket, newClientAddress);
			}
			else {
				char segment[GOOD_SEGMENT_SIZE];
				int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
				if (dataReceived > 0)
					ProcessDataFromClient(socket, segment, dataReceived);
			}
		}
	}
}

void ProcessReceivedData(char* data, int dataSize, SocketAddress& socketAddress);
void DoGameFrame();
void ProcessNewClient(TCPSocketPtr socket, SocketAddress& client);
void ProcessDataFromClient(TCPSocketPtr socket, const char* segment, int size);

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