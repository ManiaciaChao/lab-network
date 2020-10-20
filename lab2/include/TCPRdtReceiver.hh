#pragma once
#include <cstddef>  // for std::size_t

#include "RdtReceiver.hh"
#include "SRRdtCache.hh"

class TCPRdtReceiver : public RdtReceiver {
 private:
  std::size_t base;
  Packet lastAckPkt;
  Cache cache;

 public:
  void receive(const Packet &packet);

 public:
  TCPRdtReceiver() : base(0), lastAckPkt() { lastAckPkt.acknum = -1; };
  virtual ~TCPRdtReceiver() override {}
};
