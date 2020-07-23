#define VACCOUNTS_SUBSCRIBER
#define VACCOUNTS_DELAYED_CLEANUP 120

#include <string>
#include "./dist/zeus_boxes/contracts/eos/dappservices/vaccounts.hpp"
#include "./dist/zeus_boxes/contracts/eos/dappservices/ipfs.hpp"
#include "./dist/zeus_boxes/contracts/eos/dappservices/multi_index.hpp"
#include "./dist/zeus_boxes/contracts/eos/dappservices/oracle.hpp"

#define DAPPSERVICES_ACTIONS()  \
  XSIGNAL_DAPPSERVICE_ACTION    \
  IPFS_DAPPSERVICE_ACTIONS      \
  VACCOUNTS_DAPPSERVICE_ACTIONS \
  ORACLE_DAPPSERVICE_ACTIONS
#define DAPPSERVICE_ACTIONS_COMMANDS() \
  IPFS_SVC_COMMANDS()                  \
  VACCOUNTS_SVC_COMMANDS()             \
  ORACLE_SVC_COMMANDS()

#define CONTRACT_NAME() dapptokencont

#define HOST "dappaccoun.t"_n

using namespace eosio;

using std::string;

CONTRACT dapptokencont : public eosio::contract
{
  using contract::contract;

public:
  DAPPSERVICES_ACTIONS()

  struct transfer_struct
  {
    name vaccount;
    name to;
    asset quantity;
    std::string memo;
    EOSLIB_SERIALIZE(transfer_struct, (vaccount)(to)(quantity)(memo));
  };

  struct withdraw_struct
  {
    name vaccount;
    name to;
    asset quantity;
    std::string memo;
    std::string authcode;
    uint64_t levelid;
    std::string authtype;
    EOSLIB_SERIALIZE(withdraw_struct, (vaccount)(to)(quantity)(memo)(authcode)(levelid)(authtype))
  };

  [[eosio::action]] void transfervacc(transfer_struct payload);
  [[eosio::action]] void transferacc(transfer_struct payload);
  [[eosio::action]] void withdraw(withdraw_struct payload);

  TABLE settings
  {
    asset withdraw_limit;
  };
  typedef singleton<"configsett5"_n, settings> settings_index;
  typedef eosio::multi_index<"configsett5"_n, settings> dummy_for_abi;

  TABLE accountbal
  {
    asset balance;
    name tokencontract;

    uint64_t primary_key() const { return balance.symbol.code().raw(); }
  };

  typedef dapp::multi_index<"accountblc1"_n, accountbal> accountbal_tab;
  typedef eosio::multi_index<".accountblc1"_n, accountbal> accountbal_tab_v_abi;

  TABLE shardbucket
  {
    std::vector<char> shard_uri;
    uint64_t shard;
    uint64_t primary_key() const { return shard; }
  };

  typedef eosio::multi_index<"accountblc1"_n, shardbucket> accountbal_tab_abi;

private:
  void sub_balance(name owner, asset value);
  void add_balance(name owner, asset value, name tokencontract);
  
  VACCOUNTS_APPLY(((transfer_struct)(transfervacc))((transfer_struct)(transferacc))((withdraw_struct)(withdraw)))
};