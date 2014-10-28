#!/bin/sh
################################################################################
# Core (OpenSSL) environment variable.
#
# This file contains non-OpenELEC evironment variables as well as OpenELEC
# evironment variables that are not user defined.
#
# set OpenSSL for armv6, see also:
# http://archlinuxarm.org/forum/viewtopic.php?f=15&t=1694&start=20#p20813
# https://github.com/archlinuxarm/PKGBUILDs/issues/242#issuecomment-8426407
################################################################################

export OPENSSL_armcap=6
