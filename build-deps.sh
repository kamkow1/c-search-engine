#!/bin/sh
(cd onion; mkdir -p build; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make)
(cd snowball; make)
