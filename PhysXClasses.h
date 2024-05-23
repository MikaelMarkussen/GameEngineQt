#ifndef PHYSXCLASSES_H
#define PHYSXCLASSES_H

#include <PxPhysicsAPI.h>
#include <foundation/PxFoundationConfig.h>
#include <iostream>
#include "logger.h"

class PxAllocatorCallback
{
public:
    virtual ~PxAllocatorCallback() {}
    virtual void* allocate(size_t size, const char* typeName, const char* filename,
        int line) = 0;
    virtual void deallocate(void* ptr) = 0;
};



class UserErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
        int line)
    {
        std::cout << "ERROR:" << code << " :" << message;
    }
};

#endif // PHYSXCLASSES_H
