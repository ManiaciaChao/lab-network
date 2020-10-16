#include "SRRdtSender.hh"

#include <algorithm>

#include "DataStructure.hh"
#include "Global.hh"

bool SRRdtSender::getWaitingState() {
  return (next < base) || (next >= base + cache.size());
}

bool SRRdtSender::send(const Message &message) {
  if (not getWaitingState()) {
    // make_pkt()
    Packet packet{};
    packet.seqnum = next++;
    packet.acknum = -1;
    std::copy_n(message.data, sizeof(message.data), packet.payload);
    packet.checksum = pUtils->calculateCheckSum(packet);

    cache[packet.seqnum] = packet;
    pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
    pns->sendToNetworkLayer(RECEIVER, packet);

    // pUtils->printPacket("Packet Send!", packet);
    return true;
  }

  return false;
}

void SRRdtSender::receive(const Packet &packet) {
  auto ackNum = packet.acknum;
  auto checkSum = pUtils->calculateCheckSum(packet);
  // corrupt(rcvpkt)
  if (checkSum != packet.checksum) {
    // pUtils->printPacket("Wrong checksum", ackPkt);
    return;
  }
  // pUtils->printPacket("Sender receives an ACK:", packet);

  // notcorrupt(rcvpkt)
  cache[ackNum].acknum = ackNum;
  // printf("base: %d, ack:%d\n", base, ackPkt.acknum);

  if (ackNum == base) {
    printf("sw range changed from: [%d, %d] ", base, base + cache.size());
    for (std::size_t i = 1; i < cache.size(); i++) {
      if (cache[base + i].acknum != base + i) {
        base += i;
        break;
      }
    }
    printf("to: [%d, %d]\n", base, base + cache.size());
  }

  pns->stopTimer(SENDER, ackNum);
}

void SRRdtSender::timeoutHandler(int seqNum) {
  // seqNum is the base when timer just started, always 0
  // std::cout << "Timeout!" << std::endl;
  pns->stopTimer(SENDER, seqNum);
  pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
  pns->sendToNetworkLayer(RECEIVER, cache[seqNum]);
  // std::cout << std::endl;
};
