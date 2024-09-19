FROM ubuntu:latest

RUN apt-get update -y
RUN apt-get upgrade -y

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y cmake g++ libcurl4-openssl-dev nlohmann-json3-dev

COPY . .

WORKDIR "/root"