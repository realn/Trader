#pragma once

#include "ECOComponent.h"

namespace eco {
  namespace xml {
    struct CComponent;
  }

  namespace comp {
    class CNavigation 
      : public CComponent
    {
    public:
      using WaypointsT = std::vector<std::weak_ptr<CEntity>>;
      using EntitiesT = std::vector<std::shared_ptr<CEntity>>;
      using JumpsT = std::map<std::weak_ptr<CEntity>, size_t, weak_ptr_less<CEntity>>;
      using LinksT = std::map<std::weak_ptr<CEntity>, std::weak_ptr<CEntity>, weak_ptr_less<CEntity>>;

      struct DistMemory {
        JumpsT mJumps;
        LinksT mLinks;
      };
      using MemoriesT = std::map<std::weak_ptr<CEntity>, DistMemory, weak_ptr_less<CEntity>>;

    private:
      std::weak_ptr<CEntity> mCurrentDock;
      std::weak_ptr<CEntity> mCurrentTarget;
      std::weak_ptr<CEntity> mTarget;
      WaypointsT mWaypoints;
      mutable MemoriesT mMemory;
      float mMaxWaitTime = 0.5f;
      float mWaitTime = 0.5f;

    public:
      CNavigation(std::shared_ptr<CEntity> parent);
      CNavigation(std::shared_ptr<CEntity> parent, xml::CComponent const& component);
      virtual ~CNavigation();

      void SetTarget(std::shared_ptr<CEntity> target) { mTarget = target; }
      size_t QueryDistance(std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) const;

      void SetCurrentDock(std::shared_ptr<CEntity> dock);
      void ClearCurrentDock();
      std::shared_ptr<CEntity> GetCurrentDock() const;

      void Update(float const timeDelta) override;

      void PrintInfo(cb::ostream& stream) const override;

    private:
      std::shared_ptr<CEntity> GetNewTarget(std::shared_ptr<CEntity> dock);
      WaypointsT PlotCourse(std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target);

      void FindWays(EntitiesT const& entities, std::shared_ptr<CEntity> source, JumpsT& outJumps, LinksT & outLinks) const;
      WaypointsT CreateWaypoints(LinksT const& links, std::shared_ptr<CEntity> source, std::shared_ptr<CEntity> target) const;
    };
  }

  namespace xml {
    template<> void RegisterComponent<comp::CNavigation>();
  }

  template<>
  extern core::CFastId const& GetComponentId<comp::CNavigation>();
}