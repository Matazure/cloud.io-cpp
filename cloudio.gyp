{
    'includes':['cloudio/websocket/common.gypi'],
    'targets':[
        {
            'target_name' : 'cloudio',
            'type' : 'none',
            'dependencies': ['./cloudio/websocket/websocket.gyp:websocket'],

            'all_dependent_settings':
            {
                'include_dirs': [ '.'],
            },
        },
    ]
}
