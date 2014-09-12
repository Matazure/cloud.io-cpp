{
    'targets':[
        {
            'target_name' : 'cloudio',
            'type' : 'static_library',
            'dependencies': ['./cloudio/websocket/websocket.gyp:websocket'],
            's'

            'all_dependent_settings':
            {
                'include_dirs': [ '.'],
            },
        },
    ]
}
