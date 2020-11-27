FROM gcc:4.9
COPY . /welcome
WORKDIR /welcome/
RUN gcc -o welcome welcome.c
CMD ["./welcome"]
