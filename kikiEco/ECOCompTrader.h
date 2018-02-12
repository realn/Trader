#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace comp {
    class CTrader
      : public CComponent 
    {
    private:
      struct CPriceData {
        float mMin = std::numeric_limits<float>::max();
        float mMax = std::numeric_limits<float>::min();
        std::weak_ptr<CEntity> mMinMarket;
        std::weak_ptr<CEntity> mMaxMarket;
      };
      struct CTransactionProduct {
        ProductId mId;
        float mValue;
        std::weak_ptr<CEntity> mMarket;
      };
      struct CTransaction {
        CTransactionProduct mBuy;
        CTransactionProduct mSell;
      };
      using TransactionIdT = std::pair<float, size_t>;
      using PricesT = std::map<ProductId, CPriceData>;
      using TransactionsT = std::map<TransactionIdT, CTransaction>;

      PricesT mPrices;

    public:
      CTrader(std::shared_ptr<CEntity> parent);
      CTrader(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CTrader();

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;

    private:
      void UpdatePrices(CUniverse const& uni);
      void UpdateTransactions();
    };
  }
}