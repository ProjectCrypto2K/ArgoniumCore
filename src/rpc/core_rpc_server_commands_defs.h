// Copyright (c) 2012-2013 The Argoniumcore developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once
#include "currency_protocol/currency_protocol_defs.h"
#include "currency_core/currency_basic.h"
#include "currency_core/difficulty.h"
#include "crypto/hash.h"
#include "p2p/p2p_protocol_defs.h"
#include "rpc/mining_protocol_defs.h"
#include "storages/portable_storage_base.h"

namespace currency
{
  //-----------------------------------------------
#define CORE_RPC_STATUS_OK          "OK"
#define CORE_RPC_STATUS_BUSY        "BUSY"
#define CORE_RPC_STATUS_NOT_FOUND   "NOT FOUND"
#define CORE_RPC_STATUS_FAILED        "FAILED"


  struct alias_rpc_details_base
  {
    std::string address;
    std::string tracking_key;
    std::string comment;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(address)
      KV_SERIALIZE(tracking_key)
      KV_SERIALIZE(comment)
    END_KV_SERIALIZE_MAP()
  };

  struct alias_rpc_details
  {
    std::string alias;
    alias_rpc_details_base details;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(alias)
      KV_CHAIN_MAP(details)
    END_KV_SERIALIZE_MAP()
  };

  struct tx_info
  {
	  std::string id_hash;
	  std::string tx_json;
	  uint64_t blob_size;
	  uint64_t fee;
	  std::string max_used_block_id_hash;
	  uint64_t max_used_block_height;
	  bool kept_by_block;
	  uint64_t last_failed_height;
	  std::string last_failed_id_hash;
	  uint64_t receive_time;

	  BEGIN_KV_SERIALIZE_MAP()
		  KV_SERIALIZE(id_hash)
		  KV_SERIALIZE(tx_json)
		  KV_SERIALIZE(blob_size)
		  KV_SERIALIZE(fee)
		  KV_SERIALIZE(max_used_block_id_hash)
		  KV_SERIALIZE(max_used_block_height)
		  KV_SERIALIZE(kept_by_block)
		  KV_SERIALIZE(last_failed_height)
		  KV_SERIALIZE(last_failed_id_hash)
		  KV_SERIALIZE(receive_time)
		  KV_SERIALIZE(last_failed_id_hash)
	  END_KV_SERIALIZE_MAP()
  };

  struct spent_key_image_info
  {
	  std::string id_hash;
	  std::vector<std::string> txs_hashes;

	  BEGIN_KV_SERIALIZE_MAP()
		  KV_SERIALIZE(id_hash)
		  KV_SERIALIZE(txs_hashes)
	  END_KV_SERIALIZE_MAP()
  };

