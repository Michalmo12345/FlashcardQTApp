#!/bin/bash

INSTALL_DIR="$(pwd)/libs"
PROJECT_DIR="$(pwd)/libs"
mkdir -p $INSTALL_DIR


function install_qt {
    QT_VERSION="5.15.13"
    QT_DIR="$INSTALL_DIR/qt-$QT_VERSION"
    mkdir -p $QT_DIR

   
    wget http://download.qt.io/official_releases/qt/5.15/$QT_VERSION/single/qt-everywhere-opensource-src-$QT_VERSION.tar.xz
    tar -xf qt-everywhere-opensource-src-$QT_VERSION.tar.xz
    cd qt-everywhere-src-$QT_VERSION

    # Configure the installation
    ./configure -prefix $QT_DIR \
                -opensource -confirm-license \
                -nomake examples -nomake tests \
                -no-compile-examples \
                -skip qtdoc \
                -skip qtandroidextras \
                -skip qtwebengine \
                -skip qtquickcontrols \
                -skip qtquickcontrols2 \
                -skip qtmultimedia \
                -skip qt3d \
                -skip qtspeech \
                -skip qtlocation \
                -skip qtsensors \
                -skip qtconnectivity \
                -skip qtdatavis3d \
                -skip qtpurchasing \
                -skip qtcharts \
                -skip qtvirtualkeyboard \
                -skip qtnetworkauth \
                -skip qtremoteobjects \
                -skip qtwebchannel \
                -skip qtwebsockets \
                -skip qtwebview \
                -skip qtwebglplugin \
                -skip qtwebengine \
                -skip qtxmlpatterns \
                -skip qtwebengine \

                -no-opengl \
                -release
    # Build and install
    make -j$(nproc)
    make install

    # Clean up
    cd ..
    rm -rf qt-everywhere-src-$QT_VERSION qt-everywhere-opensource-src-$QT_VERSION.tar.xz

    echo "Qt installed to $QT_DIR"
}


function install_postgresql {
    PG_VERSION="13.2"
    PG_DIR="$INSTALL_DIR/postgresql-$PG_VERSION"
    mkdir -p $PG_DIR

 
    wget https://ftp.postgresql.org/pub/source/v$PG_VERSION/postgresql-$PG_VERSION.tar.gz
    tar -zxvf postgresql-$PG_VERSION.tar.gz
    cd postgresql-$PG_VERSION
    
    ./configure --prefix=$PG_DIR --without-readline --without-zlib
    make -j$(nproc)
    make install

    # mkdir -p "$PROJECT_DIR/libpq/include"
    # mkdir -p "$PROJECT_DIR/libpq/lib"
    # cp -r "$PROJECT_DIR/postgresql/include/postgresql" "$PROJECT_DIR/libpq/include"
    # cp "$PROJECT_DIR/postgresql/lib/libpq.so"* "$PROJECT_DIR/libpq/lib"
    export PATH=$PG_DIR/bin:$PATH
    export LD_LIBRARY_PATH=$PG_DIR/lib:$LD_LIBRARY_PATH



    cd ..
    rm -rf postgresql-$PG_VERSION postgresql-$PG_VERSION.tar.gz
    echo "PostgreSQL installed to $PG_DIR"
}
function install_libpq
{

    git clone https://github.com/jtv/libpqxx.git "$INSTALL_DIR/libpqxx"
    cd "$INSTALL_DIR/libpqxx"
    ./configure --disable-documentation --prefix="$INSTALL_DIR"
    make -J$(nproc)
    make install

    echo "LIBPQXX installed succesfully"
}
# function install_qt_base
# {
#     git clone https://github.com/qt/qtbase.git "$INSTALL_DIR/"
# }
function install_xcb
{
    git clone https://gitlab.freedesktop.org/xorg/lib/libxcb.git

    cd libxcb

    ./autogen.sh --prefix=$(pwd)/install
    make
    make install
    
    export PKG_CONFIG_PATH=$(pwd)/install/lib/pkgconfig:$PKG_CONFIG_PATH
    export LD_LIBRARY_PATH=$(pwd)/install/lib:$LD_LIBRARY_PATH
}
install_qt
# install_postgresql
# install_libpq
# install_qt_base
# install_xcb