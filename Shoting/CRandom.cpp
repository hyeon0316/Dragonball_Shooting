#include"CRandom.h"

CRandom::CRandom() {
	sowseed();
}

CRandom::~CRandom() {
}

void CRandom::sowseed() {
	srand(timeGetTime());
	m_nCount = 0;
}

int CRandom::number(int m, int n) {
	if (++m_nCount > 2000)
		sowseed();

	int sample;

	if (n < 0x7FFF) //16Áø¼ö
		sample = rand();
	else
		sample = ((int)rand() << 15 | (int)rand());

	return (sample % (n - m + 1) + m);
}
