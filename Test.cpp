//
// Created by root on 17-8-21.
//

#include "libSystem.hpp"

#include <sys/epoll.h>
#include <vector>

using Reimu::System::Net::Sockets::Epoll;
using Reimu::System::Net::Sockets::EpollEvent;
using Reimu::System::Net::Sockets::Socket;
using Reimu::System::Net::IPEndPoint;
using Reimu::System::Net::IPAddress;
using Reimu::System::SocketException;

using Reimu::System::Convert;

using namespace std;

vector<IPEndPoint> foundeps;

int Callback(EpollPendingEvent *event, void *userp) {
	SocketException se(event->Socket->GetErrors());
	cout << event->EndPoint->ToString() + ": " + event->ToString() + " / " + se.Message() << endl;

	if (!(event->Events & EPOLLERR)) {
		foundeps.push_back(*event->EndPoint);
	}

	event->Finalize();
}


int main(int argc, char **argv) {

	string sTr("喵喵喵。我的内心毫无波动，甚至还想笑，蛤蛤蛤蛤蛤蛤蛤蛤蛤");
	sTr.push_back(0);

	string hexstr = Convert::ToHexString(sTr);

	cout << hexstr << endl;

	string hexorigstr = Convert::FromHexString(hexstr);

	cout << hexorigstr << endl;

	string b64str = Convert::ToBase64String(sTr);

	cout << b64str << endl;

	string b64origstr = Convert::FromBase64String(b64str);

	cout << b64origstr << endl;

	exit(0);

	Epoll epollctx(128);

	for (int i=1; i<argc; i++) {
		IPAddress addr1(argv[i]);
		IPEndPoint ep1(addr1, 4028);

		Socket so1(AF_INET, SOCK_STREAM, 0);
		so1.SetNonBlocking(1);

		cout << ep1.ToString() << endl;

		try {
			so1.Connect(ep1);
		} catch (SocketException e) {
			cout << e.Message() << endl;
		}

		EpollEvent eev1(ep1, so1);

		epollctx.Add(eev1);
	}

	epollctx.Callback = &Callback;
	epollctx.ExitOnNoFDs = 1;

	epollctx.Dispatch();

	cout << "Found " << foundeps.size() << " hosts:\n";

	for (auto &tgt : foundeps) {
		cout << tgt.ToString() << endl;
	}

}