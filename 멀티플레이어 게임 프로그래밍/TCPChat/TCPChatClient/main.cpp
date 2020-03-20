#include "SocketUtil.h"
#include <string>

int main() {
	WSADATA wsaData;
	if (!WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		auto socket = SocketUtil::CreateTCPSocket(INET);
		SocketAddress socketAddress = SocketAddress(INADDR_ANY, 0);
		socket->Bind(socketAddress);
		socket->SetNonBlockingMode(true);

		auto serverAddrPtr = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:48000");
		int connected = -1;
		connected = socket->Connect(*serverAddrPtr);

		while (true) {
			string data;
			getline(cin, data);

			int sendSize = socket->Send(data.c_str(), sizeof(data));
			if (sendSize != sizeof(data)) {
				cout << "Send Error" << endl;
				WSACleanup();
				return 0;

			}

			char receiveData[1500];
			int receivedSize = -1;
			receivedSize = socket->Receive(receiveData, sizeof(receiveData));
			if(receivedSize > 0)
				cout << receiveData << endl;
		}

		WSACleanup();
	}

	return 0;
}