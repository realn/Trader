#include "stdafx.h"
#include "GUIXml.h"

#include "GUIConsts.h"
#include "GUIWidget.h"
#include "GUILabel.h"
#include "GUIRect.h"
#include "GUILayer.h"
#include "GUIPanel.h"
#include "GUIText.h"
#include "GUIAbsolute.h"
#include "GUIImage.h"
#include "GUIStackPanel.h"

static const auto XML_WIDGET_ID = L"Id"s;
static const auto XML_WIDGET_SIZE = L"Size"s;
static const auto XML_WIDGET_BACKCOLOR = L"BackColor"s;
static const auto XML_WIDGET_CONTENTALIGN = L"ContentAlign"s;
static const auto XML_WIDGET_CONTENTMARGIN = L"ContentMargin"s;
static const auto XML_WIDGET_ITEMALIGN = L"Align"s;
static const auto XML_WIDGET_ITEMMARGIN = L"Margin"s;
static const auto XML_WIDGET_ORIENTATION = L"Orientation"s;
static const auto XML_WIDGET_TEXT = L"Text"s;
static const auto XML_WIDGET_TEXTALIGN = L"TextAlign"s;
static const auto XML_WIDGET_TEXTSCALE = L"TextScale"s;
static const auto XML_WIDGET_TEXTCOLOR = L"TextColor"s;
static const auto XML_WIDGET_POSITION = L"Position"s;
static const auto XML_WIDGET_IMGNAME = L"ImgName"s;
static const auto XML_WIDGET_FLIPHORIZONTAL = L"FlipHorizontal"s;
static const auto XML_WIDGET_FLIPVERTICAL = L"FlipVertical"s;

namespace {
  class IWidgetObjectXmlFactory {
public:
  virtual ~IWidgetObjectXmlFactory() = default;

  virtual std::unique_ptr<gui::CWidget> CreateWidget(cb::string const& id) const = 0;
  virtual bool ReadWidgetAttribs(gui::CWidget& widget, cb::CXmlNode const& node) const = 0;
};

  class CWidgetXmlFactory {
  public:
    using FactoryMapT = std::map<cb::string, std::unique_ptr<IWidgetObjectXmlFactory>>;

  private:
    FactoryMapT mFactories;

  public:
    CWidgetXmlFactory(FactoryMapT&& factories) : mFactories(std::move(factories)) {}
    CWidgetXmlFactory(CWidgetXmlFactory&&) = default;
  
    std::unique_ptr<gui::CWidget> CreateWidget(cb::CXmlNode const& node) const {
      auto widgetName = node.GetName();
      auto it = mFactories.find(widgetName);
      if(it == mFactories.end()) {
        return nullptr;
      }
      auto widgetId = GetWidgetId(node);
      auto widget = it->second->CreateWidget(widgetId);
      if(!ReadWidgetAttribs(*widget, node)) {
        return nullptr;
      }
      return widget;
    }
  private:
    static cb::string GetWidgetId(cb::CXmlNode const& node) {
      return node.Attributes.GetValue(XML_WIDGET_ID);
    }
    bool ReadWidgetAttribs(gui::CWidget& widget, cb::CXmlNode const& node) const {
      for(auto& item : mFactories) {
        if(!item.second->ReadWidgetAttribs(widget, node)) {
          return false;
        }
      }
      return true;
    }
  };

  template<typename _Type>
  class CWidgetXmlObjectFactory
    : public IWidgetObjectXmlFactory {
  public:
    virtual ~CWidgetXmlObjectFactory() = default;

    // Inherited via IWidgetObjectXmlFactory
    virtual std::unique_ptr<gui::CWidget> CreateWidget(cb::string const & id) const override {
      return std::make_unique<_Type>(id);
    }
    virtual bool ReadWidgetAttribs(gui::CWidget & widget, cb::CXmlNode const & node) const override {
      auto thisWidget = dynamic_cast<_Type*>(&widget);
      if(!thisWidget) {
        return true;
      }
      if(!cb::ReadXmlObject(node, *thisWidget)) {
        return false;
      }
      return true;;
    }
  };

