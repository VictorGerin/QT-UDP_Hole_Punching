#!/bin/bash
rm -rf *.o *.h *.cpp Makefile
linuxdeployqt.sh $1 -appimage #-bundle-non-qt-libs -always-overwrite
rm -rf ${2}.tar.gz
#rm -rf AppRun doc default.desktop default.png translations Application-x86_64.AppImage
tar cvzf ../${2}.tar.gz ./*
mv ../${2}.tar.gz .

ssh root@localhost -p ${3} "rm -rf ${2}.tar.gz"
ssh root@localhost -p ${3} "rm -rf ./teste/*"
scp -P ${3} ${2}.tar.gz root@localhost:~
ssh root@localhost -p ${3} "mv ${2}.tar.gz teste; cd teste; tar xvzf ${2}.tar.gz"
