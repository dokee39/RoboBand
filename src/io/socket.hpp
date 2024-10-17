#pragma once

#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include "io/io.hpp"

namespace robo {
namespace io {
class SocketServer: public IoKey {
public:
    explicit SocketServer(const std::string &name, const int port, const int buffer_size):
        IoKey(name, buffer_size),
        sockfd(socket(AF_INET, SOCK_DGRAM, 0)) {
        if (sockfd < 0) {
            LOG(ERROR) << "[IO<" + name + ">] Cannot open socket.";
            return;
        }

        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = INADDR_ANY;
        my_addr.sin_port = htons(port);

        if (bind(sockfd, (sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
            LOG(ERROR) << "[IO<" + name + ">] Cannot bind socket fd with port number" << port << ".";
        }
    }
    ~SocketServer() override {
        close(sockfd);
    }

    int read(int &key, char *data) override {
        sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        auto len = recvfrom(sockfd, data, 256, MSG_WAITALL, (sockaddr *)&cli_addr, &cli_addr_len);
        if (len > 0) {
            key = data[0];
            if (!clients.count(key)) {
                LOG(INFO) << "[IO<" + name + ">] Register client: " << key << ".";
                clients.emplace(key, cli_addr);
            }
        }
        return len;
    }

    bool send(const int key, const char *data, const int len) {
        auto it = clients.find(key);
        if (it == clients.end()) {
            LOG(WARNING) << "[IO<" + name + ">] The client address has not been obtained yet: " << key << ".";
            return false;
        }
        auto n = sendto(sockfd, data, len, MSG_CONFIRM, (const struct sockaddr *)&it->second, sizeof(it->second));
        if (n < 0) {
            LOG(WARNING) << "[IO<" + name + ">] Failed to send to client, key: " << key << ".";
            return false;
        }
        return true;
    }

private:
    int sockfd;
    sockaddr_in my_addr;
    std::map<int, sockaddr_in> clients;
};

class SocketClient: public IoNoKey {
public:
    explicit SocketClient(const std::string &name, const char *serv_hostname, const int serv_port, const int buffer_size):
        IoNoKey(name, buffer_size),
        sockfd(socket(AF_INET, SOCK_DGRAM, 0)) {
        if (sockfd < 0) {
            LOG(ERROR) << "[IO<" + name + ">] Cannot open socket.";
            return;
        }

        serv_addr.sin_family = AF_INET;
        auto hp = gethostbyname(serv_hostname);
        if (hp == 0) {
            LOG(ERROR) << "[IO<" + name + ">] Unknown host: " + serv_hostname;
            return;
        }
        memcpy((char *)hp->h_addr, (char *)&serv_addr.sin_addr, hp->h_length);
        serv_addr.sin_port = htons(serv_port);
    }
    ~SocketClient() override {
        close(sockfd);
    }

    int read(char *data) override {
        sockaddr_in from_addr;
        socklen_t from_addr_len = sizeof(from_addr);
        return recvfrom(sockfd, data, 256, MSG_WAITALL, (sockaddr *)&from_addr, &from_addr_len);
    }

    bool send(const char *data, const int len) {
        auto n=sendto(sockfd, data, len, MSG_CONFIRM, (const struct sockaddr *)&serv_addr, sizeof(sockaddr_in));
        if (n < 0) {
            LOG(WARNING) << "[IO<" + name + ">] Failed to send to server.";
            return false;
        }
        return true;
    }

private:
    int sockfd;
    sockaddr_in serv_addr;

};
}
}