  struct COMMAND_RPC_GET_HEIGHT
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      uint64_t 	 height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_GET_BLOCKS_FAST
  {

    struct request
    {
      std::list<crypto::hash> block_ids; //*first 10 blocks id goes sequential, next goes in pow(2,n) offset, like 2, 4, 8, 16, 32, 64 and so on, and the last one is always genesis block */

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(block_ids)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::list<block_complete_entry> blocks;
      uint64_t    start_height;
      uint64_t    current_height;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(blocks)
        KV_SERIALIZE(start_height)
        KV_SERIALIZE(current_height)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_TRANSACTIONS
  {
    struct request
    {
      std::list<std::string> txs_hashes;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs_hashes)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::list<std::string> txs_as_hex;  //Transaction blobs as hex
      std::list<std::string> txs_as_json; //Transaction blobs translated from hex to JSON
      std::list<std::string> missed_tx;   //not found transactions
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs_as_hex)
        KV_SERIALIZE(txs_as_json)
        KV_SERIALIZE(missed_tx)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_TX_POOL
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::list<blobdata> txs;  //transactions blobs
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_CHECK_KEYIMAGES
  {
    struct request
    {
      std::vector<crypto::key_image> images;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(images)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::vector<bool> images_stat;  //true - unspent, false - spent
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(images_stat)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_CHECK_KEYIMAGES_NONBINARY
  {
	  enum STATUS {
		  UNSPENT = 0,
		  SPENT_IN_BLOCKCHAIN = 1,
		  SPENT_IN_POOL = 2,
	  };

	  struct request
	  {
		  std::vector<std::string> images;
		  BEGIN_KV_SERIALIZE_MAP()
			  KV_SERIALIZE(images)
		  END_KV_SERIALIZE_MAP()
	  };

	  struct response
	  {
		  std::vector<int> image_statuses;
		  std::string status;

		  BEGIN_KV_SERIALIZE_MAP()
			  KV_SERIALIZE(image_statuses)
			  KV_SERIALIZE(status)
		  END_KV_SERIALIZE_MAP()
	  };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_TX_GLOBAL_OUTPUTS_INDEXES
  {
    struct request
    {
      crypto::hash txid;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_VAL_POD_AS_BLOB(txid)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::vector<uint64_t> o_indexes;
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(o_indexes)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS
  {
    struct request
    {
      std::list<uint64_t> amounts;
      uint64_t            outs_count;
      bool                use_forced_mix_outs;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amounts)
        KV_SERIALIZE(outs_count)
        KV_SERIALIZE(use_forced_mix_outs)
      END_KV_SERIALIZE_MAP()
    };

#pragma pack (push, 1)
    struct out_entry
    {
      uint64_t global_amount_index;
      crypto::public_key out_key;
    };
#pragma pack(pop)

    struct outs_for_amount
    {
      uint64_t amount;
      std::list<out_entry> outs;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(amount)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(outs)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::vector<outs_for_amount> outs;
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(outs)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_SET_MAINTAINERS_INFO
  {
    typedef nodetool::maintainers_entry request;

    struct response
    {
      std::string status;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  //-----------------------------------------------
  struct COMMAND_RPC_SEND_RAW_TX
  {
    struct request
    {
      std::string tx_as_hex;

      request() {}
      explicit request(const transaction &);

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(tx_as_hex)
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_START_MINING
  {
    struct request
    {
      std::string miner_address;
      uint64_t    threads_count;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(miner_address)
        KV_SERIALIZE(threads_count)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_GET_INFO
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    enum
    {
      daemon_network_state_connecting = 0,
      daemon_network_state_synchronizing = 1,
      daemon_network_state_online = 2
    };

    struct response
    {
      std::string status;
      uint64_t height;
      uint64_t difficulty;
      uint64_t tx_count;
      uint64_t tx_pool_size;
      uint64_t alt_blocks_count;
      uint64_t outgoing_connections_count;
      uint64_t incoming_connections_count;
      uint64_t white_peerlist_size;
      uint64_t grey_peerlist_size;
      uint64_t current_blocks_median;
      uint64_t current_network_hashrate_50;
      uint64_t current_network_hashrate_350;
      uint64_t scratchpad_size;
      uint64_t alias_count;
      uint64_t daemon_network_state;
      uint64_t synchronization_start_height;
      uint64_t max_net_seen_height;
      uint64_t transactions_cnt_per_day;
      uint64_t transactions_volume_per_day;
      nodetool::maintainers_info_external mi;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(height)
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(tx_count)
        KV_SERIALIZE(tx_pool_size)
        KV_SERIALIZE(alt_blocks_count)
        KV_SERIALIZE(outgoing_connections_count)
        KV_SERIALIZE(incoming_connections_count)
        KV_SERIALIZE(white_peerlist_size)
        KV_SERIALIZE(grey_peerlist_size)
        KV_SERIALIZE(current_blocks_median)      
        KV_SERIALIZE(current_network_hashrate_50)
        KV_SERIALIZE(current_network_hashrate_350)
        KV_SERIALIZE(scratchpad_size)
        KV_SERIALIZE(alias_count)
        KV_SERIALIZE(daemon_network_state)
        KV_SERIALIZE(synchronization_start_height)
        KV_SERIALIZE(max_net_seen_height)
        KV_SERIALIZE(transactions_cnt_per_day)
        KV_SERIALIZE(transactions_volume_per_day)
        KV_SERIALIZE(mi)
      END_KV_SERIALIZE_MAP()
    };
  };    
  //-----------------------------------------------
  struct COMMAND_RPC_GET_PEERLISTS
  {
	struct request
	{
		BEGIN_KV_SERIALIZE_MAP()
		END_KV_SERIALIZE_MAP()
	};

	struct response
	{
		std::string status;
		std::list<nodetool::peerlist_entry> white_list;
		std::list<nodetool::peerlist_entry> gray_list;

		BEGIN_KV_SERIALIZE_MAP()
			KV_SERIALIZE(status)
			KV_SERIALIZE(white_list)
			KV_SERIALIZE(gray_list)
		END_KV_SERIALIZE_MAP()
	};
  };
  //-----------------------------------------------
  struct COMMAND_RPC_STOP_DAEMON
  {
	  struct request
	  {
		  BEGIN_KV_SERIALIZE_MAP()
		  END_KV_SERIALIZE_MAP()
	  };

	  struct response
	  {
		  std::string status;

		  BEGIN_KV_SERIALIZE_MAP()
			  KV_SERIALIZE(status)
		  END_KV_SERIALIZE_MAP()
	  };
  };
  //-----------------------------------------------
  struct COMMAND_RPC_STOP_MINING
  {
    struct request
    {

      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };


    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };


  //
  struct COMMAND_RPC_GETBLOCKCOUNT
  {
    typedef std::list<std::string> request;

    struct response
    {
      uint64_t count;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(count)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GETBLOCKHASH
  {
    typedef std::vector<uint64_t> request;

    typedef std::string response;
  };


  struct COMMAND_RPC_GETBLOCKTEMPLATE
  {
    struct request
    {
      uint64_t reserve_size;       //max 255 bytes
      std::string wallet_address;
      alias_rpc_details alias_details;
      epee::serialization::storage_entry dev_bounties_vote;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(reserve_size)
        KV_SERIALIZE(alias_details)
        KV_SERIALIZE(wallet_address)        
        KV_SERIALIZE(dev_bounties_vote)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      uint64_t difficulty;
      uint64_t height;
      uint64_t reserved_offset;
      blobdata blocktemplate_blob;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(height)
        KV_SERIALIZE(reserved_offset)
        KV_SERIALIZE(blocktemplate_blob)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_SUBMITBLOCK
  {
    typedef std::vector<std::string> request;
    
    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  struct block_header_response
  {
      uint8_t major_version;
      uint8_t minor_version;
      uint64_t timestamp;
      std::string prev_hash;
      uint64_t nonce;
      bool orphan_status;
      uint64_t height;
      uint64_t depth;
      std::string hash;
      difficulty_type difficulty;
      uint64_t reward;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(major_version)
        KV_SERIALIZE(minor_version)
        KV_SERIALIZE(timestamp)
        KV_SERIALIZE(prev_hash)
        KV_SERIALIZE(nonce)
        KV_SERIALIZE(orphan_status)
        KV_SERIALIZE(height)
        KV_SERIALIZE(depth)
        KV_SERIALIZE(hash)
        KV_SERIALIZE(difficulty)
        KV_SERIALIZE(reward)
      END_KV_SERIALIZE_MAP()
  };
  
  struct COMMAND_RPC_GET_LAST_BLOCK_HEADER
  {
    typedef std::list<std::string> request;

    struct response
    {
      std::string status;
      block_header_response block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };
  
  struct COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH
  {
    struct request
    {
      std::string hash;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(hash)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_response block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT
  {
    struct request
    {
      uint64_t height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(height)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;
      block_header_response block_header;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(block_header)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };

  };

  struct COMMAND_RPC_GET_BLOCK
  {
     struct request
     {
       std::string hash;
       uint64_t height;

       BEGIN_KV_SERIALIZE_MAP()
         KV_SERIALIZE(hash)
         KV_SERIALIZE(height)
       END_KV_SERIALIZE_MAP()
     };

     struct response
     {
       std::string status;
       block_header_response block_header;
       std::vector<std::string> tx_hashes;
       std::string blob;
       std::string json;

       BEGIN_KV_SERIALIZE_MAP()
         KV_SERIALIZE(block_header)
         KV_SERIALIZE(tx_hashes)
         KV_SERIALIZE(status)
         KV_SERIALIZE(blob)
         KV_SERIALIZE(json)
       END_KV_SERIALIZE_MAP()
     };

  };

  struct COMMAND_RPC_GET_CONNECTIONS
	{
	  struct request
	  {
		BEGIN_KV_SERIALIZE_MAP()
		END_KV_SERIALIZE_MAP()
	  };

	  struct response
	  {
		std::string status;
		std::list<connection_info> connections;

		BEGIN_KV_SERIALIZE_MAP()
		  KV_SERIALIZE(status)
		  KV_SERIALIZE(connections)
		END_KV_SERIALIZE_MAP()
	  };
	};

  struct COMMAND_RPC_GET_ALIAS_DETAILS
  {
    struct request
    {
      std::string alias;
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(alias)
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      alias_rpc_details_base alias_details;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(alias_details)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };


  struct COMMAND_RPC_GET_ALL_ALIASES
  {
    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::list<alias_rpc_details> aliases;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(aliases)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };


  struct COMMAND_RPC_GET_ALIASES_BY_ADDRESS
  {

    typedef std::string request;

    struct response
    {
      std::string alias;
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(alias)
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };


  struct COMMAND_RPC_GET_ADDENDUMS
  {

    typedef mining::height_info request;

    struct response
    {
      std::string status;
      std::list<mining::addendum> addms;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
        KV_SERIALIZE(addms)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct COMMAND_RPC_RESET_TX_POOL
  {

    struct request
    {
      BEGIN_KV_SERIALIZE_MAP()
      END_KV_SERIALIZE_MAP()
    };

    struct response
    {
      std::string status;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(status)
      END_KV_SERIALIZE_MAP()
    };
  };


}

