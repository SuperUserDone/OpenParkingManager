#include "networking/Socket.hpp"

Socket::Socket()
{
}

Socket::Socket(const std::string& address, const std::string& port)
    : m_address(address)
    , m_port(port)
{
}

bool Socket::connect_socket()
{
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv;
    if ((rv = getaddrinfo(m_address.c_str(), m_port.c_str(), &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        ;
        return false;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_socket_fd = socket(p->ai_family, p->ai_socktype,
                 p->ai_protocol))
            == -1) {
            perror("Socket: Creation error");
            continue;
        }

        if (connect(m_socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_socket_fd);
            perror("Socket: Connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        std::cout << "Socket: Failed to connect" << std::endl;
        return false;
    }

    freeaddrinfo(servinfo);
    connected = true;
    return true;
}

void Socket::set_address(const std::string& address)
{
    m_address = address;
}

void Socket::set_port(const std::string& port)
{
    m_port = port;
}

std::string Socket::get_address()
{
    return m_address;
}

std::string Socket::get_port()
{
    return m_port;
}

bool Socket::send_data(const char* data)
{
    return send_data((uint8_t*) data);
}

bool Socket::send_data(const uint8_t* data)
{
    if (connected) {
        if (send(m_socket_fd, data, strlen((char*)data), 0) == -1) {
            perror("send");
        }
        return true;
    } else {
        std::cout << "Socket: Not connected" << std::endl;
        return false;
    }
}

uint8_t* Socket::receive_data(int max_size)
{
    if (connected) {
        int numbytes = 0;
        uint8_t* buf = new uint8_t[max_size];
        if ((numbytes = recv(m_socket_fd, buf, max_size - 1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        return buf;
    } else {
        std::cout << "Socket: Not connected" << std::endl;
        return 0;
    }
}

void Socket::disconnect()
{
    connected = false;
}

Socket::~Socket()
{
}