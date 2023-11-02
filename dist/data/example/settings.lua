return {
    -- Some gfx settings, self-explaining
    gfx = {
        fullscreen = false,
        res_width = 640,
        res_height = 480,
    },

    -- Settings for IO
    io = {
        update_rate_hz = 60,
        -- Path with compiled IO device libs to load
        path_modules = "./data/io",

        -- List of IO modules to load, list of filenames without extension (!)
        modules = {
            -- Default IO is keyboard using SDL2
            "libio-keyboard-sdl2",
        },

        -- Input state processing tools. There must be
        -- one for each device which processes the inputs
        -- of the device and maps it to loader specific
        -- inputs/states
        isp_scripts = {
            -- For keyboard
            "io/keyboard/sdl2/isp-attract.lua",
            "io/keyboard/sdl2/isp-operator.lua",
        },

        -- Output state processing tools. Analogous to
        -- the ISP tools but mapping outputs from the
        -- loader to hardware to create things like light
        -- shows, display the currently selected game
        -- name on a separate display, e.g. led segment
        -- display
        osp_scripts = {

        },
    },

    -- Settings for different "screens" of the loader
    screen = {
        attract = {
            font = "OpenSans-Regular.ttf",
            -- ID of game to boot automatically if no input is pressed and the defined timeout
            -- is exceeded
            default_game_timeout_id = 0,
            -- Set this to a value greater 0 to activate
            default_game_timeout_duration_sec = 0,
        },

        boot = {
            title = "boot.png",
            min_duration_ms = 2000
        },

        error = {
            font = "OpenSans-Regular.ttf"
        },

        operator = {
            font = "OpenSans-Regular.ttf"
        },
    },

    -- Sound settings like volume
    sound = {
        -- Explicitly specify the driver you want to use. Leave empty for auto.
        -- Available drivers:
        -- Linux: alsa, pulse, oss, esd
        -- Windows: dsound, winmm, asio, wsapi
        driver = "",
        -- Explicitly specify the audio device to use. Leave empty for auto.
        -- SGL prints a list of available devices and their names to use here to the console on startup.
        device = "",
        master_volume = 1.0,
        sfx_volume = 1.0
    },

    -- System settings
    system = {
        -- Change log levels for more/less output, in order (less to more)
        -- panic
        -- error
        -- warn
        -- info
        -- debug
        -- trace
        log_level = "info",
        -- Path to the log file. Leave empty to disable logging to file
        log_file_path = "sgl.log"
        -- System data path (read only)
        data_path = "./data",
        -- System save path (must be writable)
        save_path = "./save",
        -- Path for storing temporary data (must be writable)
        tmp_path = "./tmp",
    }
}
