// Modbus for c++ <https://github.com/Mazurel/Modbus>
// Copyright (c) 2020 Mateusz Mazur aka Mazurel
// Licensed under: MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <memory>
#include <type_traits>

#include <cerrno>
#include <libnet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

#include "MB/modbusException.hpp"
#include "MB/modbusRequest.hpp"
#include "MB/modbusResponse.hpp"

namespace MB::TCP {
class Connection {
public:
  static const unsigned int DefaultTCPTimeout = 500;

private:
  int _sockfd = -1;
  uint16_t _messageID = 0;
  int _timeout = Connection::DefaultTCPTimeout;

public:
  explicit Connection() noexcept : _sockfd(-1), _messageID(0){};
  explicit Connection(int sockfd) noexcept;
  Connection(const Connection &copy) = delete;
  Connection(Connection &&moved) noexcept;
  Connection &operator=(Connection &&other) noexcept {
    if (this == &other)
      return *this;

    if (_sockfd != -1 && _sockfd != other._sockfd)
      ::close(_sockfd);

    _sockfd = other._sockfd;
    _messageID = other._messageID;
    other._sockfd = -1;

    return *this;
  }

  [[nodiscard]] int getSockfd() const { return _sockfd; }

  static Connection with(std::string addr, int port);

  ~Connection();

  void sendRequest(const MB::ModbusRequest &req);
  void sendResponse(const MB::ModbusResponse &res);
  void sendException(const MB::ModbusException &ex);

  [[nodiscard]] MB::ModbusRequest awaitRequest();
  [[nodiscard]] MB::ModbusResponse awaitResponse();

  [[nodiscard]] std::vector<uint8_t> awaitRawMessage();

  [[nodiscard]] uint16_t getMessageId() const { return _messageID; }

  void setMessageId(uint16_t messageId) { _messageID = messageId; }
};
} // namespace MB::TCP
