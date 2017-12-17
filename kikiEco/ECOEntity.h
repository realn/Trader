#pragma once

#include <glm/vec2.hpp>

namespace eco {
  class CEntity {
  private:
    cb::string mTypeId;
    cb::string mId;
    glm::vec2 mPosition;

  public:
    CEntity(cb::string const& typeId, cb::string const& id = cb::string());

    void SetId(cb::string const& id) { mId = id; }
    void SetPosition(glm::vec2 const& value) { mPosition = value; }

    cb::string GetTypeId() const { return mTypeId; }
    cb::string GetId() const { return mId; }
    glm::vec2 GetPosition() const { return mPosition; }
  };
}