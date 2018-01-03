#include "stdafx.h"
#include "ECOWallet.h"

namespace eco {
  void CWallet::Withdraw(float const value) {
    if(value < 0.0f) {
      throw std::exception("Cannot withdraw negative amount.");
    }
    if(mAmount < value) {
      throw std::exception("Cannot withdraw more than stored.");
    }
    mAmount -= value;
  }

  void CWallet::Deposit(float const value) {
    if(value < 0.0f) {
      throw std::exception("Deposit cannot be negative.");
    }
    mAmount += value;
  }

  bool CWallet::CanWithdraw(float const value) const {
    if(value < 0.0f) {
      throw std::exception("Cannot withdraw negative amount.");
    }
    return value <= mAmount;
  }

  

}
