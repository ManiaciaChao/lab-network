#include "Global.hh"
#include "RdtReceiver.hh"
#include "RdtSender.hh"
#include "StopWaitRdtReceiver.hh"
#include "StopWaitRdtSender.hh"
#include "TestUtils.hh"

int main(int argc, char* argv[]) {
  return TestUtils::tester<StopWaitRdtSender, StopWaitRdtReceiver>();
}
