#include "SocketUtil.h".

int main() {
	WSADATA wsaData;
	if (!WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		SocketAddress socketAddress = SocketAddress(INADDR_ANY, 48000);
		auto listenSocket = SocketUtil::CreateTCPSocket(INET);
		listenSocket->Bind(socketAddress);
		listenSocket->Listen();
		vector<TCPSocketPtr> readBlockSockets;
		readBlockSockets.push_back(listenSocket);
		vector<TCPSocketPtr> readableSockets;

		while (true) {
			if (!SocketUtil::Select(&readBlockSockets, &readableSockets, nullptr, nullptr, nullptr, nullptr))
				continue;

			for (const TCPSocketPtr& socket : readableSockets) {
				if (socket == listenSocket) {
					SocketAddress outAddress;
					auto connectSocket = listenSocket->Accept(outAddress);
					readBlockSockets.push_back(connectSocket);
					cout << "new socket" << endl;
				}
				else {
					char receiveData[1500];
					int receivedSize = socket->Receive(receiveData, sizeof(receiveData));
					if (receivedSize > 0)
						cout << receiveData << endl;
				}
			}
		}
		WSACleanup();
	}
	return 0;
}