#include "stdafx.h"

#include <random>
#include <limits>

#include <CBXml/Serialize.h>

#include "ECOUniverse.h"
#include "ECOEntity.h"
#include "ECOTradeRoute.h"
#include "ECOCompDock.h"
#include "ECOCompMarket.h"
#include "ECOCompWarpDrive.h"

#include "ECOXml.h"
#include "ECOXmlComponentFactory.h"
#include "ECOCompNavigation.h"

namespace eco {
  static const auto COMP_NAVIGATION_ID = L"Navigation"s;

  namespace xml {
    template<>
    void RegisterComponent<comp::CNavigation>() {
      CComponentFactory::GetInstance()->Register<CComponent>(COMP_NAVIGATION_ID);
    }
  }

  namespace comp {
    CNavigation::CNavigation(std::shared_ptr<CEntity> parent) 
      : CComponent(parent, COMP_NAVIGATION_ID)
    {}

    CNavigation::CNavigation(std::shared_ptr<CEntity> parent, xml::CComponent const & component) 
      : CComponent(parent, COMP_NAVIGATION_ID) 
    {}

    CNavigation::~CNavigation() {}

    void CNavigation::SetCurrentDock(std::shared_ptr<CEntity> dock) {
      mCurrentDock = dock;
    }

    void CNavigation::ClearCurrentDock() {
      mCurrentDock.reset();
    }

    void CNavigation::Update(float const timeDelta) {
      if(!mCurrentDock.expired()) {
        auto dock = mCurrentDock.lock();
        auto parent = GetParent();
        {
          auto it = std::find_if(mWaypoints.begin(), mWaypoints.end(), [dock](WaypointsT::value_type& item)->auto{ return item.lock() == dock; });
          if(it != mWaypoints.end()) {
            mWaypoints.erase(it);
          }
        }
        if(!mWaypoints.empty()) {
          mCurrentTarget = mWaypoints.front();
        }
        else {
          auto newTarget = GetNewTarget(dock);
          mWaypoints = PlotCourse(dock, newTarget);
          mCurrentTarget = mWaypoints.front();
        }

        auto target = mCurrentTarget.lock();
        auto junction = dock->GetComponent<CDock>().FindJunction(target);

        junction->TravelTo(parent, dock, target);
        parent->GetComponent<comp::CWarpDrive>().SetJunction(junction);
      }
    }

    void CNavigation::PrintInfo(cb::ostream & stream) const {
      stream << L" NAVIGATION: WAYPOINTS:"s << std::endl;
      for(auto& item : mWaypoints) {
        auto entity = item.lock();
        stream << L"  "s << entity->GetId() << L"("s << entity->GetTypeId() << L")"s << std::endl;
      }
    }

    std::shared_ptr<CEntity> CNavigation::GetNewTarget(std::shared_ptr<CEntity> dock) {
      auto parent = GetParent();
      auto uni = parent->GetUniverse();

      auto targets = uni->GetEntities(GetComponentId<CDock>());
      targets.erase(std::find(targets.begin(), targets.end(), dock));

      std::random_device dev;
      auto it = targets.begin();
      std::advance(it, dev() % targets.size());

      return *it;
    }

    CNavigation::WaypointsT CNavigation::PlotCourse(std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) {
      auto jumps = JumpsT();
      auto links = LinksT();

      auto entities = GetParent()->GetUniverse()->GetEntities(GetComponentId<CDock>());

      FindWays(entities, source, jumps, links);
      return CreateWaypoints(links, source, target);
    }

    void CNavigation::FindWays(EntitiesT const & entities, std::shared_ptr<CEntity> source, JumpsT & outJumps, LinksT & outLinks) const {
      constexpr auto inf = std::numeric_limits<size_t>::max();

      EntitiesT quoue;

      for(auto& entity : entities) {
        outJumps[entity] = inf;
        outLinks[entity] = LinksT::mapped_type();
        quoue.push_back(entity);
      }
      outJumps[source] = 0;

      while(!quoue.empty()) {
        auto it = std::min_element(quoue.begin(), quoue.end(), [&outJumps](auto& left, auto& right)->auto{ return outJumps[left] < outJumps[right]; });
        auto item = *it;
        quoue.erase(it);

        auto docks = item->GetComponent<CDock>().GetConnectedDocks();
        for(auto& dock : docks) {
          auto entity = dock.lock();

          if(outJumps[item] != inf) {
            auto len = outJumps[item] + 1;
            if(outJumps[entity] > len) {
              outJumps[entity] = len;
              outLinks[entity] = item;
            }
          }
        }
      }
    }

    CNavigation::WaypointsT CNavigation::CreateWaypoints(LinksT const & links, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) const {
      auto result = WaypointsT();
      auto item = target;
      while(item != source) {
        result.push_back(item);
        item = links.at(item);
      }
      //result.push_back(source);
      return { result.rbegin(), result.rend() };
    }

  }

  template<>
  cb::string const& GetComponentId<comp::CNavigation>() {
    return COMP_NAVIGATION_ID;
  }
}
