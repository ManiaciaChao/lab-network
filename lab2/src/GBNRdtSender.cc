#pragma once
#include "GBNRdtSender.hh"

#include <algorithm>

#include "DataStructure.hh"
#include "Global.hh"
bool GBNRdtSender::getWaitingState() {
  return nextSeqNum == Configuration::WINDOW_SIZE;
}

bool GBNRdtSender::send(const Message &message) {
  if (not getWaitingState()) {
    Packet packet{};
    packet.seqnum = nextSeqNum++;
    packet.acknum = -1;
    std::copy_n(message.data, sizeof(message.data), packet.payload);

    packet.checksum = pUtils->calculateCheckSum(packet);
    cache[static_cast<std::size_t>(packet.seqnum)] = packet;
    pns->startTimer(RandomEventTarget::SENDER, Configuration::TIME_OUT,
                    packet.seqnum);
    pns->sendToNetworkLayer(RandomEventTarget::RECEIVER, packet);
    pUtils->printPacket("Packet Send!", packet);
    return true;
  }

  return false;
}