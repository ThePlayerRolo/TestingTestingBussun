#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include <Game/MapObj/Coin.hpp>


class QuestionBlockDude: public LiveActor {
public:
    QuestionBlockDude(const char *);

    virtual ~QuestionBlockDude();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
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
    BenefitItemObj* mItem;
    Coin* mPurpleCoin;
    TVec3f mVel;
    bool mDeathToBlackHole;
    TQuat4f mRotationQuat;
    TVec3f  mRotationVec;
};