#!/bin/sh

# this should be a cmd line option
VERSION_NUMBER='1.2.4'
BASEDIR=$(dirname $0)
BASIC_DIR='../standalone/basic'
MULTICAM_DIR='../standalone/multicamera'
OPENNI_DIR='../standalone/openNI'
EXAMPLES_DIR='$BASEDIR/../'

BASIC="$BASEDIR/$BASIC_DIR"
MULTICAM="$BASEDIR/$MULTICAM_DIR"
OPENNI="$BASEDIR/$OPENNI_DIR"

# Make build dir
cd $BASEDIR
mkdir '../releases'
cd '../releases'
mkdir $VERSION_NUMBER

printUsage() {
    # Print the usage string and exit with $1.
    echo "usage:"
    echo "	-m build + zip up all mac binaries"
    echo "	-w build + zip up all windows binaries"
    echo "	-l build + zip up all linux binaries"
    echo "	-e build + zip up all examples"
    echo "	-h help"
    exit 1
}

# OS X
buildOSX(){
	echo 'building '$BASIC
	cd $BASIC
	xcodebuild 
	cd $BASIC
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_mac_$VERSION_NUMBER.zip" "bin"

	echo 'building '$MULTICAM
	cd $MULTICAM
	xcodebuild 
	cd $MULTICAM
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_multicamera_mac_$VERSION_NUMBER.zip" "bin"

	echo 'building '$OPENNI
	cd $OPENNI
	xcodebuild 
	cd $OPENNI
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_openni_mac_$VERSION_NUMBER.zip" "bin"
}

# WIN
buildWin(){
	ls
}

# LINUXN
buildLinux(){
	ls
}

# EXAMPLES
buildExamples(){
	# 1 - build P5 library
	cd $BASEDIR
	cd ../examples/processing/library/workspace/TSPS/resources/
	ant 
	mkdir "$BASEDIR/../examples/processing/library/Processing/libraries/TSPS" 
	mv -f "$BASEDIR/../examples/processing/library/workspace/TSPS/distribution/TSPS.zip" "$BASEDIR/../examples/processing/library/Processing/libraries/TSPS/TSPS.zip"
	cd "$BASEDIR/../examples/processing/library/Processing/libraries/TSPS"
	rm -r examples library reference src
	unzip TSPS.zip
	rm TSPS.zip
	cp "$BASEDIR/../examples/processing/library/workspace/libs/oscP5/oscP5.jar" "$BASEDIR/../examples/processing/library/Processing/libraries/TSPS/library/oscP5.jar"

	# 2 - zip everybody up
	cd $BASEDIR
	cd '../'
	zip -x "examples/processing/library/workspace/*" -r "$BASEDIR/../releases/$VERSION_NUMBER/examples_$VERSION_NUMBER.zip" "examples"
} 

#BUILD ALL (aka build all osx for now...)
buildAll(){
	buildOSX
	buildLinux
	buildExamples
}

# Parse options
if [ ${#} == 0  ] ; then
	buildAll
else
	while [ $# -gt 0 ]
	do
	   case $1 in
	       -h ) printUsage;;
	       -m ) buildOSX;;
	       -w ) buildWin;;
	       -l ) buildLinux;;
	       -e ) buildExamples;;
	       * ) echo "Invalid option.";;
	   esac
	   shift
	done
fi
