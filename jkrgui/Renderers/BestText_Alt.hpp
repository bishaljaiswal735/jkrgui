#include "Instance.hpp"
#include "TextD/BestText.hpp"
#include "TwoD/Shape.hpp"
#include <Renderers/Renderer_base.hpp>
#include <Vendor/Tracy/tracy/Tracy.hpp>
#include <Vendor/stbi/stb_image_write.h>

namespace Jkr::Renderer {

/*
An alternate renderer of the Best Text Renderer for platforms that don't support variable des indexing.
This implementation renders the text into an image and uses the Shape Renderer
to display into a rectangle
*/
struct BestText_Alt {
          struct ImageId {
                    ui mRectId;
                    ui mImgId;
          };
          BestText_Alt(Instance& inInstance, Shape& inSh, BestText_base& inBestText) : sh(inSh), bt(inBestText), i(inInstance) {}

          // TODO Remove this optRef, and make a parameter non-default
          void Add(ui inFontId,
                   glm::vec2 inPos,
                   int inDepth,
                   const sv inText,
                   ImageId& outId,
                   ksai::optref<ksai::ThreadPool> inThreadPool = std::nullopt,
                   bool inAlignBottom = true);
          void Update(ImageId inId,
                      ui inFontId,
                      glm::vec2 inPos,
                      int inDepth,
                      const sv inText,
                      ksai::optref<ksai::ThreadPool> inThreadPool = std::nullopt,
                      bool inAlignBottom = true);
          void Update(ImageId inId, ui inFontId, glm::vec2 inPos, int inDepth, const sv inText, bool inAlignBottom = true);
          void Draw(ImageId inId, Window& inWindow, glm::vec4 inColor, ui inW, ui inH, glm::mat4 inMatrix);

          ImageId AddEXT(ui inFontId, glm::vec3 inPos, const sv inText);
          void UpdateEXT(ImageId inId, ui inFontId, glm::vec3 inPos, const sv inText);
          void UpdatePosOnlyEXT(ImageId inId, ui inFontId, glm::vec3 inPos, const sv inText);
          void DrawEXT(ImageId inId, Window& inWindow, glm::vec4 inColor, glm::mat4 inMatrix);

      private:
          Shape& sh;
          BestText_base& bt;
          Instance& i;
};
} // namespace Jkr::Renderer
