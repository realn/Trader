#include "stdafx.h"

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

  namespace xml {
    template<>
    void RegisterComponent<comp::CTrader>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_TRADER_ID.GetName());
    }
  }

  namespace comp {
    CTrader::CTrader(std::shared_ptr<CEntity> parent)
      : CComponent(parent, COMP_TRADER_ID) {
      mWallet.Deposit(5000.0f);
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

      mTransIt = mTransactions.begin();
      while(mTransIt != mTransactions.end() && 
            mTransIt->second.mBuy.mValue > mWallet.GetAmount() * 10.0f) {
        mTransIt++;
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

      auto amount = market.GetStorage().GetProductAmount(transData.mId);
      auto funds = mWallet.GetAmount();

      auto buyAmount = 0.0f;
      if(value > 0)
        buyAmount = std::min(funds / value, amount);

      if(buyAmount == 0.0f || !market.CanBuyProduct(transData.mId, buyAmount, mWallet)) {
        mState = State::Find;
        mWaitTime = 0.0f;
        return;
      }

      market.BuyProduct(transData.mId, buyAmount, mWallet, mStorage);
      mState = State::Sell;
      mWaitTime = 0.0f;
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
      if(value == 0.0f) {
        UpdateSellTarget();
        nav.SetTarget(transData.mSell.mMarket.lock());
        return;
      }

      auto amount = mStorage.GetProductAmount(transData.mId);

      if(!market.CanSellProduct(transData.mId, amount, mStorage)) {
        UpdateSellTarget();
        nav.SetTarget(transData.mSell.mMarket.lock());
        mWaitTime = 0.0f;
        return;
      }

      market.SellProduct(transData.mId, amount, mWallet, mStorage);
      mState = State::Find;
      mWaitTime = 0.0f;
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
      auto maxId = CTransactionId{ 0.0f, 0, 0 };

      for(auto& priceProduct : mPrices) {

        for(auto& priceBuy : priceProduct.second[PriceType::SELL]) {
          for(auto& priceSell : priceProduct.second[PriceType::BUY]) {

            auto profit = priceSell.first - priceBuy.first;
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
        auto distVal = item.mId.mDistBuy / static_cast<float>(maxId.mDistBuy);
        auto travelVal = item.mId.mDistTravel / static_cast<float>(maxId.mDistTravel);

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
