#pragma once

#include "Game/NameObj/NameObj.hpp"



class TestNameObj: public NameObj {
public:
    TestNameObj(const char *);

    virtual ~TestNameObj();
    virtual void init(const JMapInfoIter &);
    s32 RandomStuff;
};