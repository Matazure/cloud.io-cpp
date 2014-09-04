{
    'includes': ['../../cloudio/websocket/common.gypi'],

    'targets':[
        {
            'target_name' : 'client',
            'type' : 'executable',
            'product_extension' : 'out',
            'dependencies': ['../../cloudio.gyp:cloudio'],
            'sources' : ['client.cpp'],
            'product_dir':'./',

        },

        {
            'target_name' : 'server',
            'type' : 'executable',
            'product_extension' : 'out',
            'dependencies': ['../../cloudio.gyp:cloudio'],
            'sources' : ['server.cpp'],
            'product_dir':'./',
        },
    ]
}
