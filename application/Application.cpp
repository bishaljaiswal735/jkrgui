#include <Instance.hpp>
#include <Window.hpp>
#include <EventManager.hpp>
#include <Renderers/ResourceManager.hpp>
#include <Renderers/TwoD/2d.hpp>
#include "DrawArea.hpp"

int main()
{
	constexpr uint32_t h = 1080 / 2;
	constexpr uint32_t w = 1920 / 2;
	auto Instance = Jkr::Instance();
	auto Window = Jkr::Window(Instance, "Heell", 1080 / 2, 1920 / 2);
	auto EventManager = Jkr::EventManager();
	auto RendererResources = Jkr::Renderer::ResourceManager();
	RendererResources.Load(Instance);
	auto TwoD = Jkr::Renderer::_2d(Instance, Window, RendererResources);

	auto DrawArea = App::DrawArea(TwoD, EventManager);
	DrawArea.SetWindow(&Window, w, h);

	auto Event = [&](void*)
		{
		};

	EventManager.SetEventCallBack(Event);

	auto Draw = [&](void* data) { DrawArea.Draw(); };
	Window.SetDrawCallBack(Draw);

	auto Update = [&](void* data)
		{
			auto wh = Window.GetWindowSize();
			DrawArea.Update(&Window, wh.first, wh.second);
		};
	Window.SetUpdateCallBack(Update);

	auto Dispatch = [&](void* data)
		{
			TwoD.ft.Dispatch(Window);
			TwoD.ln.Dispatch(Window);
			TwoD.sh.Dispatch(Window);
		};
	Window.SetComputeDispatchCallBack(Dispatch);

	while (!EventManager.ShouldQuit())
	{
		EventManager.ProcessEvents();
		Window.Draw();
	}


}
