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

#ifndef IAUNS_SPIRE_GLWIDGET_H
#define IAUNS_SPIRE_GLWIDGET_H

#define NOMINMAX

#include <QTimer>

#include "../Interface.hpp"
#include "GLContext.hpp"
#include "namespaces.h"

#include <spire/Interface.h>

namespace CPM_LOOK_AT_NS {
  class ArcLookAt;
}

namespace CPM_QT_GLVIEW_NS {

class GLWidget : public QGLWidget
{
  Q_OBJECT

public:
  GLWidget(GLUpdateFunction function, const QGLFormat& format);
  ~GLWidget();

  constexpr float getDefaultFOVY()   {return 32.0f * (spire::PI / 180.0f);}
  constexpr float getDefaultZNear()  {return 0.1f;}
  constexpr float getDefaultZFar()   {return 1350.0f;}

protected:
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  void closeEvent(QCloseEvent *evt);

protected slots:
  void updateRenderer();

private:

  int    mScreenWidth;    ///< Screen width in pixels.
  int    mScreenHeight;   ///< Screen height in pixels.

  glm::mat4 mPerspective; ///< Perspective matrix.

  std::shared_ptr<spire::Interface>           mSpire;
  std::shared_ptr<GLContext>                  mContext;
  GLUpdateFunction                            mCallbackFunction;
  QTimer*                                     mTimer;

  std::unique_ptr<CPM_LOOK_AT_NS::ArcLookAt>  mArcLookAt;
};

// namespace CPM_QT_GLVIEW_NS

#endif // SPIRE_GLWIDGET_H
