#include "GBNRdtReceiver.hh"
#include "GBNRdtSender.hh"
#include "Global.hh"
#include "RdtReceiver.hh"
#include "RdtSender.hh"
#include "TestUtils.hh"

int main(int argc, char* argv[]) {
  return TestUtils::tester<GBNRdtSender, GBNRdtReceiver>();
}
