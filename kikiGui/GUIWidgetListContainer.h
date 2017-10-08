#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include "GUIConsts.h"

namespace gui {
  class CWidget;

  class CWidgetListItem {
  public:
    glm::vec2 Pos;
    std::unique_ptr<CWidget> Widget;
    Align Align;

    CWidgetListItem(CWidgetListItem&&) = default;
    ~CWidgetListItem();

    CWidgetListItem& operator=(CWidgetListItem&&) = default;
  };

  class CWidgetListContainer {
  public:
    using ItemListT = std::vector<CWidgetListItem>;
    using iterator = ItemListT::iterator;
    using const_iterator = ItemListT::const_iterator;

  protected:
    ItemListT mItems;
    Align mContentAlign;
    glm::vec4 mContentMargin;

  public:
    CWidgetListContainer() : mContentAlign(Align::Default), mContentMargin(0.0f) {}
    CWidgetListContainer(CWidgetListContainer&&) = default;
    virtual ~CWidgetListContainer();

    void SetContentAlign(Align const value) { mContentAlign = value; }
    void SetContentMargin(glm::vec4 const& value) { mContentMargin = value; }

    Align GetContentAlign() const { return mContentAlign; }
    glm::vec4 GetContentMargin() const { return mContentMargin; }

    iterator begin() { return mItems.begin(); }
    const_iterator begin() const { return mItems.begin(); }

    iterator end() { return mItems.end(); }
    const_iterator end() const { return mItems.end(); }

    iterator erase(iterator it) { return mItems.erase(it); }
    void clear() { mItems.clear(); }

    bool empty() const { return mItems.empty(); }
    size_t size() const { return mItems.size(); }

    virtual CWidget* FindWidgetById(cb::string const& id);
    virtual const CWidget* FindWidgetById(cb::string const& id) const;
  };
}
