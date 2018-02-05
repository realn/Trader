#include "stdafx.h"
#include "ECOWallet.h"
#include "ECOStorage.h"
#include "ECOCompMarket.h"

namespace eco {
  static auto COMP_MARKET_ID = L"market"s;

  namespace comp {
    CMarket::CMarket(std::shared_ptr<CEntity> parent,
                     CStorage::ValuesT const& initStorage)
      : CComponent(parent, COMP_MARKET_ID) 
      , mStorage(initStorage)
    {}

    CMarket::~CMarket() {}

    void CMarket::SetProductValue(cb::string const & id, float const value) {
      mPriceList.SetValue(id, value);
    }

    void CMarket::AddProduct(cb::string const & id, float const amount) {
      mStorage.AddProduct(id, amount);
    }

    void CMarket::RemProduct(cb::string const & id, float const amount) {
      mStorage.RemProduct(id, amount);
    }

    void CMarket::BuyProduct(cb::string const & id, float const amount, CWallet & buyerWallet, CStorage & buyerStorage) {
      buyerWallet.Withdraw(mPriceList.GetValue(id) * amount);
      mStorage.RemProduct(id, amount);
      buyerStorage.AddProduct(id, amount);
    }

    void CMarket::SellProduct(cb::string const & id, float const amount, CWallet & sellerWaller, CStorage & sellerStorage) {
      sellerStorage.RemProduct(id, amount);
      mStorage.AddProduct(id, amount);
      sellerWaller.Deposit(mPriceList.GetValue(id) * amount);
    }

    bool CMarket::CanBuyProduct(cb::string const & id, float const amount, CWallet const& buyerWallet) {
      if(!mPriceList.Contains(id))
        return false;

      return mStorage.CanRemove(id, amount) && buyerWallet.CanWithdraw(amount * mPriceList.GetValue(id));
    }

    bool CMarket::CanSellProduct(cb::string const & id, float const amount, CStorage const& sellerStorage) {
      if(!mPriceList.Contains(id))
        return false;

      return sellerStorage.CanRemove(id, amount);
    }

    void CMarket::PrintInfo(cb::ostream & stream) const {
      stream << L" MARKET:"s << std::endl;
      mStorage.PrintInfo(stream);
      mPriceList.PrintInfo(stream);
    }
  }

  template<>
  cb::string const& GetComponentId<comp::CMarket>() {
    return COMP_MARKET_ID;
  }
}
