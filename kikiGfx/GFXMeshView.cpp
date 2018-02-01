#include "stdafx.h"
#include "GFXMeshView.h"
#include "GFXMesh.h"

namespace gfx {
  CMeshView::CMeshView() 
    : mVertexBuffer(cb::gl::BufferTarget::ARRAY)
    , mIndexBuffer(cb::gl::BufferTarget::ELEMENT_ARRAY)
    , mNumberOfIndices(0)
    , mPrimitiveType(cb::gl::PrimitiveType::TRIANGLES)
  {}

  CMeshView::CMeshView(CMesh const & mesh) 
    : CMeshView()
  {
    Set(mesh);
  }

  void CMeshView::Set(CMesh const & mesh) {
    mVertexBuffer.SetData(mesh.GetVertices());
    CMesh::IndexVecT indices;
    for(auto& face : mesh.GetFaces()) {
      for(auto& index : face.Indices) {
        indices.push_back(index);
      }
    }
    mIndexBuffer.SetData(indices);
    mNumberOfIndices = static_cast<cb::u32>(indices.size());
    mPrimitiveType = mesh.GetPrimitiveType();
  }

  void CMeshView::Update(CMesh const & mesh) {
    CMesh::IndexVecT indices;
    for(auto& face : mesh.GetFaces()) {
      for(auto& index : face.Indices) {
        indices.push_back(index);
      }
    }
    if(mNumberOfIndices == indices.size() &&
       mPrimitiveType == mesh.GetPrimitiveType()) {
      mVertexBuffer.SetSubData(mesh.GetVertices());
      mIndexBuffer.SetSubData(indices);
    }
    else {
      mVertexBuffer.SetData(mesh.GetVertices());
      mIndexBuffer.SetData(indices);
      mNumberOfIndices = indices.size();
      mPrimitiveType = mesh.GetPrimitiveType();
    }
  }

  void CMeshView::Bind() const {
    mVertexBuffer.Bind();
    CMeshVertex::Def.Bind();
    mIndexBuffer.Bind();
  }

  void CMeshView::UnBind() const {
    mIndexBuffer.UnBind();
    CMeshVertex::Def.UnBind();
    mVertexBuffer.UnBind();
  }

  void CMeshView::Render() {
    cb::gl::drawElements(mPrimitiveType, mNumberOfIndices);
  }
}
