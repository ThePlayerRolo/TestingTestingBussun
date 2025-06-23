#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"


class QuestionBlockDude: public LiveActor {
public:
    QuestionBlockDude(const char *);

    virtual ~QuestionBlockDude();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void exeWait();
    void exeDead();
    void exeWalk();

    bool mBodyCheck;
    f32 mBodyFrame;
    u32 mItemCase;
    s32 mPowerStarId;
    BenefitItemObj* mOneUp;
    BenefitItemLifeUp* mPowerMushroom;
};