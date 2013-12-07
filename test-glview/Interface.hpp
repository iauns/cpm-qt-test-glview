/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2013 Scientific Computing and Imaging Institute,
   University of Utah.


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
/// \date   December 2013

#ifndef IAUNS_QT_GLVIEW_INTERFACE_H
#define IAUNS_QT_GLVIEW_INTERFACE_H

#include <memory>
#include <functional>
#include <spire/Interface.h>

namespace CPM_QT_GLVIEW_NS {

/// Function callback that will be issued on initialization and on update.
/// Both 'view' and 'perspective' will be identity matrices when called at
/// initialization. Both will be valid when update is issued.
typedef std::function<void (std::shared_ptr<CPM_SPIRE_NS::Interface> spire, 
                            const glm::mat4& view, const glm::mat4& perspective)> GLCallback;

/// Blocking function. Constructs and runs an OpenGL window alongside a spire
/// instance. This function will call \p update @ 20 hertz. Issue any spire
/// calls or OpenGL calls in this callback. Additionally, \p init will be 
/// issued once when spire is initialized for the first time.
/// A valid OpenGL context will be current when both 'init' and 'update' are
/// called.
/// Returns error code Qt returns.
int buildGLView(int argc, char** argv, GLCallback init, GLCallback update);

} // namespace CPM_QT_GLVIEW_NS

#endif 
