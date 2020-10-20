#include "TCPRdtSender.hh"

#include <algorithm>

#include "Color.hh"
#include "DataStructure.hh"
#include "Global.hh"
#include "TestUtils.hh"
using namespace Color;

bool TCPRdtSender::getWaitingState() {
  return next == base + Configuration::WINDOW_SIZE;
}

bool TCPRdtSender::send(const Message &message) {
  if (not getWaitingState()) {
    // make_pkt()
    Packet packet{};
    packet.seqnum = next++;
    packet.acknum = -1;
    std::copy_n(message.data, sizeof(message.data), packet.payload);
    packet.checksum = pUtils->calculateCheckSum(packet);

    cache[packet.seqnum] = packet;

    if (base == packet.seqnum) {
      pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    }
    pns->sendToNetworkLayer(RECEIVER, packet);

    pUtils->printPacket(SENDER_PREFIX "packet sent", packet);
    return true;
  }

  return false;
}

void TCPRdtSender::receive(const Packet &packet) {
  pUtils->printPacket(SENDER_PREFIX "ack recieved", packet);
  auto checkSum = pUtils->calculateCheckSum(packet);
  auto ackNum = packet.acknum;
  // corrupt(rcvpkt)
  if (checkSum != packet.checksum) {
    printf(SENDER_PREFIX RED("wrong checksum\n"));
    return;
  }
  // notcorrupt(rcvpkt)

  // fask retransmit
  dupAckCount = ackNum == lastAck ? (dupAckCount + 1) : (lastAck = ackNum, 0);
  if (ackNum <= base && dupAckCount >= 3) {
    pUtils->printPacket(SENDER_PREFIX YELLOW("fast retransmit"), cache[ackNum]);

    pns->sendToNetworkLayer(RECEIVER, cache[ackNum]);
    dupAckCount = 0;
  } else {
    printf(
        SENDER_PREFIX YELLOW("sw range changed from: [%d, %d] to [%d, %d]\n"),
        base, base + cache.size(), ackNum + 1, ackNum + 1 + cache.size());
    base = ackNum;
  }
  pns->stopTimer(SENDER, 0);
  if (base != next) {
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
  }
}

void TCPRdtSender::timeoutHandler(int seqNum) {
  // seqNum is the base when timer just started, always 0
  // std::cout << "Timeout!" << std::endl;
  pns->stopTimer(SENDER, 0);
  pns->startTimer(SENDER, Configuration::TIME_OUT, 0);

  pUtils->printPacket(SENDER_PREFIX RED("timeout then resend"), cache[base]);
  pns->sendToNetworkLayer(RECEIVER, cache[base]);
  // std::cout << std::endl;
  // for (auto &a : cache) {
  //   pns->sendToNetworkLayer(RECEIVER, a);
  // }
};
