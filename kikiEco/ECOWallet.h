#pragma once

namespace eco {
  class CWallet {
  private:
    float mAmount = 0.0f;

  public:
    void Withdraw(float const value);
    void Deposit(float const value);

    bool CanWithdraw(float const value) const;

    float GetAmount() const { return mAmount; }
  };
}