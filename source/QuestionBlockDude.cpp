#include "QuestionBlockDude.hpp"
#include <Game/Util/StringUtil.hpp>
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"


QuestionBlockDude::QuestionBlockDude(const char * pName) : LiveActor(pName) {
    mBodyCheck = false;
    mBodyFrame = 0.0f;
    mItemCase = 0;
    mPowerStarId = 0;
    mOneUp = nullptr;
    mPowerMushroom = nullptr;
}

QuestionBlockDude::~QuestionBlockDude() {}

/*
    mBodyFrame/mItemCase values:
    0: Starbits
    1: Coins
    2: 1Up
    3: PowerMushrooms
    */

void QuestionBlockDude::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this,rIter);
    initModelManagerWithAnm("QuestionBlockDude", nullptr, false);
    initHitSensor(1);
    MR::connectToSceneMapObjStrongLight(this);
    MR::addHitSensorEnemy(this, "body", 0x10, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::validateHitSensors(this);
    MR::getJMapInfoArg0NoInit(rIter, &mBodyCheck);
    MR::getJMapInfoArg1NoInit(rIter, &mBodyFrame);
    MR::getJMapInfoArg2NoInit(rIter, &mPowerStarId);
    if (mBodyCheck) {
        MR::startBtpAndSetFrameAndStop(this, "BodyChanging1", mBodyFrame);
    }//Todo: Add BodyChanging2

    mItemCase = mBodyFrame;
    switch (mItemCase) {
        case 0:
        MR::declareStarPiece(this, 3);
        break;
        case 1:
        MR::declareCoin(this, 1);
        break;
        case 2:
        mOneUp = MR::createKinokoOneUp();
        MR::setShadowDropLength(mOneUp, nullptr, 500.0f);
        break;
        case 3:
        mPowerMushroom = MR::createKinokoSuper();
        MR::setShadowDropLength(mPowerMushroom, nullptr, 500.0f);
        break;
        case 4: //Todo: Test This, yeah.
        MR::declarePowerStar(this, mPowerStarId);
        break;
    }

    makeActorAppeared();
    
    MR::startBck(this, "Walk", nullptr);
}

void QuestionBlockDude::kill() {
    
}

void QuestionBlockDude::exeWait() {

}
void QuestionBlockDude::exeWalk() {
    
}




bool QuestionBlockDude::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReciever) {
    if (MR::isSensorPlayer(pSender)) {
        OSReport("recievePlayerAttack!");
        return true;
    }
}

bool QuestionBlockDude::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReciever) {
    if (MR::isSensorPlayer(pReciever)) {
        OSReport("recieveEnemyAttack!");
        return true;
    }
}

/*
Use MR::useStageSwitchWriteA/B/Dead in init
And then later on, use MR::onSwitchA/B/Dead
Then simply link that switch with an SW_APPEAR of another object
for hitsensors use initHitSensor, MR::addHitSenser and overload the existing LiveActor functions for hitsensor collision

first one: HitSensor* pSender, HitSensor* pReceiver

bottom 2: u32 msg, HitSensor* pSender, HitSensor* pReceiver
Difference Between player and enemy
is Caller.

*/