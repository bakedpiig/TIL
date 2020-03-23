#include "OutputMemoryBitStream.h"

int main() {
	OutputMemoryBitStream mbs;
	mbs.WriteBits(13, 5);
	mbs.WriteBits(52, 6);
}