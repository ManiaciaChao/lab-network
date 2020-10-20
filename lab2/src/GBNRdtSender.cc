#include "GBNRdtSender.hh"

#include <algorithm>

#include "Color.hh"
#include "DataStructure.hh"
#include "Global.hh"

bool GBNRdtSender::getWaitingState() {
  return next == base + Configuration::WINDOW_SIZE;
}

bool GBNRdtSender::send(const Message &message) {
  if (not getWaitingState()) {
    // make_pkt()
    Packet packet{};
    packet.seqnum = next;
    packet.acknum = -1;
    std::copy_n(message.data, sizeof(message.data), packet.payload);
    packet.checksum = pUtils->calculateCheckSum(packet);

    cache.push_back(packet);

    if (base == next) {
      pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    }
    next++;
    pns->sendToNetworkLayer(RECEIVER, packet);
    pUtils->printPacket(SENDER_PREFIX "packet sent", packet);
    return true;
  }

  return false;
}

void GBNRdtSender::receive(const Packet &packet) {
  pUtils->printPacket(SENDER_PREFIX "ack recieved", packet);
  auto checkSum = pUtils->calculateCheckSum(packet);
  // corrupt(rcvpkt)
  if (checkSum != packet.checksum) {
    printf(SENDER_PREFIX RED("wrong checksum\n"));
    return;
  }
  // notcorrupt(rcvpkt)

  // printf("base: %d, ack:%d\n", base, packet.acknum);
  printf(SENDER_PREFIX YELLOW("sw range changed from: [%d, %d] "),
         cache.front().seqnum, cache.back().seqnum);
  for (int diff = packet.acknum - base; diff >= 0; diff--) {
    cache.pop_front();
  }
  printf(YELLOW("to [%d, %d]\n"), cache.front().seqnum, cache.back().seqnum);

  base = packet.acknum + 1;
  pns->stopTimer(SENDER, 0);

  if (base != next) {
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
  }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
  // seqNum is the base when timer just started, always 0
  // std::cout << "Timeout!" << std::endl;
  pns->stopTimer(SENDER, 0);
  pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
  for (auto &a : cache) {
    pns->sendToNetworkLayer(RECEIVER, a);
    pUtils->printPacket(SENDER_PREFIX RED("timeout then resend"), a);
  }
  // std::cout << std::endl;
};
