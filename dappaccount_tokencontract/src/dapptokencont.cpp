
#include "dapptokencont.hpp"

void dapptokencont::transfervacc(transfer_struct payload)
{
  require_vaccount(payload.vaccount);
  check(payload.quantity.is_valid(), "invalid quantity");
  check(payload.quantity.amount > 0, "must transfer positive quantity");
  check(payload.memo.size() <= 256, "memo has more than 256 bytes");
  check(payload.vaccount != payload.to, "Cannot transfer to self");
  check_vacc(payload.to);

  accountbal_tab from_acnts(_self, payload.vaccount.value);
  auto &from = from_acnts.get(payload.quantity.symbol.code().raw(), "no balance object found");

  sub_balance(payload.vaccount, payload.quantity);
  add_balance(payload.to, payload.quantity, from.tokencontract);
}

void dapptokencont::transferacc(transfer_struct payload)
{
  require_vaccount(payload.vaccount);
  settings_index config(_self, _self.value);
  auto setitr = config.get();
  check(payload.quantity <= setitr.withdraw_limit, "Need two factor auth for withdrawal of quantity more than limit set. Call withdraw method");
  check(payload.quantity.is_valid(), "invalid quantity");
  check(payload.quantity.amount > 0, "must transfer positive quantity");
  check(payload.memo.size() <= 256, "memo has more than 256 bytes");
  check(is_account(payload.to), "to account does not exist");

  accountbal_tab from_acnts(_self, payload.vaccount.value);
  auto &from = from_acnts.get(payload.quantity.symbol.code().raw(), "no balance object found");

  sub_balance(payload.vaccount, payload.quantity);

  action(
      permission_level{get_self(), "active"_n},
      from.tokencontract, "transfer"_n,
      std::make_tuple(get_self(), payload.to, payload.quantity, payload.memo))
      .send();
}


void dapptokencont::withdraw(withdraw_struct payload)
{
  action(
      permission_level{_self, "active"_n},
      HOST, "genoraclereq"_n,
      std::make_tuple(payload.vaccount, payload.authcode, payload.levelid, payload.authtype))
      .send();
  require_vaccount(payload.vaccount);
  check(payload.quantity.is_valid(), "invalid quantity");
  check(payload.quantity.amount > 0, "must transfer positive quantity");
  check(payload.memo.size() <= 256, "memo has more than 256 bytes");
  check(is_account(payload.to), "to account does not exist");

  accountbal_tab from_acnts(_self, payload.vaccount.value);
  auto &from = from_acnts.get(payload.quantity.symbol.code().raw(), "no balance object found");

  sub_balance(payload.vaccount, payload.quantity);

  action(
      permission_level{get_self(), "active"_n},
      from.tokencontract, "transfer"_n,
      std::make_tuple(get_self(), payload.to, payload.quantity, payload.memo))
      .send();
}

void dapptokencont::sub_balance(name owner, asset value)
{
  accountbal_tab from_acnts(_self, owner.value);

  auto &from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
  check(from.balance.amount >= value.amount, "overdrawn balance");

  from_acnts.modify(from, _self, [&](auto &a) {
    a.balance -= value;
  });
}

void dapptokencont::add_balance(name owner, asset value, name tokencontract)
{
  accountbal_tab to_acnts(_self, owner.value);
  auto to = to_acnts.find(value.symbol.code().raw());
  if (to == to_acnts.end())
  {
    to_acnts.emplace(_self, [&](auto &a) {
      a.balance = value;
      a.tokencontract = tokencontract;
    });
  }
  else
  {
    to_acnts.modify(to, _self, [&](auto &a) {
      a.balance += value;
    });
  }
}

EOSIO_DISPATCH_SVC(dapptokencont, (transfervacc)(transferacc)(withdraw))