#pragma once
#include <filesystem>

#include "Global.hh"
#include "RdtReceiver.hh"
#include "RdtSender.hh"

namespace TestUtils {
namespace fs = std::filesystem;
const auto inputPath = fs::relative("./input.txt");
const auto outputPath = fs::relative("./output.txt");

template <typename Sender, typename Receiver>
int tester() {
  RdtSender* ps = new Sender();
  RdtReceiver* pr = new Receiver();
  pns->setRunMode(0);  // VERBOS模式
                       // pns->setRunMode(1);  //安静模式
  pns->init();
  pns->setRtdSender(ps);
  pns->setRtdReceiver(pr);
  pns->setInputFile(TestUtils::inputPath.c_str());
  pns->setOutputFile(TestUtils::outputPath.c_str());

  pns->start();

  delete ps;
  delete pr;
  delete pUtils;  //指向唯一的工具类实例，只在main函数结束前delete
  delete pns;  //指向唯一的模拟网络环境类实例，只在main函数结束前delete

  return 0;
}
}  // namespace TestUtils