#include "stdafx.h"
#include <CBIO/File.h>
#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

#include "ECOXmlComponentFactory.h"
#include "ECOXml.h"

const auto XML_UNIVERSE = L"Universe"s;
const auto XML_UNIVERSE_MAXJUNCTIONDISTANCE = L"MaxJunctionDistance"s;
const auto XML_UNIVERSE_ENTITYLIST = L"EntityList"s;
const auto XML_UNIVERSE_ENTITYTYPE = L"EntityType"s;
const auto XML_UNIVERSE_FACTORYTEMPLATE = L"FactoryTemplate"s;

const auto XML_ENTITYTYPE_ID = L"Id"s;

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
  bool fromStr(string const& text, glm::vec2& outValue) {
    auto list = split(text, L","s);
    return list.size() >= 2 &&
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }
}

CB_DEFINEXMLREAD(eco::xml::CProduct) {
  return
    GetAttribute(XML_PRODUCT_ID, mObject.mId) &&
    GetAttribute(XML_PRODUCT_VALUE, mObject.mValue);
}

CB_DEFINEXMLREAD(eco::xml::CStorage) {
  return
    GetNodeList(mObject.mProducts, XML_STORAGE_PRODUCT);
}

CB_DEFINEXMLREAD(eco::xml::CFactoryTemplate) {
  return
    GetAttribute(XML_FACTORYTEMPLATE_ID, mObject.mId) &&
    GetAttribute(XML_FACTORYTEMPLATE_NAME, mObject.mName) &&
    GetNodeList(mObject.mInProducts, XML_FACTORYTEMPLATE_INPRODUCT) &&
    GetNodeList(mObject.mOutProducts, XML_FACTORYTEMPLATE_OUTPRODUCT);
}

CB_DEFINEXMLREAD(eco::xml::CEntity) {
  GetAttribute(XML_ENTITY_POSITION, mObject.mPosition);
  GetAttribute(XML_ENTITY_DOCK, mObject.mDock);
  return GetAttribute(XML_ENTITY_NAME, mObject.mName);
}

CB_DEFINEXMLREAD(eco::xml::CEntityList) {
  return
    GetAttribute(XML_ENTITYLIST_TYPEID, mObject.mTypeId) &&
    GetNodeList(mObject.mEntities, XML_ENTITYLIST_ENTITY);
}

CB_DEFINEXMLREAD(eco::xml::CComponent) {
  return true;
}

CB_DEFINEXMLREAD(eco::xml::CEntityType) {
  if(!GetAttribute(XML_ENTITYTYPE_ID, mObject.mId))
    return false;

  auto componentFactory = eco::xml::CComponentFactory::GetInstance();
  for(auto& node : mNode.Nodes) {
    mObject.mComponents.push_back(componentFactory->Create(node));
  }
  return true;
}

CB_DEFINEXMLREAD(eco::xml::CUniverse) {
  GetAttribute(XML_UNIVERSE_MAXJUNCTIONDISTANCE, mObject.mMaxJunctionDistance);
  return
    GetNodeList(mObject.mFactoryTemplates, XML_UNIVERSE_FACTORYTEMPLATE) &&
    GetNodeList(mObject.mTypes, XML_UNIVERSE_ENTITYTYPE) &&
    GetNodeList(mObject.mLists, XML_UNIVERSE_ENTITYLIST);
}

bool eco::xml::Load(cb::string const & filepath, CUniverse & outUniverse) {
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
