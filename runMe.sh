#!/bin/bash

#set -x
g++ trivial-shell.cpp -o trivial-shell
chmod +x trivial-shell
./trivial-shell < trivial.script