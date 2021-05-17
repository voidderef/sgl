return {
    {
        -- Id for the game. Used for sorting and identification
        id = 0,
        -- Name of the entry
        name = "my game 2",
        -- Script to call, once selected, use .bat on Windows
        start_script = "my_game_2.sh",
        -- Script used to display the entry
        lua = "screen/attract/std/title.lua",
        -- Keys defined by lua script. table here is passed to it
        res = {
            tex_title = "my_game_2.png"
        },
        args = {

        }
    },
    {
        id = 1,
        name = "my game 1",
        -- Again, use the .bat file on Windows
        start_script = "my_game_1.sh",
        -- Here we just display the game's name as text
        lua = "screen/attract/std/text.lua",
        -- Specify the font to use to display the text
        -- Required by the script
        res = {
            font_title = "OpenSans-Regular.ttf",
        },
        args = {
            -- text to display
            text = "my game 1",
        }
    },
}
