#include "QuestionBlockDude.hpp"
#include <Game/Util/StringUtil.hpp>


QuestionBlockDude::QuestionBlockDude(const char * pName) : LiveActor(pName) {
    RandomStuff = 23;
}

QuestionBlockDude::~QuestionBlockDude() {}

void QuestionBlockDude::init(const JMapInfoIter &rIfter) {
    MR::initDefaultPos(this,rIfter);
    initModelManagerWithAnm("QuestionBlockDude", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    makeActorAppeared();
    MR::startBtpAndSetFrameAndStop(this, "BodyChanging1", 1.0f);
}

/*
Use MR::useStageSwitchWriteA/B/Dead in init
And then later on, use MR::onSwitchA/B/Dead
Then simply link that switch with an SW_APPEAR of another object*/