#include "Input.hpp"
#include "Utilities.hpp"

namespace DarkEmperor
{
    void Main(int width, int height, int size, bool flat)
    {
        auto graphics = Graphics::Initialize("Hex Test");

        // load assets
        Asset::LoadTextures(graphics.Renderer, "assets.json");

        auto map = DarkEmperor::Map(width, height, size, flat);

        map.View = Point(0, 0);

        map.Limit = map.Dimensions - map.View * 2;

        if (map.Flat)
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 3 + 1) * map.Size / 2) / 2 + map.Size;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 2 + 1) * map.Size * DarkEmperor::Scale / 2) / 2;
        }
        else
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 2 + 1) * map.Size * DarkEmperor::Scale / 2) / 2;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 3 + 1) * map.Size / 2) / 2 + map.Size;
        }

        for (auto y = 0; y < map.Dimensions.Y; y++)
        {
            for (auto x = 0; x < map.Dimensions.X; x++)
            {
                auto point = Point(x, y);

                auto city = false;

                auto battlefield = false;

                // show city "terrain"
                if (map.Flat)
                {
                    city = (point.Y % 3 == 0 && point.X % 2 == 0) || (point.Y % 3 == 1 && point.X % 2 == 1);

                    battlefield = (point.Y % 3 == 2 && point.X % 2 == 0) || (point.Y % 3 == 0 && point.X % 2 == 1);
                }
                else
                {
                    city = (point.X % 3 == 0 && point.Y % 2 == 0) || (point.X % 3 == 1 && point.Y % 2 == 1);

                    battlefield = (point.X % 3 == 2 && point.Y % 2 == 0) || (point.X % 3 == 0 && point.Y % 2 == 1);
                }

                auto &tile = map[Point(x, y)];

                if (city)
                {
                    tile.Asset = Asset::Id("DESERT");

                    tile.Terrain = TerrainType::CAPITAL_CITY;
                }
                else if (battlefield)
                {
                    tile.Background = Color::White;

                    tile.Terrain = TerrainType::BATTLEFIELD;
                }
                else
                {
                    tile.Background = Color::Grey;

                    tile.Terrain = TerrainType::PORT;
                }

                tile.Border = Color::Black;
            }
        }

        auto units = DarkEmperor::Units();

        auto input = Controls::User();

        auto done = false;

        while (!done)
        {
            auto scene = MapScene(map, units, Color::Brown, Color::Red);

            input = Input::WaitForInput(graphics, {scene}, input, true, 25);

            if (Input::Check(input))
            {
                if (input.Type == Controls::Type::LOCATION)
                {
                    done = true;
                }

                input.Selected = false;
            }
        }

        Asset::ClearTextures();

        Graphics::Quit(graphics);
    }
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        std::cerr << "To Use:" << std::endl
                  << std::endl
                  << argv[0] << " [width] [height] [size in pixels] [flat/pointy orientation]" << std::endl;

        exit(1);
    }

    auto width = std::atoi(argv[1]);

    auto height = std::atoi(argv[2]);

    auto size = std::atoi(argv[3]);

    auto flat = DarkEmperor::Utilities::ToUpper(argv[4]) == "FLAT";

    DarkEmperor::Main(width, height, size, flat);

    return 0;
}
