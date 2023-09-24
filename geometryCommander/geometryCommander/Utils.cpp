#include "Utils.h"

Utils* Utils::GetInstance()
{
	if (instance = nullptr) {
		instance = new Utils();
	}

	return instance;
}

Utils::~Utils()
{
	delete instance;
}
