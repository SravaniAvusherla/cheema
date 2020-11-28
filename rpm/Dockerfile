#FROM gcc:4.9
#COPY . /rpm
#WORKDIR /rpm/
#RUN gcc -pthread rpm.c -o rpm.o -Ihiredis/ -Ilibuv/include -Wl,--no-as-needed -ldl -lhiredis/hiredis libuv/libuv_a.a
#CMD ["./rpm"]

FROM ubuntu:20.04
#FROM gcc:4.9

# Copy the source files under /usr/src
COPY . /usr/src/

# Copy any other extra libraries or dependencies from your machine into the image
COPY hiredis/ /usr/src/hiredis
COPY libuv/   /usr/src/libuv

# Specify the working directory in the image
WORKDIR /usr/src/

# Run your cmake instruction you would run
RUN apt-get update && apt-get install -y cmake
RUN apt-get install -y build-essential
RUN cmake . && make

# Run the program output from the previous step
CMD ["./rpm"]
