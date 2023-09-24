#pragma once
#include <map>

class Utils
{
private:
		Utils();
		static Utils* instance;

public:
	static Utils* GetInstance();
	~Utils();
};

