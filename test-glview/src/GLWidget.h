/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/// \author James Hughes
/// \date   September 2012

#ifndef IAUNS_QT_GLVIEW_GLWIDGET_H
#define IAUNS_QT_GLVIEW_GLWIDGET_H

#define NOMINMAX

#include <QTimer>

#include "../Interface.hpp"
#include "GLContext.hpp"

#include <spire/Interface.h>
#include <glm-aabb/AABB.hpp>
#include <glm/gtc/constants.hpp>

namespace CPM_LOOK_AT_NS {
  class ArcLookAt;
}

namespace CPM_QT_GLVIEW_NS {

class GLWidget : public QGLWidget
{
  Q_OBJECT

public:
  GLWidget(GLCallback init, GLCallback update,
           CPM_GLM_AABB_NS::AABB sceneExtents,
           const QGLFormat& format);
  ~GLWidget();

  static float getDefaultFOVY()   {return 32.0f * (glm::pi<float>() / 180.0f);}
  static float getDefaultZNear()  {return 0.1f;}
  static float getDefaultZFar()   {return 1350.0f;}

protected:
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);
  void closeEvent(QCloseEvent *evt);

protected slots:
  void updateRenderer();

private:

  glm::vec2 calculateScreenSpaceCoords(const glm::ivec2& mousePos);

  int    mScreenWidth;    ///< Screen width in pixels.
  int    mScreenHeight;   ///< Screen height in pixels.

  glm::mat4 mPerspective; ///< Perspective matrix.

  std::shared_ptr<CPM_SPIRE_NS::Interface>    mSpire;
  std::shared_ptr<GLContext>                  mContext;
  GLCallback                                  mCallbackFunction;
  QTimer*                                     mTimer;
  CPM_GLM_AABB_NS::AABB                       mSceneExtents;

  std::shared_ptr<CPM_LOOK_AT_NS::ArcLookAt>  mArcLookAt;
};

} // namespace CPM_QT_GLVIEW_NS

#endif // SPIRE_GLWIDGET_H
