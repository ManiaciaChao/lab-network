#pragma once
#include <cstddef>  // for std::size_t

#include "RdtReceiver.hh"

class GBNRdtReceiver : public RdtReceiver {
 private:
  std::size_t expectSeqNum;
  Packet lastAckPkt;

 public:
  void receive(const Packet &packet);

 public:
  GBNRdtReceiver() : expectSeqNum(0), lastAckPkt() { lastAckPkt.acknum = -1; };
  virtual ~GBNRdtReceiver() override {}
};
