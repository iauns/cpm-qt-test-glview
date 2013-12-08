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
#include <arc-look-at/ArcLookAt.hpp>

#include "GLWidget.h"

namespace Spire = CPM_SPIRE_NS;

namespace CPM_QT_GLVIEW_NS {

//------------------------------------------------------------------------------
GLWidget::GLWidget(GLCallback init, GLCallback update,
                   CPM_GLM_AABB_NS::AABB sceneExtents,
                   const QGLFormat& format) :
    QGLWidget(format),
    mContext(new GLContext(this)),
    mCallbackFunction(update),
    mArcLookAt(new CPM_LOOK_AT_NS::ArcLookAt),
    mSceneExtents(sceneExtents)
{
  std::vector<std::string> shaderSearchDirs;
  shaderSearchDirs.push_back("shaders");

  // Create an instance of spire and hook it up to a timer.
  mSpire = std::shared_ptr<Spire::Interface>(
      new Spire::Interface(mContext, shaderSearchDirs));
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(updateRenderer()));
  mTimer->start(35);

  // Let the client initialize their graphics code.
  init(mSpire, glm::mat4(), mPerspective);

  // We must disable auto buffer swap on the 'paintEvent'.
  setAutoBufferSwap(false);

  // Ensure that we receive keyboard events.
  setFocusPolicy(Qt::StrongFocus);
}

//------------------------------------------------------------------------------
GLWidget::~GLWidget()
{
  if (mSpire != nullptr)
  {
    mSpire.reset();
  }
}

//------------------------------------------------------------------------------
void GLWidget::closeEvent(QCloseEvent *evt)
{
  if (mSpire != nullptr)
  {
    mSpire.reset();
  }
  QGLWidget::closeEvent(evt);
}

//------------------------------------------------------------------------------
void GLWidget::resizeGL(int width, int height)
{
  mSpire->makeCurrent();
  GL(glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));

  mScreenWidth = width;
  mScreenHeight = height;

  // Build the perspective matrix.
  float aspect = static_cast<float>(mScreenWidth) / 
                 static_cast<float>(mScreenHeight);
  mPerspective = glm::perspective(getDefaultFOVY(), aspect, getDefaultZNear(), getDefaultZFar());
}

//------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
  
}

//------------------------------------------------------------------------------
void GLWidget::updateRenderer()
{
  mContext->makeCurrent();

  // Do not even attempt to render if the framebuffer is not complete.
  // This can happen when the rendering window is hidden (in SCIRun5 for
  // example);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return;

  if (mCallbackFunction)
    mCallbackFunction(mSpire, mArcLookAt->getWorldViewTransform(), mPerspective);

  mContext->swapBuffers();
}

//------------------------------------------------------------------------------
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Space)
  {
    if (mSceneExtents.isNull())
    {
      std::cout << "Failure" << std::endl;
    }
    mArcLookAt->autoview(mSceneExtents, getDefaultFOVY());
  }
}

//------------------------------------------------------------------------------
glm::vec2 GLWidget::calculateScreenSpaceCoords(const glm::ivec2& mousePos)
{
  float windowOriginX = 0.0f;
  float windowOriginY = 0.0f;

  // Transform incoming mouse coordinates into screen space.
  glm::vec2 mouseScreenSpace;
  mouseScreenSpace.x = 2.0f * (static_cast<float>(mousePos.x) - windowOriginX) 
      / static_cast<float>(mScreenWidth) - 1.0f;
  mouseScreenSpace.y = 2.0f * (static_cast<float>(mousePos.y) - windowOriginY)
      / static_cast<float>(mScreenHeight) - 1.0f;
  mouseScreenSpace.y = -mouseScreenSpace.y;

  return mouseScreenSpace;
}

//------------------------------------------------------------------------------
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  glm::vec2 screenSpace = calculateScreenSpaceCoords(glm::ivec2(event->x(), event->y()));
  if (event->buttons() & Qt::LeftButton)
    mArcLookAt->doRotation(screenSpace);
  else if (event->buttons() & Qt::RightButton)
    mArcLookAt->doPan(screenSpace);
}

//------------------------------------------------------------------------------
void GLWidget::mousePressEvent(QMouseEvent* event)
{
  glm::vec2 screenSpace = calculateScreenSpaceCoords(glm::ivec2(event->x(), event->y()));
  mArcLookAt->doReferenceDown(screenSpace);
}

//------------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent(QMouseEvent*)
{
}

//------------------------------------------------------------------------------
void GLWidget::wheelEvent(QWheelEvent* event)
{
  mArcLookAt->doZoom(-static_cast<float>(event->delta()) / 100.0f);
}

} // namespace CPM_QT_GLVIEW_NS

