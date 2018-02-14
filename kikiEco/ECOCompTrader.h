#pragma once

#include "ECOWallet.h"
#include "ECOStorage.h"
#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CNavigation;

    class CTrader
      : public CComponent 
    {
    private:
      struct CPriceData {
        PriceType mType = PriceType::BUY;
        std::weak_ptr<CEntity> mMarket;
      };
      struct CTransactionProduct {
        float mValue;
        std::weak_ptr<CEntity> mMarket;
      };
      struct CTransactionId {
        float mProfit;
        size_t mDistBuy;
        size_t mDistTravel;
      };
      struct CTransaction {
        ProductId mId;
        CTransactionProduct mBuy;
        CTransactionProduct mSell;
      };
      struct CTransactionIdData {
        CTransactionId mId;
        CTransaction mData;
      };
      enum class State {
        Find, Buy, Sell
      };


      using PriceDataT = std::multimap<float, std::weak_ptr<CEntity>>;
      using PriceDataTypeT = std::map<PriceType, PriceDataT>;
      using PricesT = std::map<ProductId, PriceDataTypeT>;

      using TransactionIdT = float;
      using TransactionsT = std::map<TransactionIdT, CTransaction>;
      using TransactionIdDatasT = std::vector<CTransactionIdData>;

      State mState = State::Find;
      CStorage mStorage;
      CWallet mWallet;
      PricesT mPrices;
      TransactionsT mTransactions;
      float mWaitTime = 0.5f;
      float mMaxWainTime = 0.5f;
      TransactionsT::iterator mTransIt;

    public:
      CTrader(std::shared_ptr<CEntity> parent);
      CTrader(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CTrader();

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;

    private:
      void FindTransaction();
      void BuyProducts();
      void SellProducts();

      void UpdatePrices(CUniverse const& uni);
      void UpdateTransactions(CNavigation& nav);
      void UpdateSellTarget();
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CTrader>();
  }

  template<>
  extern core::CFastId const& GetComponentId<comp::CTrader>();

}