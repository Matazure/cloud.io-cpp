{
    'targets':[
        {
            'target_name' : 'cloud.io',
            'type' : 'none',
            'dependencies': ['./cloudio/websocket/websocket.gyp:websocket'],

            'all_dependent_settings':
            {
                'include_dirs': [ '.'],
            },
        },
    ]
}
