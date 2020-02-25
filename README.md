# Faust Plugin for Moldeo

Faust compiler as a sound effect with visualization parameters

# Install and compile faust with full options including libfaust

## For Linux
```
git clone https://github.com/grame-cncm/faust.git --depth 1 --single-branch
cd faust
make -j8 all
sudo make install PREFIX=/usr
```

## Mac OSX Warning!!! Install llvm and llvm-config
```
install mac port
install llvm-X.Y where X is between 3 and 9
sudo port install llvm-9.0
sudo ln -sf /opt/local/bin/llvm-config-mp-9.0 /opt/local/bin/llvm-config
```
then install faust to /opt/local
```
sudo make install PREFIX=/opt/local
```

# Compile moSoundFaust plugin and install
```
./autogen.sh --prefix=/usr
make
sudo make install
```

# Parameters



# Example