  static auto getWidgetFactoriesMap() {
    auto result = CWidgetXmlFactory::FactoryMapT();
    result[L"Rect"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CRect>>();
    result[L"Image"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CImage>>();
    result[L"Label"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CLabel>>();
    result[L"Panel"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CPanel>>();
    result[L"StackPanel"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CStackPanel>>();
    result[L"Absolute"s] = std::make_unique<CWidgetXmlObjectFactory<gui::CAbsolute>>();
    result[L"Text"] = std::make_unique<CWidgetXmlObjectFactory<gui::CText>>();
    return result;
  }
  static auto widgetFactory = CWidgetXmlFactory(getWidgetFactoriesMap());
}

namespace cb {
  static auto alignStrMap = std::map<gui::Align, cb::string>{
    {gui::Align::Left, L"Left"s},
    {gui::Align::Right, L"Right"s},
    {gui::Align::Top, L"Top"s},
    {gui::Align::Bottom, L"Bottom"s},
  };
  static auto alignStrMapRev = mapflip(alignStrMap);
  static auto alignMultiStrMap = std::map<gui::Align, cb::string>{
    {gui::Align::Middle, L"Middle"s},
    {gui::Align::Center, L"Center"s},
  };
  static auto alignMultiStrMapRev = mapflip(alignMultiStrMap);

  static string toStr(glm::vec2 const& value) {
    return format(L"{0},{1}"s, value.x, value.y);
  }
  static string toStr(glm::vec4 const& value) {
    return format(L"{0},{1},{2},{3}"s, value.x, value.y, value.z, value.w);
  }
  static string toStr(gui::Aligns const align) {
    auto result = cb::strvector();
    if(align & gui::Align::MidCenter) {
      result.push_back(L"MidCenter"s);
    }
    else {
      auto temp = align;
      for(auto& item : alignMultiStrMap) {
        if(temp & item.first) { result.push_back(item.second); temp &= ~gui::Aligns(item.first); }
      }
      for(auto& item : alignStrMap) { 
        if(temp & item.first) { result.push_back(item.second); }
      }
    }
    return join(result, L","s);
  }

  static bool fromStr(string const& text, glm::vec2& outValue) {
    auto list = split(text, L","s);
    return list.size() >= 2 &&
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }

  static bool fromStr(string const& text, glm::vec4& outValue) {
    auto list = split(text, L","s);
    return list.size() >= 4 &&
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y) &&
      fromStr(list[2], outValue.z) &&
      fromStr(list[3], outValue.w);
  }

  static bool fromStr(string const& text, gui::Align& outValue) {
    auto result = gui::Aligns(gui::Align::None);
    auto list = split(text, L","s);
    if(std::find(list.begin(), list.end(), L"MidCenter"s) != list.end()) {
      result |= gui::Align::MidCenter;
    }
    else {
      for(auto& item : list) {
        auto it = alignMultiStrMapRev.find(item);
        if(it != alignMultiStrMapRev.end()) {
          result |= it->second;
        }
      }
      for(auto& item : list) {
        auto it = alignStrMapRev.find(item);
        if(it != alignStrMapRev.end()) { result |= it->second; }
      }
    }
    outValue = result;
    return true;
  }
}

CB_DEFINEXMLREAD(gui::CWidget) {
  auto size = glm::vec2();
  if(GetAttribute(XML_WIDGET_SIZE, size)) { mObject.SetFixedSize(size); }
  return true;
}

CB_DEFINEXMLREAD(gui::CRect) {
  if(!cb::ReadXmlObject<gui::CWidget>(mNode, mObject)) { return false; }
  
  auto backColor = glm::vec4();

  if(GetAttribute(XML_WIDGET_BACKCOLOR, backColor)) { mObject.SetBackColor(backColor); }
  return true;
}

CB_DEFINEXMLREAD(gui::CImage) {
  if(!cb::ReadXmlObject<gui::CRect>(mNode, mObject)) { return false; }

  auto imgName = cb::string();
  auto flipHor = false;
  auto flipVer = false;

  if(GetAttribute(XML_WIDGET_IMGNAME, imgName)) { mObject.SetImage(imgName); }
  if(GetAttribute(XML_WIDGET_FLIPHORIZONTAL, flipHor)) { mObject.SetFlipHorizontal(flipHor); }
  if(GetAttribute(XML_WIDGET_FLIPVERTICAL, flipVer)) { mObject.SetFlipVertical(flipVer); }

  return true;
}

CB_DEFINEXMLREAD(gui::CLabel) {
  if(!cb::ReadXmlObject<gui::CWidget>(mNode, mObject)) { return false; }

  auto text = cb::string();
  auto align = gui::Align::Default;
  auto scale = 1.0f;
  auto scale2 = glm::vec2(1.0f);
  auto color = glm::vec4(1.0f);

  if(GetAttribute(XML_WIDGET_TEXTALIGN, align)) { mObject.SetTextAlign(align); }
  if(GetAttribute(XML_WIDGET_TEXTSCALE, scale)) { mObject.SetTextScale({scale, scale}); }
  if(GetAttribute(XML_WIDGET_TEXTSCALE, scale2)) { mObject.SetTextScale(scale2); }
  if(GetAttribute(XML_WIDGET_TEXTCOLOR, color)) { mObject.SetTextColor(color); }
  if(GetAttribute(XML_WIDGET_TEXT, text)) { mObject.SetText(text); }
  else { mObject.SetText(mNode.GetValue()); }
  return true;
}

CB_DEFINEXMLREAD(gui::CText) {
  if(!cb::ReadXmlObject<gui::CLabel>(mNode, mObject)) { return false; }

  return true;
}

CB_DEFINEXMLREAD(gui::CPanel) {
  if(!cb::ReadXmlObject<gui::CRect>(mNode, mObject)) { return false; }

  auto contentAlign = gui::Align::Default;
  auto contentMargin = glm::vec4(0.0f);

  if(GetAttribute(XML_WIDGET_CONTENTALIGN, contentAlign)) { mObject.SetContentAlign(contentAlign); }
  if(GetAttribute(XML_WIDGET_CONTENTMARGIN, contentMargin)) { mObject.SetContentMargin(contentMargin); }

  for(auto& node : mNode.Nodes) {
    auto widget = widgetFactory.CreateWidget(node);
    if(widget) {
      mObject.SetContent(std::move(widget));
      return true;
    }
  }
  return true;
}

CB_DEFINEXMLREAD(gui::CStackPanel) {
  if(!cb::ReadXmlObject<gui::CWidget>(mNode, mObject)) { return false; }

  auto align = gui::Align::Default;
  auto margin = glm::vec4(0.0f);

  if(GetAttribute(XML_WIDGET_CONTENTALIGN, align)) { mObject.SetContentAlign(align); }
  if(GetAttribute(XML_WIDGET_CONTENTMARGIN, margin)) { mObject.SetContentMargin(margin); }

  for(auto& node : mNode.Nodes) {
    auto widget = widgetFactory.CreateWidget(node);
    if(widget) {
      auto itemAlign = gui::Align::Default;
      cb::fromStr(node.Attributes.GetValue(XML_WIDGET_ITEMALIGN), itemAlign);
      mObject.AddWidget(std::move(widget), itemAlign);
    }
  }
  return true;
}

CB_DEFINEXMLREAD(gui::CAbsolute) {
  if(!cb::ReadXmlObject<gui::CWidget>(mNode, mObject)) { return false; }

  auto align = gui::Align::Default;
  auto margin = glm::vec4(0.0f);
  auto pos = glm::vec2(0.0f);

  if(GetAttribute(XML_WIDGET_CONTENTALIGN, align)) { mObject.SetContentAlign(align); }
  if(GetAttribute(XML_WIDGET_CONTENTMARGIN, margin)) { mObject.SetContentMargin(margin); }
  if(GetAttribute(XML_WIDGET_POSITION, pos)) { mObject.SetPosition(pos); }

  for(auto& node : mNode.Nodes) {
    auto widget = widgetFactory.CreateWidget(node);
    if(widget) {
      mObject.SetContent(std::move(widget));
      return true;
    }
  }
  return true;
}

CB_DEFINEXMLREAD(gui::CLayer) {
  auto textScale = glm::vec2(1.0f);
  auto contentAlign = gui::Align::Default;
  auto contentMargin = glm::vec4(0.0f);

  if(GetAttribute(XML_WIDGET_CONTENTALIGN, contentAlign)) { mObject.SetContentAlign(contentAlign); }
  if(GetAttribute(XML_WIDGET_CONTENTMARGIN, contentMargin)) { mObject.SetContentMargin(contentMargin); }
  if(GetAttribute(XML_WIDGET_TEXTSCALE, textScale)) { mObject.SetTextScale(textScale); }
  if(GetAttribute(XML_WIDGET_TEXTSCALE, textScale.x)) { mObject.SetTextScale(glm::vec2(textScale.x)); }

  for(auto& node : mNode.Nodes) {
    auto widget = widgetFactory.CreateWidget(node);
    if(widget) {
      mObject.SetContent(std::move(widget));
      return true;
    }
  }
  return true;
}
