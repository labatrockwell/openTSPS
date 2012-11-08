#!/bin/sh

# this should be a cmd line option
VERSION_NUMBER='1.2.4'
BASEDIR=$(cd "$(dirname "$0")"; pwd)
BASIC_DIR='../standalone/basic'
MULTICAM_DIR='../standalone/multicamera'
OPENNI_DIR='../standalone/openNI'
EXAMPLES_DIR='$BASEDIR/../'

BASIC="$BASEDIR/$BASIC_DIR"
MULTICAM="$BASEDIR/$MULTICAM_DIR"
OPENNI="$BASEDIR/$OPENNI_DIR"

# Make build dir
makeInstallDir(){
	echo $VERSION_NUMBER
	cd $BASEDIR
	mkdir '../releases'
	cd '../releases'
	mkdir $VERSION_NUMBER
}

printUsage() {
    # Print the usage string and exit with $1.
    echo "usage:"
    echo "	-v set version number"
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
	cp -r "bin" "TSPS"
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_mac_$VERSION_NUMBER.zip" "TSPS" -x "TSPS/freenect.lib"
	rm -r "TSPS"

	echo 'building '$MULTICAM
	cd $MULTICAM
	xcodebuild 
	cd $MULTICAM
	cp -r "bin" "TSPS"
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_multicamera_mac_$VERSION_NUMBER.zip" "TSPS" -x "TSPS/freenect.lib"
	rm -r "TSPS"

	echo 'building '$OPENNI
	cd $OPENNI
	xcodebuild 
	cd $OPENNI
	cp -r "bin" "TSPS"
	zip -r "$BASEDIR/../releases/$VERSION_NUMBER/tsps_openni_mac_$VERSION_NUMBER.zip" "TSPS" -x "TSPS/freenect.lib"
	rm -r "TSPS"
}

# WIN
buildWin(){
	ls
}

# LINUXN
buildLinux(){
	ls
}

# EXAMPLES_DIR
buildExamples(){
	# 1 - build P5 library
	cd $BASEDIR
	cd "../examples/processing/library/workspace/TSPS/resources/"
	ant
	rm -r "$BASEDIR/../examples/processing/library/TSPS" 
	mkdir "$BASEDIR/../examples/processing/library/TSPS" 
	mv -f "$BASEDIR/../examples/processing/library/workspace/TSPS/distribution/TSPS-1/download/TSPS.zip" "$BASEDIR/../examples/processing/library/TSPS/TSPS.zip"
	cd "$BASEDIR/../examples/processing/library/TSPS"
	unzip TSPS.zip
	rm TSPS.zip

	# 2 - copy OF stuff
	cd $BASEDIR
	cd "../"
	cp -r "addons/ofxTSPSReceiver" "examples/openframeworks/ofxTSPSReceiver"
	rm -r "examples/openframeworks/ofxTSPSReceiver/.git"

	# 4 - zip everybody up
	cd $BASEDIR
	cd '../'
	zip -x@"build/exclude.lst" -r "$BASEDIR/../releases/$VERSION_NUMBER/examples_$VERSION_NUMBER.zip" "examples"
	rm -r "examples/openframeworks/ofxTSPSReceiver"
} 

#BUILD ALL (aka build all osx for now...)
buildAll(){
	makeInstallDir
	buildOSX
	buildLinux
	buildExamples
}

BALL='1'
BOSX='0'
BWIN='0'
BLINUX='0'
BEXAMPLES='0'

# Parse options
if [ ${#} == 1  ] ; then
	VERSION_NUMBER=$1
	buildAll
elif [ ${#} == 0  ] ; then
	buildAll
fi
# TO-DO: Fix this...
# else

# 	while [ $# -gt 0 ]
# 	do
# 	   case $1 in
# 	   		-v) set $VERSION_NUMBER='$1';;
# 			-h ) printUsage;;
# 			-m ) set $BOSX='1' 	
# 				set $BALL='0';;
# 			-w ) set $BLINUX='1' 
# 				set $BALL='0';;
# 			-l ) set $BWIN='1' 
# 				set $BALL='0';;
# 			-e ) set $BEXAMPLES='1' 
# 				set $BALL='0';;
# 	       	[?] ) echo 'Invalid option.';;
# 	   esac
# 	   shift
# 	done

# 	makeInstallDir

# 	if [[ $BALL == '1' ]]; then
# 		buildAll
# 	else

# 		if [[ $BOSX == '1' ]]; then
# 			buildOSX
# 		fi

# 		if [[ $BWIN == '1' ]]; then
# 			buildWin
# 		fi

# 		if [[ $BLINUX == '1' ]]; then
# 			buildLinux
# 		fi

# 		if [[ $BEXAMPLES == '1' ]]; then
# 			buildExamples
# 		fi
# 	fi
# fi
