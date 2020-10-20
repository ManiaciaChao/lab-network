#include "TCPRdtReceiver.hh"

#include "Color.hh"
#include "Global.hh"
#include "algorithm"

using namespace Color;

void TCPRdtReceiver::receive(const Packet& packet) {
  pUtils->printPacket(RECIEVER_PREFIX "packet recieved", packet);

  auto checkSum = pUtils->calculateCheckSum(packet);
  auto seqNum = packet.seqnum;

  if (packet.checksum != checkSum) {
    printf(RECIEVER_PREFIX RED("wrong checksum\n"));
    lastAckPkt.acknum = base;
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    pUtils->printPacket(RECIEVER_PREFIX "ack resent", lastAckPkt);
    return;
  }
  cache[packet.seqnum] = packet;

  if (packet.seqnum != base) {
    printf(RECIEVER_PREFIX "unexpected or out-of-order packet incoming\n");
    lastAckPkt.acknum = base;
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    pUtils->printPacket(RECIEVER_PREFIX "ack resent", lastAckPkt);
    return;
  }

  for (std::size_t i = 0; i < cache.size(); i++) {
    if (cache[base + i].seqnum == base + i) {
      Message msg;
      std::copy_n(cache[base + i].payload, sizeof(msg.data), msg.data);
      pns->delivertoAppLayer(RECEIVER, msg);
    } else {
      lastAckPkt.acknum = base;
      lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
      pns->sendToNetworkLayer(SENDER, lastAckPkt);
      printf(RECIEVER_PREFIX YELLOW(
                 "sw range changed from: [%d, %d] to [%d, %d]\n"),
             base, base + cache.size(), base + i, base + i + cache.size());
      base += i;

      break;
    }
  }
}