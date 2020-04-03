#!/bin/sh
#
# File: autogen.sh
# Date: 2020-04-03
# By  : Kevin L. Esteb
#
# rebuild the configuration
#
automake --add-missing \
&& autoconf

