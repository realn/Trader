#pragma once

#include "ECOComponent.h"
#include "ECOProduct.h"
#include "ECOStorage.h"
#include "ECOPriceList.h"

namespace eco {
  class CWallet;

  namespace xml {
    struct CComponent;
  }

  namespace comp {
    class CMarket :
      public CComponent {
    private:
      CStorage mStorage;
      CPriceList mPriceList;

    public:
      CMarket(std::shared_ptr<CEntity> parent, 
              CStorage::ValuesT const& initStorage = CStorage::ValuesT());
      CMarket(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      CMarket(CMarket&&) = default;
      virtual ~CMarket();

      CStorage const& GetStorage() const { return mStorage; }
      CPriceList const& GetPriceList() const { return mPriceList; }

      void SetProductValue(ProductId const& id, PriceType const type, float const value);
      void ClearProductValue(ProductId const& id);

      void AddProduct(ProductId const& id, float const amount);
      void RemProduct(ProductId const& id, float const amount);

      void BuyProduct(ProductId const& id, float const amount, CWallet& buyerWallet, CStorage& buyerStorage);
      void SellProduct(ProductId const& id, float const amount, CWallet& sellerWaller, CStorage& sellerStorage);

      bool CanBuyProduct(ProductId const& id, float const amount, CWallet const& buyerWallet, CStorage const& buyerStorage);
      bool CanSellProduct(ProductId const& id, float const amount, CStorage const& sellerStorage);

      void PrintInfo(cb::ostream& stream) const;
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CMarket>();
  }

  template<>
  extern core::CFastId const& GetComponentId<comp::CMarket>();
}