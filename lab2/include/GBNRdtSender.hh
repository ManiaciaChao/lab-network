#pragma once
#include <cstddef>
#include <deque>

#include "RdtSender.hh"

class GBNRdtSender : public RdtSender {
 private:
  std::size_t base;
  std::size_t next;
  std::deque<Packet> cache;

 public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);

 public:
  GBNRdtSender() : base(0), next(0), cache() {}
  virtual ~GBNRdtSender() override {}
};