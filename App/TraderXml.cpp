#include "stdafx.h"

#include <CBIO/File.h>
#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

#include "TraderXml.h"

const auto XML_UNIVERSE = L"Universe"s;
const auto XML_UNIVERSE_MAXJUNCTIONDISTANCE = L"MaxJunctionDistance"s;
const auto XML_UNIVERSE_ENTITYLIST = L"EntityList"s;
const auto XML_UNIVERSE_ENTITYTYPE = L"EntityType"s;
const auto XML_UNIVERSE_FACTORYTEMPLATE = L"FactoryTemplate"s;

const auto XML_ENTITYTYPE_ID = L"Id"s;
const auto XML_COMPONENT_FACTORIES = L"Factories"s;
const auto XML_COMPONENT_STORAGE = L"Storage"s;

const auto XML_ENTITYLIST_TYPEID = L"TypeId"s;
const auto XML_ENTITYLIST_ENTITY = L"Entity"s;

const auto XML_ENTITY_NAME = L"Name"s;
const auto XML_ENTITY_POSITION = L"Position"s;
const auto XML_ENTITY_DOCK = L"Dock"s;

const auto XML_FACTORYTEMPLATE_ID = L"Id"s;
const auto XML_FACTORYTEMPLATE_NAME = L"Name"s;
const auto XML_FACTORYTEMPLATE_INPRODUCT = L"In"s;
const auto XML_FACTORYTEMPLATE_OUTPRODUCT = L"Out"s;

const auto XML_STORAGE_PRODUCT = L"Product"s;

const auto XML_PRODUCT_ID = L"Id"s;
const auto XML_PRODUCT_VALUE = L"Value"s;

namespace cb {
  static bool fromStr(string const& text, glm::vec2& outValue) {
    auto list = split(text, L","s);
    return list.size() >= 2 &&
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }
}

namespace trader {
  namespace data {
    class IComponentTypeFactory {
    public:
      virtual ~IComponentTypeFactory() {}
      virtual std::unique_ptr<CComponent> Create(cb::CXmlNode const& node) const = 0;
    };
    using ComponentTypeFactoriesT = std::map<cb::string, std::unique_ptr<IComponentTypeFactory>>;

    template<class _Type>
    class CComponentTypeFactory 
      : public IComponentTypeFactory
    {
    protected:
      cb::string mId;

    public:
      CComponentTypeFactory(cb::string const& id) : mId(id) {}
      virtual ~CComponentTypeFactory() {}

      std::unique_ptr<CComponent> Create(cb::CXmlNode const& node) const override {
        auto component = std::make_unique<_Type>();
        component->mId = mId;
        if(cb::ReadXmlObject(node, *component)) {
          return component;
        }
        throw std::exception("Failed component xml reading.");
      }
    };

    class CComponentFactory {
    private:
      ComponentTypeFactoriesT mFactories;

    public:
      std::unique_ptr<CComponent> Create(cb::CXmlNode const& node) const {
        return mFactories.at(node.GetName())->Create(node);
      }

      template<class _Type>
      void Register(cb::string const& id) {
        mFactories[id] = std::make_unique<CComponentTypeFactory<_Type>>(id);
      }
    };

    static CComponentFactory CreateComponentFactory() {
      auto result = CComponentFactory();
      result.Register<CMarket>(L"Market"s);
      result.Register<CIndustry>(L"Industry"s);
      result.Register<CComponent>(L"Dock"s);
      result.Register<CComponent>(L"WarpDrive"s);
      result.Register<CComponent>(L"Navigation"s);
      return result;
    }

    static auto gComponentFactory = CreateComponentFactory();
  }
}

CB_DEFINEXMLREAD(trader::data::CProduct) {
  return
    GetAttribute(XML_PRODUCT_ID, mObject.mId) &&
    GetAttribute(XML_PRODUCT_VALUE, mObject.mValue);
}

CB_DEFINEXMLREAD(trader::data::CStorage) {
  return
    GetNodeList(mObject.mProducts, XML_STORAGE_PRODUCT);
}

CB_DEFINEXMLREAD(trader::data::CFactoryTemplate) {
  return
    GetAttribute(XML_FACTORYTEMPLATE_ID, mObject.mId) &&
    GetAttribute(XML_FACTORYTEMPLATE_NAME, mObject.mName) &&
    GetNodeList(mObject.mInProducts, XML_FACTORYTEMPLATE_INPRODUCT) &&
    GetNodeList(mObject.mOutProducts, XML_FACTORYTEMPLATE_OUTPRODUCT);
}

CB_DEFINEXMLREAD(trader::data::CEntity) {
  GetAttribute(XML_ENTITY_POSITION, mObject.mPosition);
  GetAttribute(XML_ENTITY_DOCK, mObject.mDock);
  return GetAttribute(XML_ENTITY_NAME, mObject.mName);
}

CB_DEFINEXMLREAD(trader::data::CEntityList) {
  return
    GetAttribute(XML_ENTITYLIST_TYPEID, mObject.mTypeId) &&
    GetNodeList(mObject.mEntities, XML_ENTITYLIST_ENTITY);
}

CB_DEFINEXMLREAD(trader::data::CComponent) {
  return true;
}

CB_DEFINEXMLREAD(trader::data::CIndustry) {
  if(!ReadXmlObject<trader::data::CComponent>(mNode, mObject)) { return false; }

  auto factories = cb::string();
  if(GetAttribute(XML_COMPONENT_FACTORIES, factories)) {
    mObject.mFactories = cb::split(factories, L","s, true);
  }
  return true;
}

CB_DEFINEXMLREAD(trader::data::CMarket) {
  if(!ReadXmlObject<trader::data::CComponent>(mNode, mObject)) { return false; }

  GetNode(XML_COMPONENT_STORAGE, mObject.mStorage);
  return true;
}

CB_DEFINEXMLREAD(trader::data::CEntityType) {
  if(!GetAttribute(XML_ENTITYTYPE_ID, mObject.mId))
    return false;

  for(auto& node : mNode.Nodes) {
    mObject.mComponents.push_back(trader::data::gComponentFactory.Create(node));
  }
  return true;
}

CB_DEFINEXMLREAD(trader::data::CUniverse) {
  GetAttribute(XML_UNIVERSE_MAXJUNCTIONDISTANCE, mObject.mMaxJunctionDistance);
  return 
    GetNodeList(mObject.mFactoryTemplates, XML_UNIVERSE_FACTORYTEMPLATE) &&
    GetNodeList(mObject.mTypes, XML_UNIVERSE_ENTITYTYPE) &&
    GetNodeList(mObject.mLists, XML_UNIVERSE_ENTITYLIST);
}

bool trader::data::Load(cb::string const & filepath, CUniverse & outUniverse) {
  auto source = cb::readtextfileutf8(filepath);
  if(source.empty()) {
    return false;
  }
  auto doc = cb::CXmlDocument(source);
  if(!doc.IsValid()) {
    return false;
  }
  if(doc.RootNode.GetName() != XML_UNIVERSE) {
    return false;
  }
  return cb::ReadXmlObject(doc.RootNode, outUniverse);
}
