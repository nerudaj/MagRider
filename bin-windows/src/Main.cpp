#include <DGM/dgm.hpp>
#include <appstate/AppStateMainMenu.hpp>
#include <misc/CMakeVars.hpp>
#include <misc/DependencyContainer.hpp>

int main(int, char*[])
{
    // TODO: Resolve appdata folder
    // TODO: Attept to load settings from appdata

    try
    {
        auto&& settings = AppSettings {
            .video =
                VideoSettings {
#ifdef _DEBUG
                    .resolution = { 1280, 720 },
#else
                    .resolution = sf::VideoMode::getDesktopMode().size,
#endif
                },
        };

        auto&& window = dgm::Window(dgm::WindowSettings {
            .resolution = settings.video.resolution,
            .title = CMakeVars::TITLE,
            .useFullscreen = settings.video.fullscreen,
        });
        auto&& app = dgm::App(window);
        auto&& dependencies = DependencyContainer(
            window, "../assets", Language::English, settings);

        window.getSfmlWindowContext().setMouseCursorVisible(false);

        app.pushState<AppStateMainMenu>(dependencies, settings);
        app.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // TODO: capture window settings
    // TODO: store settings in appdata

    return 0;
}
