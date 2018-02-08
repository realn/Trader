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

      void SetProductValue(cb::string const& id, float const value);
      void AddProduct(cb::string const& id, float const amount);
      void RemProduct(cb::string const& id, float const amount);

      void BuyProduct(cb::string const& id, float const amount, CWallet& buyerWallet, CStorage& buyerStorage);
      void SellProduct(cb::string const& id, float const amount, CWallet& sellerWaller, CStorage& sellerStorage);

      bool CanBuyProduct(cb::string const& id, float const amount, CWallet const& buyerWallet);
      bool CanSellProduct(cb::string const& id, float const amount, CStorage const& sellerStorage);

      void PrintInfo(cb::ostream& stream) const;
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CMarket>();
  }

  template<>
  extern cb::string const& GetComponentId<comp::CMarket>();
}