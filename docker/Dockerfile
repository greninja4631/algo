FROM gcc:13.2.0

WORKDIR /workspace

COPY . .

RUN apt-get update && \
    apt-get install -y make git doxygen cppcheck clang-tidy clang-format valgrind && \
    rm -rf /var/lib/apt/lists/*

# ここで一度必ずクリーンしてからCI実行（.o, .d残り対策）
RUN make clean

RUN bash run_ci.sh

CMD ["bash"]