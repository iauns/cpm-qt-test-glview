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

#ifndef TOOLS_VIEW_MAINWINDOW_H
#define TOOLS_VIEW_MAINWINDOW_H

// For windows.
#define NOMINMAX

#include <QMainWindow>
#include <QDialog>

#include "GLWidget.h"

class MyGLFrame;

namespace Ui {
class MainWindow;
}

namespace CPM_QT_GLVIEW_NS {

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(GLCallback init, GLCallback update, QWidget *parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *evt);

private:
  Ui::MainWindow*     ui;
  GLWidget*           mGLWidget;
};

} // namespace CPM_QT_GLVIEW_NS

#endif // TOOLS_VIEW_MAINWINDOW_H
