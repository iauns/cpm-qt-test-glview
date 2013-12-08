
#include <cstdlib>
#include <iostream>
#include <exception>
#include <memory>

#include <spire/Interface.h>
#include <spire/src/GLMathUtil.h>
#include <test-glview/Interface.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace Spire = CPM_SPIRE_NS;

const std::string objectName = "obj1";

void init(std::shared_ptr<Spire::Interface> spire,
          const glm::mat4& /*view*/, const glm::mat4& /* perspective */)
{
  spire->addShaderAttribute("aPos",         3,  false,  sizeof(float) * 3,  Spire::Interface::TYPE_FLOAT);
  spire->addShaderAttribute("aNormal",      3,  false,  sizeof(float) * 3,  Spire::Interface::TYPE_FLOAT);
  spire->addShaderAttribute("aColorFloat",  4,  false,  sizeof(float) * 4,  Spire::Interface::TYPE_FLOAT);
  spire->addShaderAttribute("aColor",       4,  true,   sizeof(char) * 4,   Spire::Interface::TYPE_UBYTE);

  // Build appropriate shaders in Spire. Imported from batch_testing module.
  std::string shader1 = "UniformColor";
  std::vector<std::tuple<std::string, Spire::Interface::SHADER_TYPES>> shaderFiles;
  shaderFiles.push_back(std::make_pair("UniformColor.vsh", Spire::Interface::VERTEX_SHADER));
  shaderFiles.push_back(std::make_pair("UniformColor.fsh", Spire::Interface::FRAGMENT_SHADER));
  spire->addPersistentShader(shader1, shaderFiles);

  std::vector<float> vboData = 
  {
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f
  };
  std::vector<std::string> attribNames = {"aPos"};

  std::vector<uint16_t> iboData =
  {
    0, 1, 2, 3
  };
  Spire::Interface::IBO_TYPE iboType = Spire::Interface::IBO_16BIT;

  uint8_t*  rawBegin;
  size_t    rawSize;

  // Copy vboData into vector of uint8_t. Using std::copy.
  std::shared_ptr<std::vector<uint8_t>> rawVBO(new std::vector<uint8_t>());
  rawSize = vboData.size() * (sizeof(float) / sizeof(uint8_t));
  rawVBO->reserve(rawSize);
  rawBegin = reinterpret_cast<uint8_t*>(&vboData[0]);
  rawVBO->assign(rawBegin, rawBegin + rawSize);

  // Copy iboData into vector of uint8_t. Using std::vector::assign.
  std::shared_ptr<std::vector<uint8_t>> rawIBO(new std::vector<uint8_t>());
  rawSize = iboData.size() * (sizeof(uint16_t) / sizeof(uint8_t));
  rawIBO->reserve(rawSize);
  rawBegin = reinterpret_cast<uint8_t*>(&iboData[0]);
  rawIBO->assign(rawBegin, rawBegin + rawSize);

  // Add necessary VBO's and IBO's
  std::string vbo1 = "vbo1";
  std::string ibo1 = "ibo1";
  spire->addVBO(vbo1, rawVBO, attribNames);
  spire->addIBO(ibo1, rawIBO, iboType);

  // Add object
  spire->addObject(objectName);

  // Build the pass (default pass).
  spire->addPassToObject(objectName, shader1, vbo1, ibo1, Spire::Interface::TRIANGLE_STRIP);

  glm::mat4 xform;
  xform[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  spire->addObjectPassUniform(objectName, "uColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

}

void update(std::shared_ptr<Spire::Interface> spire,
            const glm::mat4& view, const glm::mat4& perspective)
{
  glm::mat4 inverseView = glm::affineInverse(view);
  Spire::printM44(inverseView);

  // Render the scene. We will need to be sure and clear the screen first.
  GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

  Spire::GPUState defaultGPUState;
  spire->applyGPUState(defaultGPUState, true); // true = force application of state.

  // Setup simple camera
  spire->addGlobalUniform("uProjIV", perspective);
  spire->addObjectGlobalUniform(objectName, "uProjIVObject", perspective * inverseView);

  spire->renderObject(objectName);
}

int main(int argc, char** argv)
{
  return CPM_QT_GLVIEW_NS::buildGLView(argc, argv, init, update);
}

