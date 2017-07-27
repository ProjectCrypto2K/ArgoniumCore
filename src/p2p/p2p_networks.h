// Copyright (c) 2012-2013 The Argoniumcore developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

namespace nodetool
{
#ifndef TESTNET
  const static boost::uuids::uuid P2P_NETWORK_ID = { { 0xcb, 0x25, 0xef, 0x14, 0xe, 0xfb , 0x37, 0x8, 0xd7, 0x14, 0x25, 0xe3, 0xcb, 0x25, 0xef, 0x14 } }; //edit: This identifier is used for network packages in order not to mix two different cryptocoin networks. Change all bytes to random values for your network:
#else
  const static boost::uuids::uuid P2P_NETWORK_ID = { { 0xcb, 0x25, 0xef, 0x14, 0xe, 0xfb , 0x37, 0x8, 0xd7, 0x14, 0x25, 0xe3, 0x2, 0x25, 0xef, 0x14 } }; //edit: This identifier is used for network packages in order not to mix two different cryptocoin networks. Change all bytes to random values for your network:
#endif
}
