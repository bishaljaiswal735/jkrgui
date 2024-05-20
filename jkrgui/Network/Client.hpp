#pragma once
#include "Connection.hpp"

namespace Jkr::Network {
class ClientInterface {
    public:
    using OnClientValidationFunctionType = Connection::OnClientValidationFunctionType;
    ClientInterface() : mSocket(mContext) {}
    ~ClientInterface() { Disconnect(); }
    bool Connect(OnClientValidationFunctionType& inFunction,
                 const std::string_view inHost,
                 const uint16_t inPort);
    void Disconnect();
    bool IsConnected();
    void Send(const Message& inMessage);

    GETTER& GetIncomingQMessages() { return mQMessagesIn; }

    protected:
    // handles data transfer
    asio::io_context mContext;
    std::thread mThreadContext;
    asio::ip::tcp::socket mSocket;
    up<Connection> mConnection;

    private:
    TsQueue<OwnedMessage> mQMessagesIn;
};
} // namespace Jkr::Network