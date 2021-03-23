#!/usr/bin/env python

__author__ = "AUTHOR"
__version__ = "0.1.0"
__license__ = "MIT"

import sys
import argparse
from datetime import datetime
start=datetime.now()
version="0.1.0"
verbose=False

def main():
    global verbose
    parser = argparse.ArgumentParser(description="description of your script", epilog="Contact AUTHOR for more info")
    group = parser.add_mutually_exclusive_group()
    parser.add_argument("-i", "--in", type=str, default=None, help="input file")
    parser.add_argument("-o", "--out", type=str, default=None, help="output file")
    parser.add_argument("-n", "--num", type=int, default=None, help="some int value")
    parser.add_argument("-c", "--choice", type=int, choices=[0, 1, 2], help="choices to pick")
    group.add_argument("-q", "--quiet", action="store_true", default=False, help="suppress verbose")
    group.add_argument("--verbose", action="store_true", default=False, help="increase output verbosity")
    parser.add_argument("-v", "--version", action="store_true", default=False, help="show program version")
    args = parser.parse_args()
    if args.verbose:
        verbose=True
    if args.version:
        print("version: ", version)
    print(args.num)



if __name__ == '__main__':
    main()
    #print(__file__)
    if verbose:
        print("TAT: ", datetime.now()-start)
