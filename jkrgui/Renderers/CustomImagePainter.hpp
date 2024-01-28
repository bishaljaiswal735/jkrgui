#include "../Painter.hpp"
#include "../PainterParameter.hpp"

namespace Jkr::Renderer {
using Image = PainterParameter<Jkr::PainterParameterContext::StorageImage>;

struct CustomPainterImage {
    up<Image> mPainterParam;
    CustomPainterImage(const Instance& inInstance, const Window& inWindow, ui inWidth, ui inHeight)
    {
        mPainterParam = mu<Image>(inInstance);
        mPainterParam->Setup(inWidth, inHeight);
        Painter::OptimizeParameter(inInstance, *mPainterParam, inWindow);
    }
    GETTER& GetPainterParam() { return *mPainterParam; }
};


class CustomImagePainter {
     using ComPar = Jkr::Window::ParameterContext;
public:
    CustomImagePainter(
        sv inName,
        sv inComputeShaderFunction,
        sv inPushConstantSignature,
        ui inX,
        ui inY,
        ui inZ);
    GETTER& GetPainter() { return *mPainter; }

    void Load(const Instance& inInstance, Window& inWindow);
    void Store(const Instance& inInstance, Window& inWindow);

    void RegisterImage(const Instance& inInstance, Window& inWindow, CustomPainterImage& inImage)
    {
        mPainter->RegisterPainterParameter(inImage.GetPainterParam(), 0, 0, 0, Jkr::RegisterMode::ComputeOnly);
    }

    void Bind(const Window& inWindow, ComPar inPar = ComPar::None)
    {
        mPainter->BindDispatchParametersPipelineOnly_EXT(inWindow, inPar);
    }

    void BindImage(const Window& inWindow, ComPar inPar = ComPar::None)
    {
        mPainter->BindDispatchParametersDescriptorsOnly_EXT(inWindow, inPar);
    }

    template <class T>
    void Draw(Window& inWindow, T inPushConstant, ui inX, ui inY, ui inZ, ComPar inPar = ComPar::None)
    {
        mPainter->Dispatch_EXT<T>(inPushConstant, inX, inY, inZ, inPar);
    }

    template <class T>
    void Draw(Window& inWindow, T inPushConstant, ComPar inPar = ComPar::None)
    {
        mPainter->Dispatch_EXT<T>(inPushConstant, mThreads.x, mThreads.y, mThreads.z, inPar);
    }

private:
    s mCustomPainterFileName = "customPainter.bin";
    std::ostringstream mComputeStream;
    std::ostringstream mVertexStream;
    std::ostringstream mFragmentStream;
    glm::vec3 mThreads;
    Up<PainterCache> mCustomPainterCache;
    Up<Painter> mPainter;
};

} // namespace Jkr::Renderer
