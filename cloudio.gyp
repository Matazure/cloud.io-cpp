{
    'targets':[
        {
            'target_name' : 'cloudio',
            'type' : 'none',
            'dependencies': ['./cloudio/websocket/websocket.gyp:websocket'],
            's'

            'all_dependent_settings':
            {
                'include_dirs': [ '.'],
            },
        },
    ]
}
