export CUR_PATH=$PWD
export CMAKE=cmake
export BLD_DIR=bld
if [ -d $BLD_DIR ]
then
    echo Removing build dir...
    rm -rf $BLD_DIR
fi

if [ -d "$BLD_DIR" ]
then
    echo "Cant remove" $BLD_DIR
    exit
fi

mkdir $BLD_DIR
cd $BLD_DIR
cmake ../




