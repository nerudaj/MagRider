#include <appstate/AppStateMainMenu.hpp>
#include <DGM/dgm.hpp>
#include <misc/CMakeVars.hpp>
#include <misc/Compatibility.hpp>
#include <misc/DependencyContainer.hpp>
#include <SFML/System/Err.hpp>
#include <mutex>

int main(int, char*[])
{
    // TODO: Resolve appdata folder
    // TODO: Attept to load settings from appdata

    try
    {
        auto&& settings = AppSettings {
            .video =
                VideoSettings {
                    .resolution = sf::VideoMode::getDesktopMode().size,
                },
        };
        auto&& window = dgm::Window(dgm::WindowSettings {
            .resolution = settings.video.resolution,
            .title = CMakeVars::TITLE,
            .useFullscreen = settings.video.fullscreen,
        });
        auto&& app = dgm::App(window);
        auto&& dependencies = DependencyContainer(window, "", Language::English, settings);

        window.getSfmlWindowContext().setMouseCursorVisible(false);

        app.pushState<AppStateMainMenu>(dependencies, settings);
        app.run();
    }
    catch (const std::exception& ex)
    {
        sf::err() << ex.what() << std::endl;
        return 1;
    }

    // TODO: capture window settings
    // TODO: store settings in appdata

    return 0;
}
