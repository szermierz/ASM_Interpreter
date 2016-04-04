#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

#define CLASS(name) \
class name; \
typedef shared_ptr<name> name##Ptr; \
typedef weak_ptr<name> name##WeakPtr;

#define STRUCT(name) \
struct name; \
typedef shared_ptr<name> name##Ptr; \
typedef weak_ptr<name> name##WeakPtr;

namespace math
{
	template<typename T>
	T clampCeil(T x, T max) // Clamps x to [x, max) 
	{
		if (x >= max)
			return max - 1;

		return x;
	}
}
