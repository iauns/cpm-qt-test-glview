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

#include <iostream>
#include <QMouseEvent>

#include "GLWidget.hpp"

#include "namespaces.h"

using namespace spire;
using spire::V4;
using spire::V3;
using spire::V2;
using spire::M44;

//------------------------------------------------------------------------------
GLWidget::GLWidget(GLUpdateFunction function, const QGLFormat& format) :
    QGLWidget(format),
    mContext(new GLContext(this)),
    mCallbackFunction(function)
{
  std::vector<std::string> shaderSearchDirs;
  shaderSearchDirs.push_back("shaders");

  // Create an instance of spire and hook it up to a timer.
  mSpire = std::shared_ptr<spire::Interface>(
      new spire::Interface(mContext, shaderSearchDirs));
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(updateRenderer()));
  mTimer->start(35);

  // We must disable auto buffer swap on the 'paintEvent'.
  setAutoBufferSwap(false);
}

//------------------------------------------------------------------------------
void GLWidget::resizeEvent(QResizeEvent*)
{
  /// @todo Inform the renderer that screen dimensions have changed.
  //mSpire.resizeViewport(evt->size());
}

//------------------------------------------------------------------------------
void GLWidget::closeEvent(QCloseEvent *evt)
{
  // Kill off the graphics thread.
  mSpire.reset();
  QGLWidget::closeEvent(evt);
}

//------------------------------------------------------------------------------
void GLWidget::updateRenderer()
{
  mContext->makeCurrent();    // Required on windows...

  // Do not even attempt to render if the framebuffer is not complete.
  // This can happen when the rendering window is hidden (in SCIRun5 for
  // example);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return;

  if (mCallbackFunction)
    mCallbackFunction(mSpire);
}

//------------------------------------------------------------------------------
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  glm::ivec2 thisPos;
  thisPos.x = event->x();
  thisPos.y = event->y();

  glm::ivec2 delta = thisPos - mLastMousePos;

  // Apply this rotation extremelly naively to the camera.
  // divisor is a magic calibration number from pixels to rotation speed.
  const float divisor = 32.0f;
  float rx = static_cast<float>(-delta.y) / divisor;
  float ry = static_cast<float>(delta.x) / divisor;

  M44 tx = glm::rotate(M44(), rx, V3(1.0, 0.0, 0.0));
  M44 ty = glm::rotate(M44(), ry, V3(0.0, 1.0, 0.0));

  // x applied first in object space, then y.
  mCamWorld = mCamWorld * ty * tx;

  mLastMousePos = thisPos;
}

//------------------------------------------------------------------------------
void GLWidget::mousePressEvent(QMouseEvent* event)
{
  mLastMousePos.x = event->x();
  mLastMousePos.y = event->y();
}

//------------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent(QMouseEvent*)
{
}



