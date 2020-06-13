#!/bin/bash
for i in {1..5}
do
	./fft_block 
	sleep 5
	./fft_block
	sleep 5
	./fft_block
	sleep 5
    python3 process.py
    sleep 1
done	