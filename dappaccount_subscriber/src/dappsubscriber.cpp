
#include "dappsubscriber.hpp"

void dappsubscriber::testdappact(hello_action payload)
{
  require_vaccount(payload.vaccount);

  //action logic
  check(payload.balance.is_valid(), "invalid quantity");
  check(payload.balance.amount > 0, "must transfer positive quantity");
}

void dappsubscriber::testtwofact(hello_twofa payload)
{
  action(
      permission_level{_self, "active"_n},
      HOST, "genoraclereq"_n,
      std::make_tuple(payload.vaccount, payload.authcode, payload.levelid, payload.authtype))
      .send();

  require_vaccount(payload.vaccount);
  /*
    contract action logic
  */
}

EOSIO_DISPATCH_SVC(dappsubscriber, (testdappact)(testtwofact))