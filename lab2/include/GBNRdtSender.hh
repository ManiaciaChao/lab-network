#pragma once
#include <cstddef>
#include <deque>

#include "RdtSender.hh"

class GBNRdtSender : public RdtSender {
 private:
  std::size_t baseSeqNum;
  std::size_t nextSeqNum;
  std::deque<Packet> cache;

 public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);

 public:
  GBNRdtSender() : baseSeqNum(0), nextSeqNum(0), cache() {}
  virtual ~GBNRdtSender() override {}
};