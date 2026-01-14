#!/bin/bash
set -e

echo "Compiling..."
gcc -pthread 1.c templateFiles.c -o func_mt
gcc -pthread 2.c templateFiles.c -o sys_mt

# Make folders
rm -rf src out_func_seq out_func_thr out_sys_seq out_sys_thr
mkdir -p src out_func_seq out_func_thr out_sys_seq out_sys_thr

# Create 10 test files (each 5MB) so timing is meaningful
echo "Generating 10 input files..."
for i in $(seq 1 10); do
  dd if=/dev/urandom of="src/file$i.bin" bs=1M count=5 status=none
done

# Build argument list: src/file1.bin out/.../file1.bin ...
ARGS_FUNC_SEQ=()
ARGS_FUNC_THR=()
ARGS_SYS_SEQ=()
ARGS_SYS_THR=()

for i in $(seq 1 10); do
  ARGS_FUNC_SEQ+=("src/file$i.bin" "out_func_seq/file$i.bin")
  ARGS_FUNC_THR+=("src/file$i.bin" "out_func_thr/file$i.bin")
  ARGS_SYS_SEQ+=("src/file$i.bin" "out_sys_seq/file$i.bin")
  ARGS_SYS_THR+=("src/file$i.bin" "out_sys_thr/file$i.bin")
done

echo
echo "===== FUNCTION COPY (SEQUENTIAL baseline) ====="
# sequential baseline: run the threaded program one file at a time
/usr/bin/time -f "Elapsed: %e sec" bash -c '
for i in $(seq 1 10); do
  ./func_mt src/file$i.bin out_func_seq/file$i.bin
done
'

echo
echo "===== FUNCTION COPY (THREADED) ====="
/usr/bin/time -f "Elapsed: %e sec" ./func_mt "${ARGS_FUNC_THR[@]}"

echo
echo "===== SYSCALL COPY (SEQUENTIAL baseline) ====="
/usr/bin/time -f "Elapsed: %e sec" bash -c '
for i in $(seq 1 10); do
  ./sys_mt src/file$i.bin out_sys_seq/file$i.bin
done
'

echo
echo "===== SYSCALL COPY (THREADED) ====="
/usr/bin/time -f "Elapsed: %e sec" ./sys_mt "${ARGS_SYS_THR[@]}"

echo
echo "Done. Outputs in: out_func_seq/ out_func_thr/ out_sys_seq/ out_sys_thr/"
