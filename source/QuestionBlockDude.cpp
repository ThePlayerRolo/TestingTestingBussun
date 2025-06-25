#include "QuestionBlockDude.hpp"
#include <Game/Util/StringUtil.hpp>
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <Game/MapObj/Coin.hpp>
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
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
    mItem = nullptr;
    mVel.zero();
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
    MR::onCalcGravity(this);
    initHitSensor(2);
    initSound(4, false);
    MR::connectToSceneEnemy(this);
    MR::addHitSensorEnemy(this, "body", 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(30.0f, 30.0f, 0);
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
        mItem = MR::createKinokoOneUp();
        break;
        case 3:
        mItem = MR::createKinokoSuper();
        break;
        case 4: //Todo: Test This, yeah.
        case 5:
        case 6:
        MR::declarePowerStar(this, mPowerStarId);
        break;
        case 7:
        mPurpleCoin = (Coin*)(MR::createPurpleCoin(this, "Coin1"));
        break;
    }
    if (mItem != nullptr) {
        MR::setShadowDropLength(mItem, nullptr, 500.0f);
        mItem->mPosition.set(mPosition);
        MR::initDefaultPos(mItem, rIter);
    }
    initNerve(&NrvQuestionBlockDude::NrvQuestionBlockDudeWait::sInstance);
    MR::onCalcGravity(this);
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
        case 3:
        MR::appearKinokoOneUpPop(mItem, getBaseMtx(), 15.0f);
        break;
        case 4:
        case 5:
        case 6:
        MR::requestAppearPowerStar(this, mPowerStarId, mPosition);
        break;

        case 7:
        TVec3f gravityVec;
        MR::calcGravityVector(this, mPosition, &gravityVec, 0, 0);
        gravityVec.scale(-25.0f);
        mPurpleCoin->appearMove(mPosition, gravityVec, 500,0);
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
    if (!MR::isOnGround(this)) {
        MR::addVelocityToGravity(this, 1.5f);
    }
    mVel.z += 0.05f;
    if (mVel.z < 1.0f) {
        MR::addVelocity(this, mVel);
    }
    if (MR::isBindedWall(this)) {
        MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.2f, 0.69f);
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
    return false;
}

bool QuestionBlockDude::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        setNerve(&NrvQuestionBlockDude::NrvQuestionBlockDudeDead::sInstance);
        return true;
    }
    return false;
}

bool QuestionBlockDude::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        OSReport("recieveEnemyAttack!\n");
        return true;
    }
    return false;
}

/*
Use MR::useStageSwitchWriteA/B/Dead in init
And then later on, use MR::onSwitchA/B/Dead
Then simply link that switch with an SW_APPEAR of another object
for hitsensors use initHitSensor, MR::addHitSenser and overload the existing LiveActor functions for hitsensor collision


sendMsgPush is pushing hitsensors outside of others
mTranslation
rVelocity is up to me
lifeTime is how long it lasts, and cannotTime is the time frame of intangibility before the coin is collectable
*/