#pragma once
#include <array>
#include <cstddef>

#include "DataStructure.hh"

class Cache : public std::array<Packet, Configuration::WINDOW_SIZE> {
 public:
  Packet& operator[](size_t i) {
    return this->at(i % Configuration::WINDOW_SIZE);
  };

 public:
  Cache() {
    for (auto& pkt : *this) {
      pkt.acknum = -1;
      pkt.seqnum = -1;
    }
  }
};