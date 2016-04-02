#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

#define CLASS(name) \
class name; \
typedef shared_ptr<name> name##Ptr; \
typedef weak_ptr<name> name##WeakPtr;

#define STRUCT(name) \
struct name; \
typedef shared_ptr<name> name##Ptr; \
typedef weak_ptr<name> name##WeakPtr;
