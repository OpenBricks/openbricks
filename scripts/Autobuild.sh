#!/bin/sh

# $1 is target
# $2 is DOOZER_CONCURRENCY_MAKE_LEVEL

# In case it's the first time we try the build
mkdir -p /project/sources /project/stamps /project/build.host/.cache build/build.host build/config /project/.ccache

# Do not dowmnload if we have already the sources
ln -s /project/sources sources
ln -s /project/stamps .stamps

# Nothing for ccache, already embedded in our scripts

# Restore build.host, it saves 6-7 min 
echo "Restoring build.host..."
cp -r /project/build.host/ build/

# Just to see...
ls -al build/build.host
ls -al build/build.host/.cache
ls -al /project/sources
ls -al /project


echo "######################"
echo "# Starting the build #"
echo "######################"
./scripts/loadcfg $1
# Build with the maximum speed (maybe we should try too without MAKECFLAGS set)
echo DOOZER_CONCURRENCY_MAKE_LEVEL=$2 >> build/config/options-doozer
make

# keep build.host
if [ -n "$(ls -A /project/build.host)" ] ; then 
  echo "Nothing to save in /project/build.host"
else
 echo "Copying build/build.host to /project"
 cp -r build/build.host /project/
fi

# Should we publish our opkg files ?
