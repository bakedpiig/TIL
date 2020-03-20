#pragma once
#include "TCPSocket.h"
#include <stack>
#include <vector>
using namespace std;

enum SocketAddressFamily {
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil {
public:
	static void ReportError(const wchar_t*);
	static int GetLastError();
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
	static int Select(const vector<TCPSocketPtr>* inReadSet, vector<TCPSocketPtr>* outReadSet,
		const vector<TCPSocketPtr>* inWriteSet, vector<TCPSocketPtr>* outWriteSet,
		const vector<TCPSocketPtr>* inExceptSet, vector<TCPSocketPtr>* outExceptSet);
private:
	static fd_set* FillSetFromVector(fd_set& outSet, const vector<TCPSocketPtr>* inSockets);
	static void FillVectorFromSet(vector<TCPSocketPtr>* outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inset);
	static stack<const wchar_t*> reportLog;
};