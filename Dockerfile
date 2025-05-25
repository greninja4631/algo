FROM gcc:latest

WORKDIR /workspace

COPY . .

RUN apt-get update && apt-get install -y make valgrind

CMD ["/bin/bash"]