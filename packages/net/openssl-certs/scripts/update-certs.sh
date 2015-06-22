#!/bin/bash
##################################
# update-certs.sh
# Created Oct 5 2014
# Author: Joel Green
# Downloads SSL certficates from mozilla.org and installs them into geexbox
# Expired certficates are removed
# Useful when downloading from https sources to avoid certificate errors
##################################

set -e
set -u

certhost='http://mxr.mozilla.org'
certdir='/mozilla/source/security/nss/lib/ckfw/builtins'
file="certdata.txt"
url="$certhost$certdir/$file?raw=1"

WGET='/usr/bin/wget'
C_REHASH='/bin/c_rehash'
TMP='/tmp'
CHECK_PROGRAMS="$WGET $C_REHASH $TMP"
ERROR=0

for f in $CHECK_PROGRAMS; do
        if [[ ! -x $f ]]; then
                echo "missing $f"
                ERROR=1
        fi
done

if (( ERROR )); then
        exit 1
fi

cd /tmp
echo "Retrieving ssl certficates..."
$WGET -c --output-document $file $url
if [[ ! -f $file ]]; then
      echo "Unable to retrieve certificate file."
      exit 1
fi

unset certhost certdir url WGET C_REHASH TMP CHECK_PROGRAMS ERROR

echo "Reformatting certificates for openssl..."
make-ca.sh
echo "Removing expired certificates..."
remove-expired-certs.sh certs
echo "Copying..."
rm -f /etc/ssl/certs/*.pem
cp certs/*.pem /etc/ssl/certs
cat certs/*.pem > /etc/ssl/ca-bundle.crt
rm -rf certs BLFS-ca-bundle*
echo "Rehashing..."
c_rehash /etc/ssl/certs
echo "SSL certificates successfully installed."
