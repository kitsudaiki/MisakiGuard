FROM ubuntu:20.04

RUN apt-get update; \
    apt-get install -y openssl libuuid1 libcrypto++6 libsqlite3-0 rst2pdf; \
    mkdir /etc/misaki
COPY upload/MisakiGuard /usr/bin/MisakiGuard
CMD MisakiGuard
