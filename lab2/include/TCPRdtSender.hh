#pragma once
#include <cstddef>

#include "RdtSender.hh"
#include "SRRdtCache.hh"

class TCPRdtSender : public RdtSender {
 private:
  std::size_t base;
  std::size_t next;
  int lastAck;
  std::size_t dupAckCount;
  Cache cache;

 public:
  bool getWaitingState();
  bool send(const Message &message);
  void receive(const Packet &ackPkt);
  void timeoutHandler(int seqNum);

 public:
  TCPRdtSender() : base(0), next(0), lastAck(-1), dupAckCount(0) {}
  virtual ~TCPRdtSender() override {}
};