{
    'includes':['cloudio/websocket/common.gypi'],
    'targets':[
        {
            'target_name' : 'json11',
            'type' : 'static_library',
            'sources': ['cloudio/json11/json11.cpp'],
            'include_dirs': ['cloudio'],
            'all_dependent_settings':{
                'include_dirs': ['cloudio'],
            }
        },

        {
            'target_name' : 'json11_test',
            'type': 'executable',
            'sources': ['cloudio/json11/test.cpp'],
            'dependencies': ['json11']
        },

        {
            'target_name' : 'cloudio',
            'type' : 'none',
            'dependencies': ['./cloudio/websocket/websocket.gyp:websocket', 'json11'],

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
