#! /usr/bin/env python3

import argparse
import serial
import sys

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("addr", type=int, help="Start address")
    parser.add_argument("size", type=int, help="Size to read")
    parser.add_argument("output_file", help="Output file")
    parser.add_argument("-d", "--device", help="Serial port")

    return parser.parse_args()

def dump(serial_device, addr, size, output_file):
    ser = serial.Serial(serial_device, 19200)
    f = open(output_file, "wb")

    ser.write("DUMP {addr} {size}\r".format(addr=addr, size=size).encode())
    for i in range(0, size):
        f.write(ser.read(1))
        if i > 0 and i % 255 == 0:
            print('.', end='', flush=True)
    print()

    f.close()
    ser.close()

def main():
    args = parse_args()
    dump(args.device, args.addr, args.size, args.output_file)

if __name__ == "__main__":
    main()
