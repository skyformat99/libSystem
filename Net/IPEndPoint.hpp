//
// Created by root on 17-8-21.
//

#ifndef LIBSYSTEM_IPENDPOINT_HPP
#define LIBSYSTEM_IPENDPOINT_HPP

#include "Net.hpp"
#include "IPAddress.hpp"
#include "EndPoint.hpp"
#include "../Exception/SocketException.hpp"
#include "Socket.hpp"

using Reimu::System::SocketException;

namespace Reimu {
    namespace System {
	namespace Net {
	    class IPEndPoint : public EndPoint {
	    public:
		IPAddress Address;
		uint16_t Port = 0;

		IPEndPoint() {}
		IPEndPoint(IPAddress addr, uint16_t port) {
			AddressFamily = addr.Family;
			Port = port;
			Address = addr;
			SocketAddress = (uint8_t *)malloc_reimu(AddressFamily == AF_INET ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
			if (AddressFamily == Sockets::AddressFamily::InterNetwork) {
				sockaddr_in *thissa = (sockaddr_in *)SocketAddress;
				thissa->sin_family = AF_INET;
				memcpy(&thissa->sin_addr, Address.NetworkAddress, sizeof(in_addr));
				thissa->sin_port = htobe16(Port);
			} else if (AddressFamily == Sockets::AddressFamily::InterNetworkV6) {
				sockaddr_in6 *thissa = (sockaddr_in6 *)SocketAddress;
				thissa->sin6_family = AF_INET;
				memcpy(&thissa->sin6_addr, Address.NetworkAddress, sizeof(in6_addr));
				thissa->sin6_port = htobe16(Port);
			} else {
				throw SocketException(EAFNOSUPPORT);
			}
		}

		std::string ToString() { return Address.ToString() + ":" + std::to_string(Port); }

		bool const operator== (const IPEndPoint &o) const { return Port == o.Port && Address == o.Address; }
		bool const operator< (const IPEndPoint &o) const {
			if (Address < o.Address)
				return 1;

			if (Port < o.Port)
				return 0;

			return 1;
		}

		IPEndPoint(const IPEndPoint &other) {
			Address = other.Address;
			SocketAddress = Address.NetworkAddress;
			Port = other.Port;
		}
		IPEndPoint(const IPEndPoint &&other) {
			Address = std::move(other.Address);
			SocketAddress = Address.NetworkAddress;
			Port = other.Port;
		}
		IPEndPoint& operator= (IPEndPoint other) {
			Address = other.Address;
			SocketAddress = Address.NetworkAddress;
			Port = other.Port;
			return *this;
		}
	    };
	}
    }
}


#endif //LIBSYSTEM_IPENDPOINT_HPP
