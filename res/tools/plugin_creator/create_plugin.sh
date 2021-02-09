#!/bin/bash

CURDIR=`/bin/pwd`
BASEDIR=$(dirname $0)
ABSPATH=$(readlink -f $0)
ABSDIR=$(dirname $ABSPATH)
PATH_TEMP=${ABSDIR}/template_plugin
# . ${ABSDIR}/../path.sh
PATH_SOURCE=$(readlink -f ${ABSDIR}/../../..)

if [ $# -ne 1 ]; then
	print "Please provide the name of the plugin you want to create"
	exit 255
fi

NEW_PATH=${PATH_SOURCE}/tfa-plugins/${1}

cp -r $PATH_TEMP $NEW_PATH

sed -i 's/@@PlugName@@/'${1}'/g' $NEW_PATH/inc/template_plugin/template_plugin.h
sed -i 's/@@PlugName@@/'${1}'/g' $NEW_PATH/src/template_plugin.cpp
sed -i 's/@@PlugName@@/'${1}'/g' $NEW_PATH/CMakeLists.txt

mv $NEW_PATH/inc/template_plugin/template_plugin.h $NEW_PATH/inc/template_plugin/${1}.h
mv $NEW_PATH/inc/template_plugin $NEW_PATH/inc/${1}
mv $NEW_PATH/src/template_plugin.cpp $NEW_PATH/src/${1}.cpp



