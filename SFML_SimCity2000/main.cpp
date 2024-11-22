#include "pch.h"


int main()
{
	Framework::GetInstance()->Initialize(1080, 920, "SimCity2000");
	Framework::GetInstance()->Do();
	Framework::GetInstance()->Release();
	return 0;
}

