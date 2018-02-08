#pragma once

#include <memory>

#include <CBXml/Node.h>

#include "ECOComponent.h"

namespace eco {
  namespace xml {
    struct CComponent;

    class IComponentTypeFactory {
    public:
      virtual ~IComponentTypeFactory() {}
      virtual std::shared_ptr<CComponent> Create(cb::CXmlNode const& node) const = 0;
    };

    template<class _Type>
    class CComponentTypeFactory
      : public IComponentTypeFactory {
    protected:
      cb::string mId;

    public:
      CComponentTypeFactory(cb::string const& id) : mId(id) {}
      virtual ~CComponentTypeFactory() {}

      std::shared_ptr<CComponent> Create(cb::CXmlNode const& node) const override {
        auto component = std::make_shared<_Type>();
        component->mId = mId;
        if(cb::ReadXmlObject(node, *component)) {
          return component;
        }
        throw std::exception("Failed component xml reading.");
      }
    };

    class CComponentFactory {
    private:
      using FactoriesT = std::map<cb::string, std::unique_ptr<IComponentTypeFactory>>;

      FactoriesT mFactories;
      static std::shared_ptr<CComponentFactory> mInstance;

    public:
      static std::shared_ptr<CComponentFactory> GetInstance();

      std::shared_ptr<CComponent> Create(cb::CXmlNode const& node) const;

      template<class _Type>
      void Register(cb::string const& id) {
        mFactories[id] = std::make_unique<CComponentTypeFactory<_Type>>(id);
      }
    };
  }
}
