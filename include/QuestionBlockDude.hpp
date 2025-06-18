#pragma once

#include "Game/LiveActor/LiveActor.hpp"



class QuestionBlockDude: public LiveActor {
public:
    QuestionBlockDude(const char *);

    virtual ~QuestionBlockDude();
    virtual void init(const JMapInfoIter &);
    s32 RandomStuff;
};