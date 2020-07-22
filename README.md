### Dappaccount Subscriber Model

This is a simple example of a subscriber contract which is using liquidApps services to achieve virtual account functionalities. The code sample contains two actions describing how dapps can authorize any action with virtual user's permission, i.e an action can be pushed with an account's permission without the need of actual EOS accounts.

The hpp file has included some dependencies for virtual account service from liquidapps. dapps need to clone the service files into their include folder and declare the include path accordingly.

How to include and use liquidapps library files can be found [here](https://docs.liquidapps.io/en/v2.0/developers/vaccounts-getting-started.html).

##### Contract explained

In the `.hpp` file, dapp needs to add the `#define VACCOUNTS_SUBSCRIBER` definition to specify that the contract will act as a subscriber model where the HOST is dappaccount contract.
Also add `#define HOST "dappaccoun.t"_n` definition in the `.hpp` file.
Every action which has the authorization of virtual users, must take its parameters as a payload structure, as defined in this sample code.
```
  struct hello_action
  {
    name vaccount;
    asset balance;
    uint64_t dummyid;

    EOSLIB_SERIALIZE(hello_action, (vaccount)(balance)(dummyid))
  };
```
where `name Vaccount` must be added in the structure where the action has the virtual account's permission.

##### Action testdappact
The first line of this action defines how we need to authorize the action with virtual user's permission. It is done by simply passing the virtual account name in the require_vaccount() method. To use this method, dapps must include the liquidapps library files with a correct path as stated above.

##### Action testtwofact  
This example action defines how to define an action of higher security level with two factor authentication - first factor is verifying user's email / contact with access code within smart contract (through an inline action call) and then with user's signature. If dapp doesn't want to use verification of email / phone while calling action, it can follow `testdappact` action only.  


**Note :** The user's keys, through which signature is generated and later verified in action like stated above, are always added by verifying email / contact at the time of user registration on dappaccount for every user. So, this depends on dapps and the security level of the action , that whether the access code verification is needed at the time of action call or not.

All the informations about level id and other parameters of this inline action call, can be found on the [docs](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing)
If this verification method is followed, dapps need to request for access code using dappaccount library, given in the [docs](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing) under section 3.2.e.

##### Steps needed before dapp can start using dappaccount
1. Deploy contract and give eosio.code permission to the account
2. Select and Stake DAPP to use DSP package for Vaccount service. Details Can be found [here](https://docs.liquidapps.io/en/v2.0/developers/vaccounts-getting-started.html#select-and-stake-dapp-for-dsp-package-dsp-portal-link) 

Dapps must stake to VRAM and VACCOUNT services to use this sbuscriber model. Please refer to this [liquidapps doc](https://docs.liquidapps.io/en/v2.0/developers/dsp-packages-and-staking.html) for a better understanding about available DSP packages.

**Note :** Please refer to the Section 3.3 of [docs](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing) before selecting any DSP.

3. Run `xvinit` as state [here](https://docs.liquidapps.io/en/v2.0/developers/vaccounts-getting-started.html#select-and-stake-dapp-for-dsp-package-dsp-portal-link)  

< if using VACCOUNTS_SUBSCRIBER>
```
export HOST_ACCOUNT_NAME=dappaccoun.t
cleos -u $DSP_ENDPOINT push action $DAPP_CONTRACT_ACCOUNT xvinit "[\"$HOST_ACCOUNT_NAME\"]" -p $DAPP_CONTRACT_ACCOUNT
```
DSP_ENDPOINT - it is specific to the DSP dapp has selected and staked to. **dappaccoun.t** is staked to **blockstartac** provider

4. Whitelist Contract on dappaccount contract It cannot be done with DAPP's permission now. It will need dappaccount's authorization. Please contact dappaccount team for the same. The whitelisting includes defining a quota which is done by dappaccount for now. Later it will be a automated process which can be initiated by dapp itself. Please find the  information about Quota below.
Contact through [Telegram](https://t.me/dappaccount)

5. Once the contract is whitelisted, DAPP needs to whitelist its action with the specific security level.
Information about security level and action whitelisting , can be found on the [docs](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing). Specifically , under **Section 2.3**

Dapps need to call action adddappact of dappaccoun.t contract
```
cleos -u $EOS_ENDPOINT push action $HOST_ACCOUNT_NAME adddappact '{"dappaccount":<DAPP_CONTRACT_ACCOUNT>, "levelid": <1 OR 2>, "actionlist": [action1, action2]}' -p $DAPP_CONTRACT_ACCOUNT
```
This action registers an array of action of the DAPP contract under a particular security level. If action1 needs less security, it can be marked as level 1 and high security action can be marked as level 2. Details on level id and action whitelisting can be found under **Section 2.2 and 2.3** of the [docs](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing)

After these steps , DAPP will be ready to register its users on dappaccount and then use their signatures to authorize action. 
How to register users on dappaccount and then user their signature to authorize dapp's action can be found [here](https://docs.google.com/document/d/1fB5kH-fLERaeF5XRJo33V0edBLPbIJ2UIEuxvmEyjQc/edit?usp=sharing) under **Section 3.2**



QUOTA - 

Quota is a limit set by dappaccount initially, and later can be increased by a dapp following certain steps. Those will be defined soon. Quota is a number which describes the number of transaction that can be performed in a day by a particular dapp. And here the transaction means action with virtual account's (dappaccount for user) permission (like described in this sample contract). The quota is reset in every 24 hours automatically. The registration or login / recovery does not come under any quota. That is unlimited.