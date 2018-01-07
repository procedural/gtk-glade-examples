#!/bin/bash
cd "$(dirname -- "$(readlink -fn -- "${0}")")"

glib-compile-resources --generate-source --target=gresource.c gresource.xml
clang -o main gresource.c main.c -rdynamic -I../.. `pkg-config --cflags --libs gtk+-3.0`
rm gresource.c
