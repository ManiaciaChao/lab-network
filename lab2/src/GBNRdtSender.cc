#include "GBNRdtSender.hh"

#include <algorithm>

#include "DataStructure.hh"
#include "Global.hh"
bool GBNRdtSender::getWaitingState() {
  return nextSeqNum == baseSeqNum + Configuration::WINDOW_SIZE;
}

bool GBNRdtSender::send(const Message &message) {
  if (not getWaitingState()) {
    // make_pkt()
    Packet packet{};
    packet.seqnum = nextSeqNum;
    packet.acknum = -1;
    std::copy_n(message.data, sizeof(message.data), packet.payload);
    packet.checksum = pUtils->calculateCheckSum(packet);

    cache.push_back(packet);

    if (baseSeqNum == nextSeqNum) {
      pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
    }
    nextSeqNum++;
    pns->sendToNetworkLayer(RECEIVER, packet);

    pUtils->printPacket("Packet Send!", packet);
    return true;
  }

  return false;
}

void GBNRdtSender::receive(const Packet &ackPkt) {
  auto checkSum = pUtils->calculateCheckSum(ackPkt);
  // corrupt(rcvpkt)
  if (checkSum != ackPkt.checksum) {
    // pUtils->printPacket("Wrong checksum", ackPkt);
    return;
  }
  // notcorrupt(rcvpkt)

  // printf("base: %d, ack:%d\n", baseSeqNum, ackPkt.acknum);
  printf("sw range changed from: [%d, %d] ", cache.front().seqnum,
         cache.back().seqnum);
  for (int diff = ackPkt.acknum - baseSeqNum; diff >= 0; diff--) {
    cache.pop_front();
  }
  printf("to [%d, %d]\n", cache.front().seqnum,
         cache.back().seqnum);

  baseSeqNum = ackPkt.acknum + 1;
  pns->stopTimer(SENDER, 0);

  if (baseSeqNum != nextSeqNum) {
    pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
  }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
  // seqNum is the baseSeqNum when timer just started, always 0
  // std::cout << "Timeout!" << std::endl;
  pns->stopTimer(SENDER, 0);
  pns->startTimer(SENDER, Configuration::TIME_OUT, 0);
  for (auto &a : cache) {
    pns->sendToNetworkLayer(RECEIVER, a);
  }
  // std::cout << std::endl;
};
