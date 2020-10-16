#pragma once
#include <array>
#include <cstddef>

#include "RdtSender.hh"
#include "SRRdtCache.hh"

class SRRdtSender : public RdtSender {
 private:
  std::size_t base;
  std::size_t next;
  Cache cache;

 public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);

 public:
  SRRdtSender() : base(0), next(0), cache() {}
  virtual ~SRRdtSender() override {}
};