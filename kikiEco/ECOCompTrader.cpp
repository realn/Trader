#include "stdafx.h"

#include <random>

#include "ECOStorage.h"
#include "ECOCompDock.h"
#include "ECOCompMarket.h"
#include "ECOCompNavigation.h"

#include "ECOEntity.h"
#include "ECOUniverse.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompTrader.h"

namespace eco {
  static const auto COMP_TRADER_ID = L"Trader"_id;
  static const auto MAX_FUNDS = 10000000.0f;

  static std::random_device RAND_DEV;

  namespace xml {
    template<>
    void RegisterComponent<comp::CTrader>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_TRADER_ID.GetName());
    }
  }

  namespace comp {
    CTrader::CTrader(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_TRADER_ID)
      , mStorage(CStorage::ValuesT(), 200.0f)
      , mWallet(1.0f)
    {
    }

    CTrader::CTrader(std::shared_ptr<CEntity> parent, xml::CComponent const & component)
      : CTrader(parent) {}

    CTrader::~CTrader() {}

    void CTrader::Update(float const timeDelta) {
      if(mWaitTime < mMaxWainTime) {
        mWaitTime += timeDelta;
        return;
      }

      switch(mState) {
      case State::Find: FindTransaction(); break;
      case State::Buy:  BuyProducts(); break;
      case State::Sell: SellProducts(); break;
      default: break;
      }
    }

    void CTrader::PrintInfo(cb::ostream & stream) const {
      stream << L" TRADER ("s << mWallet.GetAmount() << L"):"s << std::endl;
      mStorage.PrintInfo(stream);
    }

    void CTrader::FindTransaction() {
      auto parent = GetParent();
      auto uni = parent->GetUniverse();
      auto& nav = parent->GetComponent<CNavigation>();

      UpdatePrices(*uni);
      UpdateTransactions(nav);

      if(mTransactions.empty()) {
        mWaitTime = 0.0f;
        return;
      }

      for(mTransIt = mTransactions.begin(); mTransIt != mTransactions.end(); mTransIt++) {
        //if(mTransIt->second.mBuy.mValue > mWallet.GetAmount()) {
        //  continue;
        //}
        if(RAND_DEV() % 10 == 0) {
          continue;
        }
        if(mTransIt->second.mSell.mMarket.lock()->GetComponent<CMarket>().GetStorage().GetEmpty() < 0.1f) {
          continue;
        }
        break;
      }

      if(mTransIt == mTransactions.end()) {
        mWaitTime = 0.0f;
        return;
      }

      mState = State::Buy;
      nav.SetTarget(mTransIt->second.mBuy.mMarket.lock());
    }

    void CTrader::BuyProducts() {
      if(mTransIt == mTransactions.end()) {
        mState = State::Find;
        return;
      }
      auto parent = GetParent();
      auto& nav = parent->GetComponent<CNavigation>();
      auto dock = nav.GetCurrentDock();
      if(!dock || dock != mTransIt->second.mBuy.mMarket.lock()) {
        mWaitTime = 0.0f;
        return;
      }

      auto& transData = mTransIt->second;

      auto& market = dock->GetComponent<CMarket>();
      auto& priceList = market.GetPriceList();

      auto value = priceList.GetValue(transData.mId, PriceType::SELL);
      if(value == 0.0f) {
        throw std::exception("SOMETHING WENT WRONG");
      }

      if((transData.mBuy.mValue - value) / transData.mBuy.mValue < -0.2f) {
        mState = State::Find;
        mWaitTime = 0.0f;
        return;
      }

      auto amount = market.GetStorage().GetProductAmount(transData.mId);
      if(amount <= 0) {
        mState = State::Find;
        return;
      }

      auto funds = mWallet.GetAmount();
      auto buyAmount = 0.0f;
      if(value > 0)
        buyAmount = std::min({ funds / value, amount, mStorage.GetSpace() });      

      if(buyAmount == 0.0f || !market.CanBuyProduct(transData.mId, buyAmount, mWallet, mStorage)) {
        mState = State::Find;
        return;
      }

      market.BuyProduct(transData.mId, buyAmount, mWallet, mStorage);
      mState = State::Sell;
      nav.SetTarget(transData.mSell.mMarket.lock());
    }

    void CTrader::SellProducts() {
      if(mTransIt == mTransactions.end()) {
        mState = State::Find;
        return;
      }
      auto parent = GetParent();
      auto& nav = parent->GetComponent<CNavigation>();
      auto dock = nav.GetCurrentDock();
      if(!dock || dock != mTransIt->second.mSell.mMarket.lock()) {
        mWaitTime = 0.0f;
        return;
      }

      auto& transData = mTransIt->second;

      auto& market = dock->GetComponent<CMarket>();
      auto& priceList = market.GetPriceList();

      auto value = priceList.GetValue(transData.mId, PriceType::BUY);
      if(value == 0.0f ||
         (value - transData.mSell.mValue) / transData.mSell.mValue < -0.2f) {
        UpdateSellTarget();
        nav.SetTarget(transData.mSell.mMarket.lock());
        return;
      }

      auto amount = mStorage.GetProductAmount(transData.mId);

      if(!market.CanSellProduct(transData.mId, amount, mStorage)) {
        UpdateSellTarget();
        nav.SetTarget(transData.mSell.mMarket.lock());
        return;
      }

      market.SellProduct(transData.mId, amount, mWallet, mStorage);
      mState = State::Find;

      if(mWallet.GetAmount() >= MAX_FUNDS) {
        // THIS GUY WON!
        mWallet = CWallet(1.0f);
      }
    }

    void CTrader::UpdatePrices(CUniverse const& uni) {
      auto marketEntities = uni.GetEntities<comp::CMarket, comp::CDock>();

      mPrices.clear();
      for(auto& entity : marketEntities) {
        auto& market = entity->GetComponent<comp::CMarket>();

        auto& priceList = market.GetPriceList();
        for(auto& price : priceList) {
          auto& data = mPrices[price.first];

          data[price.second.GetType()].insert(std::make_pair(price.second.GetValue(), entity));
        }
      }
    }

    void CTrader::UpdateTransactions(CNavigation& nav) {
      auto currentDock = nav.GetCurrentDock();
      mTransactions.clear();

      auto data = TransactionIdDatasT();
      auto maxId = CTransactionId{ 1.0f, 0, 0 };

      for(auto& priceProduct : mPrices) {

        for(auto& priceBuy : priceProduct.second[PriceType::SELL]) {
          for(auto& priceSell : priceProduct.second[PriceType::BUY]) {

            auto profit = priceSell.first - priceBuy.first;
            if(profit == 0.0f)
              continue;

            auto dist = nav.QueryDistance(currentDock, priceBuy.second.lock());
            auto travel = nav.QueryDistance(priceBuy.second.lock(), priceSell.second.lock());

            maxId.mProfit = glm::max(maxId.mProfit, profit);
            maxId.mDistBuy = glm::max(maxId.mDistBuy, dist);
            maxId.mDistTravel = glm::max(maxId.mDistTravel, travel);

            auto transaction = CTransaction{
              priceProduct.first,
            { priceBuy.first, priceBuy.second },
            { priceSell.first, priceSell.second },
            };

            data.push_back({ profit, dist, travel, transaction });
          }
        }

      }

      for(auto& item : data) {
        auto profitVal = item.mId.mProfit / maxId.mProfit;
        auto distVal = (item.mId.mDistBuy+1) / static_cast<float>(maxId.mDistBuy+1);
        auto travelVal = (item.mId.mDistTravel+1) / static_cast<float>(maxId.mDistTravel+1);

        float id = -profitVal * 0.4f + distVal * 0.4f + travelVal + 0.2f;

        mTransactions[id] = item.mData;
      }
    }

    void CTrader::UpdateSellTarget() {
      auto parent = GetParent();
      auto uni = parent->GetUniverse();
      auto marketEntities = uni->GetEntities<comp::CMarket, comp::CDock>();

      auto& data = mTransIt->second;

      auto value = 0.0f;
      auto target = std::shared_ptr<CEntity>();

      for(auto& entity : marketEntities) {
        auto& market = entity->GetComponent<comp::CMarket>();
        if(market.GetStorage().GetEmpty() < 0.1f)
          continue;

        auto& priceList = market.GetPriceList();
        auto price = priceList.GetValue(data.mId, PriceType::BUY);
        if(price > value) {
          price = value;
          target = entity;
        }
      }

      data.mSell.mMarket = target;
      data.mSell.mValue = value;
    }
  }

  template<>
  core::CFastId const& GetComponentId<comp::CTrader>() {
    return COMP_TRADER_ID;
  }
}
