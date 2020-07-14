#define VACCOUNTS_SUBSCRIBER
#define VACCOUNTS_DELAYED_CLEANUP 120

#include <string>
#include "./dist/zeus_boxes/contracts/eos/dappservices/vaccounts.hpp"


#define DAPPSERVICES_ACTIONS() \
  XSIGNAL_DAPPSERVICE_ACTION \
  VACCOUNTS_DAPPSERVICE_ACTIONS
#define DAPPSERVICE_ACTIONS_COMMANDS() \
  VACCOUNTS_SVC_COMMANDS()      

#define CONTRACT_NAME() dappsubscriber

#define HOST "dappaccoun.t"_n

using namespace eosio;

using std::string;

CONTRACT dappsubscriber : public eosio::contract
{
  using contract::contract;

public:
  DAPPSERVICES_ACTIONS()

  struct hello_action
  {
    name vaccount;
    asset balance;
    uint64_t dummyid;

    EOSLIB_SERIALIZE(hello_action, (vaccount)(balance)(dummyid))
  };

  struct hello_twofa
  {
    name vaccount;
    std::string authcode;  //access code sent to user's email / contact
    uint64_t levelid;       // 1 or 2, explained in docs. Refer to readme.md
    std::string authtype;   //"email" OR "phone" depends on level id, refer to readme.md
    uint64_t dummyid;       //dummy field 

    EOSLIB_SERIALIZE(hello_twofa, (vaccount)(authcode)(levelid)(authtype)(dummyid))
  };

  [[eosio::action]] void testdappact(hello_action payload);
  [[eosio::action]] void testtwofact(hello_twofa payload);

  
  VACCOUNTS_APPLY(((hello_action)(testdappact))((hello_twofa)(testtwofact)))
};