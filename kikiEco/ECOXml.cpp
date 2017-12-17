#include "stdafx.h"
//
//#include <CBStr\StringEx.h>
//#include <CBStr\Convert.h>
//#include <CBXml\Serialize.h>
//
//#include "ECOEntity.h"
//#include "ECOTradeRoute.h"
//#include "ECOUniverse.h"
//
//#include "ECOXml.h"
//
//using namespace std::literals;
//
//static const auto XML_ELEM_ENTITY = L"Entity"s;
//static const auto XML_ATTR_ENTITY_ID = L"Id"s;
//static const auto XML_ATTR_ENTITY_POSITION = L"Position"s;
//
//namespace cb {
//  template<> string toStr<glm::vec2>(glm::vec2 const& value) {
//    return format(L"{0},{1}", value.x, value.y);
//  }
//
//  template<> bool fromStr<glm::vec2>(string const& value, glm::vec2& outValue) {
//    auto list = split(value, L",");
//    if(list.size() != 2) {
//      return false;
//    }
//    return
//      fromStr(list[0], outValue.x) &&
//      fromStr(list[1], outValue.y);
//  }
//}
//
//CB_DEFINEXMLWRITE(eco::CEntity) {
//  SetAttribute(XML_ATTR_ENTITY_ID, mObject.GetId());
//  SetAttribute(XML_ATTR_ENTITY_POSITION, mObject.GetPosition());
//  return true;
//}
//
//CB_DEFINEXMLREAD(eco::CEntity) {
//  auto id = L""s;
//  auto pos = glm::vec2();
//  if(GetAttribute(XML_ATTR_ENTITY_ID, id) &&
//     GetAttribute(XML_ATTR_ENTITY_POSITION, pos)) {
//    mObject.SetId(id);
//    mObject.SetPosition(pos);
//    return true;
//  }
//  return false;
//}
//
//CB_DEFINEXMLWRITE(eco::CUniverse) {
//  SetNodeList(mObject.GetEntities(), XML_ELEM_ENTITY);
//  return true;
//}
//
//CB_DEFINEXMLREAD(eco::CUniverse) {
//  return GetNodeList(mObject.GetEntities(), XML_ELEM_ENTITY);
//}