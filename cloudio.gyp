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

        {
            'target_name' : 'example_basic_client',
            'type' : 'executable',
            'product_extension' : 'out',
            'dependencies': ['cloudio'],
            'sources' : ['example/basic/client.cpp'],
            # 'product_dir':'./',

        },

        {
            'target_name' : 'example_basic_server',
            'type' : 'executable',
            'product_extension' : 'out',
            'dependencies': ['cloudio'],
            'sources' : ['example/basic/server.cpp'],
            # 'product_dir':'./',
        },
    ]
}
