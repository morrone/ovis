#! /bin/sh

# create config dir if not exist
test -d config || mkdir config

autoreconf --install --force
