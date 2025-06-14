#include "TestNameObj.hpp"
#include <Game/Util/StringUtil.hpp>


TestNameObj::TestNameObj(const char * pName) : NameObj(pName) {

}

TestNameObj::~TestNameObj() {}

void TestNameObj::init(const JMapInfoIter &rIfter) {
    OSReport("This is a test obj!");
}