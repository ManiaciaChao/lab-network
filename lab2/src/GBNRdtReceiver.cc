#include "GBNRdtReceiver.hh"

#include "Global.hh"
#include "algorithm"
void GBNRdtReceiver::receive(const Packet& packet) {
  auto checkSum = pUtils->calculateCheckSum(packet);
  if (packet.checksum != checkSum || packet.seqnum != expectSeqNum) {
    pns->sendToNetworkLayer(SENDER, lastAckPkt);
    return;
  }
  Message msg;
  std::copy_n(packet.payload, sizeof(msg.data), msg.data);
  pns->delivertoAppLayer(RECEIVER, msg);

  lastAckPkt.seqnum = -1;
  lastAckPkt.acknum = expectSeqNum++;
  lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
  pUtils->printPacket("Receiver sends an ACK", lastAckPkt);
  pns->sendToNetworkLayer(SENDER, lastAckPkt);
}