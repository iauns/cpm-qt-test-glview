
#include <cstdlib>
#include <iostream>
#include <exception>
#include <memory>

#include <spire/Interface.h>
#include <test-glview/Interface.hpp>

void init(std::shared_ptr<CPM_SPIRE_NS::Interface> spire,
          const glm::mat4& view, const glm::mat4& perspective)
{
  // Build appropriate shaders in Spire. Imported from batch_testing module.
}

void update(std::shared_ptr<CPM_SPIRE_NS::Interface> spire,
            const glm::mat4& view, const glm::mat4& perspective)
{
  // Render the scene. We will need to be sure and clear the screen first.
  GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

  CPM_SPIRE_NS::GPUState defaultGPUState;
  mSpire->applyGPUState(defaultGPUState, true); // true = force application of state.
}

int main(int argc, char** argv)
{
  return CPM_QT_GLVIEW_NS::buildGLView(init, update);
}

