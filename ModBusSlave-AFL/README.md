# Fuzz libmodbus using AFL

### Compile libmodbus

```bash
CFLAGS="-fprofile-arcs -ftest-coverage" CC=afl-gcc CXX=afl-g++ ./configure --prefix=/media/root/bc0d2c7b-6e6b-4204-86c8-ac651d67d2ae/FuzzProj/libmodbus-afl-dist --enable-static
make all
make install
```

### Compile preeny

```bash
# logging
gcc -c logging.c -o logging.o
ar rcv logging.a logging.o
# desock
gcc -c desock.c -o desock.o
ar rcv desock.a desock.o
```

### Compile slave

```bash
# Fuzz with LD_PRELOAD, without gcov
afl-gcc -g -fprofile-arcs -ftest-coverage -o slave -I ../libmodbus-afl-dist/include/modbus slave.c ../libmodbus-afl-dist/lib/libmodbus.so
# Fuzz without LD_PRELOAD, with gcov
afl-gcc -g -fprofile-arcs -ftest-coverage -o slave-static -I ../libmodbus-afl-dist/include/modbus slave.c ../libmodbus-afl-dist/lib/libmodbus.a ./desock.a ./logging.a -l pthread -l dl -l gcov
```

### LD_PRELOAD

```bash
LD_PRELOAD=../libmodbus-afl-dist/lib/libmodbus.so:../../Fuzzer/preeny-dist/desock.so:../../Fuzzer/preeny-dist/logging.so ./slave
```

### Code coverage

```bash
export DISK_GUID=bc0d2c7b-6e6b-4204-86c8-ac651d67d2ae
python2 ./afl-cov -d ../../FuzzProj/modbus-honggfuzz/output --live --enable-branch-coverage --code-dir /media/root/$DISK_GUID/FuzzProj/libmodbus-afl/src --coverage-cmd "cat AFL_FILE | ../../FuzzProj/modbus-honggfuzz/slave-static"
```

### Begin fuzzing

```bash
afl-fuzz -i ./data/ -o ./output/ ./slave-static
```

### Resume fuzzing

```bash
afl-fuzz -i - -o ./output/ ./slave-static
python2 ./afl-cov -d ../../FuzzProj/modbus-honggfuzz/output --live --enable-branch-coverage --code-dir /media/root/bc0d2c7b-6e6b-4204-86c8-ac651d67d2ae/FuzzProj/libmodbus-afl/src --coverage-cmd "cat AFL_FILE | ../../FuzzProj/modbus-honggfuzz/slave-static" --overwrite
```

