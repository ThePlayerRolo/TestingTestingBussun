#include "QuestionBlockDude.hpp"
#include <Game/Util/StringUtil.hpp>
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"


namespace NrvQuestionBlockDude {
    NEW_NERVE(NrvQuestionBlockDudeWalk, QuestionBlockDude, Walk);
    NEW_NERVE(NrvQuestionBlockDudeWait, QuestionBlockDude, Wait);
    NEW_NERVE(NrvQuestionBlockDudeDead, QuestionBlockDude, Dead);
}


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
    initHitSensor(2);
    initSound(4, false);
    MR::connectToSceneMapObjStrongLight(this);
    MR::addHitSensorEnemy(this, "body", 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
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
    initNerve(&NrvQuestionBlockDude::NrvQuestionBlockDudeWait::sInstance);
    makeActorAppeared();
}

void QuestionBlockDude::kill() {
    switch (mItemCase) {
        case 0:
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        break;
        case 1:
        MR::appearCoinPop(this, mPosition, 1);
        break;
        case 2:
        MR::appearKinokoOneUpPop(mOneUp, getBaseMtx(), 15.0f);
        break;
        case 3:
        MR::appearKinokoSuper(mPowerMushroom, getBaseMtx(), 15.0f);
        break;
        case 4:
        MR::requestAppearPowerStar(this, mPowerStarId, mPosition);
        break;
    }
    LiveActor::kill();
}

void QuestionBlockDude::exeWait() {
    if (MR::isFirstStep(this)) {
        setNerve(&NrvQuestionBlockDude::NrvQuestionBlockDudeWalk::sInstance);
    }
}
void QuestionBlockDude::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
    }
}

void QuestionBlockDude::exeDead() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::startBck(this, "Die", nullptr);
    }
    if (!MR::isBckPlaying(this, "Die")) {
        kill();
    }
}
void QuestionBlockDude::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {

    if(MR::isSensorPlayerOrRide(pReceiver) && MR::isSensorEnemyAttack(pSender)){
        MR::sendMsgEnemyAttack(pReceiver,pSender);
    }else{
        MR::sendMsgPush(pReceiver,pSender);
    }
}


bool QuestionBlockDude::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {

}

bool QuestionBlockDude::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        setNerve(&NrvQuestionBlockDude::NrvQuestionBlockDudeDead::sInstance);
        return true;
    }
}

bool QuestionBlockDude::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        OSReport("recieveEnemyAttack!\n");
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

void RingBeamer::attackSensor(HitSensor *pSender, HitSensor *pReceiver){
    if(MR::isSensorPlayerOrRide(pReceiver) && MR::isSensorEnemyAttack(pSender)){
        MR::sendMsgEnemyAttackStrong(pReceiver,pSender);
    }else{
        MR::sendMsgPush(pReceiver,pSender);
    }
}
sendMsgPush is pushing hitsensors outside of others
*/