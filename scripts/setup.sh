apt update
apt install cmake g++ -y
apt install zip wget curl git -y
wget https://github.com/rdfhdt/hdt-cpp/archive/v1.3.3.zip
unzip v1.3.3.zip
rm v1.3.3.zip 

echo "Installing hdt-cpp..."
apt install vim -y 
apt install autoconf -y
apt install libtool -y
apt install pkg-config -y
apt install libserd-0-0 libserd-dev -y
apt install zlib1g-dev -y
cd hdt-cpp-1.3.3;./autogen.sh;./configure;make -j2;make install; cd ..;rm -Rf hdt-cpp-1.3.3


echo "Install coverage..."
apt install gcovr -y

echo "Installing easy-logger..."
wget https://github.com/ahmad88me/easy-logger/archive/v1.0.zip
unzip v1.0.zip
rm  v1.0.zip
cd easy-logger-1.0;make install;cd ..;rm -Rf easy-logger-1.0

echo "Installing tabular-parser..."
wget https://github.com/ahmad88me/tabular-parser/archive/v1.2.zip
unzip v1.2.zip
rm v1.2.zip
cd tabular-parser-1.2;make install;cd ..;rm -Rf tabular-parser-1.2

echo "update linker caches..."
ldconfig
