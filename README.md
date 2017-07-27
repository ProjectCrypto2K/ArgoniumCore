# ArgoniumCore forking how-to

### First step. Give a name to your coin

**Good name must be unique.** Check uniqueness with [google](http://google.com) and [Map of Coins](mapofcoins.com) or any other similar service.

Name must be specified twice:

**1. in file src/currency_config.h** - `CURRENCY_NAME_BASE` constant

Example: 
```
define CURRENCY_NAME_BASE "ArgoniumCore"
```

**2. in src/CMakeList.txt file** - set_property(TARGET daemon PROPERTY OUTPUT_NAME "argoniumcore**d**")

Example: 
```
set_property(TARGET daemon PROPERTY OUTPUT_NAME "argoniumcored")
```

**Note:** You should also change a repository name.


### Second step. Logic 

**1. Total money supply** (src/currency_config.h)

Total amount of coins to be emitted. Most of ArgoniumCore based coins use `(uint64_t)(-1)` (equals to 18446744073709551616). You can define number explicitly (for example `UINT64_C(858986905600000000)`).

Example:
```
define TOTAL_MONEY_SUPPLY (uint64_t)(-1)
```

**2. Difficulty target** (src/currency_config.h)

Difficulty target is an ideal time period between blocks. In case an average time between blocks becomes less than difficulty target, the difficulty increases. Difficulty target is measured in seconds.

Difficulty target directly influences several aspects of coin's behavior:

- transaction confirmation speed: the longer the time between the blocks is, the slower transaction confirmation is
- emission speed: the longer the time between the blocks is the slower the emission process is
- orphan rate: chains with very fast blocks have greater orphan rate

For most coins difficulty target is 60 or 120 seconds.

Example:
```
define DIFFICULTY_TARGET 120
```


### Third step. Networking

**1. Default ports for P2P and RPC networking** (src/currency_config.h)

P2P port is used by daemons to talk to each other through P2P protocol.
RPC port is used by wallet and other programs to talk to daemon.

It's better to choose ports that aren't used by other software or coins. See known TCP ports lists:

* https://en.wikipedia.org/wiki/Internet_protocol_suite

Example:
```
define P2P_DEFAULT_PORT 836 
define RPC_DEFAULT_PORT 846

define P2P_DEFAULT_PORT 48654
define RPC_DEFAULT_PORT 48854
```


**2. Network identifier** (src/p2p/p2p_networks.h)

This identifier is used in network packages in order not to mix two different cryptocoin networks. Change all the bytes to random values for your network:
```
const static boost::uuids::uuid P2P_NETWORK_ID = { { 0xcb, 0x25, 0xef, 0x14, 0xe, 0xfb , 0x37, 0x8, 0xd7, 0x14, 0x25, 0xe3, 0xcb, 0x25, 0xef, 0x14 } };

const static boost::uuids::uuid P2P_NETWORK_ID = { { 0xcb, 0x25, 0xef, 0x14, 0xe, 0xfb , 0x37, 0x8, 0xd7, 0x14, 0x25, 0xe3, 0xcb, 0x25, 0xef, 0x14 } };
```


**3. Seed nodes** (src/p2p/net_node.inl)

Add IP addresses of your seed nodes.

Example:
```
ifndef TESTNET
    ADD_HARDCODED_SEED_NODE("195.54.162.157:" STRINGIFY_EXPAND(P2P_DEFAULT_PORT));
else
    ADD_HARDCODED_SEED_NODE("195.54.162.157:" STRINGIFY_EXPAND(P2P_DEFAULT_PORT));
endif
```


### Fourth step. Address prefix

You may choose a letter (in some cases several letters) all the coin's public addresses will start with. It is defined by `CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX` constant. Since the rules for address prefixes are nontrivial you may use the [prefix generator tool](https://cryptonotestarter.org/tools.html).

Example:
```
define CURRENCY_PUBLIC_ADDRESS_BASE58_PREFIX 0x2cca
```


### Fifth step. Genesis block

**1. Build the binaries with blank genesis tx hex** (src/currency_core/currency_format_utils.cpp)

You should leave `const char GENESIS_COINBASE_TX_HEX[]` blank and compile the binaries without it.

Example:
```
ifndef TESTNET
    std::string genesis_coinbase_tx_hex = "";
else 
    std::string genesis_coinbase_tx_hex = "";                                          
endif
```


**2. Start the daemon to print out the genesis block**

Run your daemon with `--print-genesis-tx` argument. It will print out the genesis block coinbase transaction hash.

Example:
```
furiouscoind --print-genesis-tx
```


**3. Copy the printed transaction hash** (src/currency_core/currency_format_utils.cpp)

Copy the tx hash that has been printed by the daemon to `GENESIS_COINBASE_TX_HEX` in `src/currency_core/currency_format_utils.cpp`

Example:
```
ifndef TESTNET
    std::string genesis_coinbase_tx_hex = "21l3j123jnjh12n3hbhldcn...ajsdny347hr73329jr32f";
else 
    std::string genesis_coinbase_tx_hex = "21l3j123jnjh12n3hbhldcn...ajsdny347hr73329jr32f";                                          
endif
```


**4. Recompile the binaries**

Recompile everything again. Your coin code is ready now. Make an announcement for the potential users and enjoy!


## Building

### Unix and MacOS X

Dependencies: GCC 4.7.3 or later, CMake 2.8.6 or later, and Boost 1.53(but don't use 1.54) or later. You may download them from:
http://gcc.gnu.org/
http://www.cmake.org/
http://www.boost.org/
Alternatively, it may be possible to install them using a package manager.

More detailed instructions for OS X (assume you’re using MacPorts (they’re, however, pretty self-explanatory and homebrew users shouldn't have troubles following it too):

* Install latest Xcode and command line tools (these are in fact MacPorts prerequisites)
* Install GCC 4.8 and toolchain selector tool: `sudo port install gcc48 gcc_select`
* Set GCC 4.8 as an active compiler toolchain: `sudo port select --set gcc mp-gcc48`
* Install latest Boost, CMake: `sudo port install boost cmake`

To build, change to a directory where this file is located, and run `make`. The resulting executables can be found in `build/release/src`.

**Advanced options**:

Parallel build: run `make -j<number of threads>` instead of just `make`.

Debug build: run `make build-debug`.

Test suite: run `make test-release` to run tests in addition to building. Running `make test-debug` will do the same to the debug version.

Building with Clang: it may be possible to use Clang instead of GCC, but this may not work everywhere. To build, run `export CC=clang CXX=clang++` before running `make`.

### Windows

Download and install Microsoft Visual Studio 2013 from: https://www.microsoft.com/en-us/download/details.aspx?id=44914

Download and install CMAKE from: https://cmake.org/download/ 

Download and install Boost from: https://sourceforge.net/projects/boost/files/boost-binaries/
 - Specifically for Boost, download and install at least version 1.53 or later (but not 1.54 or 1.55) (Boost 1.56 will be used in this    tutorial)
 - Install Boost to C:\boost\boost_1_56_0 or some other directory you wish to put it in. Be sure to navigate to your boost_1_56_0 folder and rename the library where the static libraries are stored from "lib64-msvc-12.0" to "lib". This will ensure that cmake can find the libraries using the -DBOOST_ROOT option in the commands below.

Next you need to open up the VS2013 Command Prompt found here: "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x64 Cross Tools Command Prompt"

Inside the command prompt navigate to the home directory of the ArgoniumCore source code.

Next run the following commands:
```bash
 mkdir build
 cd build
 cmake -DBOOST_ROOT=C:\boost\boost_1_56_0 -G "Visual Studio 12 Win64" ..
 msbuild.exe argoniumcore.sln /p:Configuration=Release /m
```
Note: The /m is optional on the last line and it will direct msbuild to perform a multi-core build. Sometimes this speeds things up.

This resulting executables can be found here: \build\src\Release
