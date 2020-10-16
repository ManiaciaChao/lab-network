#include "SRRdtReceiver.hh"

#include "Global.hh"
#include "algorithm"
void SRRdtReceiver::receive(const Packet& packet) {
  auto checkSum = pUtils->calculateCheckSum(packet);
  auto seqNum = packet.seqnum;
  if (packet.checksum != checkSum) {
    // printf("R: wrong checksum\n");
    return;
  }
  if (seqNum >= static_cast<int>(base - cache.size()) &&
      seqNum < static_cast<int>(base + cache.size())) {
    lastAckPkt.seqnum = -1;
    lastAckPkt.acknum = seqNum;
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
    // pUtils->printPacket("Receiver sends an ACK", lastAckPkt);
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    if (seqNum < base) {
      return;
    }
    if (cache[seqNum].seqnum != seqNum) {
      cache[seqNum] = packet;
    };
    if (seqNum == base) {
      for (std::size_t i = 0; i < cache.size(); i++) {
        if (cache[base + i].seqnum == base + i) {
          Message msg;
          std::copy_n(cache[base + i].payload, sizeof(msg.data), msg.data);
          pns->delivertoAppLayer(RECEIVER, msg);
        } else {
          base += i;
          break;
        }
      }
    }
  }
}