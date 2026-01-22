#!/bin/bash
set -e

echo "Compiling..."
gcc -pthread 1.c templateFiles.c -o func_mt
gcc -pthread 2.c templateFiles.c -o sys_mt

rm -rf src out_func_seq out_func_thr out_sys_seq out_sys_thr
mkdir -p src out_func_seq out_func_thr out_sys_seq out_sys_thr

echo "Generating 10 input files..."
for i in $(seq 1 10); do
  dd if=/dev/urandom of="src/file$i.bin" bs=1M count=5 status=none
done

ARGS_FUNC_THR=()
ARGS_SYS_THR=()
for i in $(seq 1 10); do
  ARGS_FUNC_THR+=("src/file$i.bin" "out_func_thr/file$i.bin")
  ARGS_SYS_THR+=("src/file$i.bin" "out_sys_thr/file$i.bin")
done

echo
echo "===== FUNCTION COPY (SEQUENTIAL baseline) ====="
echo "Elapsed time:"
time bash -c '
for i in $(seq 1 10); do
  ./func_mt src/file$i.bin out_func_seq/file$i.bin
done
'

echo
echo "===== FUNCTION COPY (THREADED) ====="
echo "Elapsed time:"
time ./func_mt "${ARGS_FUNC_THR[@]}"

echo
echo "===== SYSCALL COPY (SEQUENTIAL baseline) ====="
echo "Elapsed time:"
time bash -c '
for i in $(seq 1 10); do
  ./sys_mt src/file$i.bin out_sys_seq/file$i.bin
done
'

echo
echo "===== SYSCALL COPY (THREADED) ====="
echo "Elapsed time:"
time ./sys_mt "${ARGS_SYS_THR[@]}"

echo
echo "Done."
