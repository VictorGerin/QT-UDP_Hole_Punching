# QT-UDP_Hole_Punching

QT UDP Hole Puching is a test project try to teste the tecnic called UDP Hole Puching there are better describe in this link http://www.brynosaurus.com/pub/net/p2pnat/.
This is only a proof of concept of UDP hole punch a tecnique that allow 2 computers behind NATs comunicate with outer direct.

## Getting Started

To teste just get the newest version with AppImage https://github.com/VictorGerin/QT-UDP_Hole_Punching/releases

### Prerequisites

The arent prerequisites to use the AppImage version, but if you want download the code and compile you can use the QTCreator to do that it have all depedences.

## Running the tests

To teste you will need at least one computer to be a server and two to be a client all can be made using the VirtualBox, in my case i have create 2 virtual machine behind NAT and use my own computer as server.

To start the Server just type the command, i have choose 12345 as listen UDP port but you can use any port that you want

```
$ ./UDP -s --port 12345
```

Now to start the client you just do the folow command remenber that the IP set must be the server IP and the port must be the same used by the server

```
$ ./UDP -c --ip 192.168.0.15 --port 12345
```

If all go rigth you will see a menssage, like this ![Image](https://raw.githubusercontent.com/VictorGerin/QT-UDP_Hole_Punching/master/Screenshot%20from%202018-09-23%2014-50-03.png)

```
Teste : "Testando que foda isso veii :)"
```




## Built With

* [QT](http://doc.qt.io/) - The C++ framework used


## Authors

* **Victor Lacerda** - [QT-UDP_Hole_Punching](https://github.com/VictorGerin)

## License

This project is licensed under the GNU License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Bryan Ford (http://www.brynosaurus.com/pub/net/p2pnat/)
* Pyda Srisuresh (Caymas Systems, Inc)
* Dan Kegel (dank (at) kegel.com)

