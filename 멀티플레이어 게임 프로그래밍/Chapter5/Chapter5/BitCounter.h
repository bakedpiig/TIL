constexpr int GetRequiredBits(int value, int bits = 0) {
	return value ? GetRequiredBits(value >> 1, bits + 1) : bits;
}