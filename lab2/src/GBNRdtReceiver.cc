#include "GBNRdtReceiver.hh"

#include "Color.hh"
#include "Global.hh"
#include "algorithm"

void GBNRdtReceiver::receive(const Packet& packet) {
  pUtils->printPacket(RECEIVER_PREFIX "packet recieved", packet);
  auto checkSum = pUtils->calculateCheckSum(packet);
  if (packet.checksum != checkSum) {
    printf(RECEIVER_PREFIX RED("wrong checksum\n"));
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    pUtils->printPacket(RECEIVER_PREFIX "ack resent", lastAckPkt);
    return;
  }
  if (packet.seqnum != expectSeqNum) {
    printf(RECEIVER_PREFIX "unexpected or out-of-order packet incoming\n");
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    pUtils->printPacket(RECEIVER_PREFIX "ack resent", lastAckPkt);
    return;
  }
  Message msg;
  std::copy_n(packet.payload, sizeof(msg.data), msg.data);
  pns->delivertoAppLayer(RECEIVER, msg);

  lastAckPkt.seqnum = -1;
  lastAckPkt.acknum = expectSeqNum++;
  lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);

  pns->sendToNetworkLayer(SENDER, lastAckPkt);
  pUtils->printPacket(RECEIVER_PREFIX "ack resent", lastAckPkt);
}