# QT-UDP_Hole_Punching

QT UDP Hole Puching is a test project try to teste the tecnic called UDP Hole Puching there are better describe in this link http://www.brynosaurus.com/pub/net/p2pnat/.
This is only a proof of concept of UDP hole punch a tecnique that allow 2 computers behind NATs comunicate with other direct.

The bigs new now is there no need to create you own rendezvous server is possible do tha same with public STUN and Torrent Tracker Servers

As torrent Tracker i'm using
public.popcorn-tracker.org:6969

And as STUN i'm using
iphone-stun.strato-iphone.de:3478

both are publics servers with good responce if you want use other check on google is easy find lists of this kind server.

## Getting Started

This current master version don't have a appimage version.
Just download the project and open the project with QT creator

### Prerequisites

Is need the QT creator version 5.10 or grater with C++14

## Running the tests

After download the project and open with QT creator go to the testeclass.cpp there you will see a small use example of the project

The ideia is simple use a STUN serve to get the current public endpoint (ip + upd port) and make public on a torrent Tracker.

You will see in the TesteClass constructor a signal and slot
connect(connection, &Conn::update, this, &TesteClass::sendToAllPeopleInRoom);

This is a very import signal send from the Conn class, the update signal is send
whatever a annouce response is recive from the Torrent Tracker, with that you can
get the newest updated list of endpoints on the current "Torrent room" the only problem is that list includes your current endpoint is included so you need to ignore your self using the information from the StunClient.

Another problem is the QUdpSocket must be shared between the StunClient, Torrent Tracker and your implementation, because that when you create your own implementation you can't send and recive at the same time with annouce on the tracker.

## Built With

* [QT](http://doc.qt.io/) - The C++ framework used


## Authors

* **Victor Lacerda** - [VictorGerin](https://github.com/VictorGerin)

## License

This project is licensed under the GNU License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Bryan Ford (Massachusetts Institute of Technology)
* Pyda Srisuresh (Caymas Systems, Inc)
* Dan Kegel (dank (at) kegel.com)

