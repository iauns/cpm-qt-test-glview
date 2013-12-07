
#include <cstdlib>
#include <iostream>
#include <exception>
#include <memory>

#include <spire/Interface.h>
#include <test-glview/Interface.hpp>

namespace Spire = CPM_SPIRE_NS;

void init(std::shared_ptr<CPM_SPIRE_NS::Interface> spire,
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
}

void update(std::shared_ptr<CPM_SPIRE_NS::Interface> spire,
            const glm::mat4& /* view */, const glm::mat4& /* perspective */)
{
  // Render the scene. We will need to be sure and clear the screen first.
  GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

  CPM_SPIRE_NS::GPUState defaultGPUState;
  spire->applyGPUState(defaultGPUState, true); // true = force application of state.
}

int main(int argc, char** argv)
{
  return CPM_QT_GLVIEW_NS::buildGLView(argc, argv, init, update);
}

