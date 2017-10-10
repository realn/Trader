#pragma once

namespace core {
  template<typename _ObserverType>
  class IEventSource {
  private:
    _ObserverType* mObserver = nullptr;

  public:
    IEventSource() = default;
    virtual ~IEventSource() = default;

    void SetObserver(_ObserverType* observer) { mObserver = observer; }
    _ObserverType* GetObserver() const { return mObserver; }

  protected:
    _ObserverType* Get() { return mObserver; }
    const _ObserverType* Get() const { return mObserver; }
  };

  template<typename _ObserverType>
  class IEventTarget : public _ObserverType {
  private:
    IEventSource<_ObserverType>* mSource = nullptr;

  public:
    IEventTarget() = default;
    virtual ~IEventTarget() { if(mSource) mSource->SetObserver(nullptr); }

    void SetSource(IEventSource<_ObserverType>& source) {
      mSource = &source;
    }
  };

  template<typename _Type>
  void bind(IEventSource<_Type>& source, IEventTarget<_Type>& target) {
    target.SetSource(source);
    source.SetObserver(&target);
  }
}