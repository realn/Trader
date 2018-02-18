#pragma once

namespace eco {
  class CWallet {
  private:
    float mAmount;

  public:
    CWallet(float const startAmount = 0.0f) : mAmount(startAmount) {}

    void Withdraw(float const value);
    void Deposit(float const value);

    bool CanWithdraw(float const value) const;

    float GetAmount() const { return mAmount; }
  };
}