{
    'target_defaults': {
      'default_configuration': 'Debug',
      'configurations': {
        # TODO: hoist these out and put them somewhere common, because
        #       RuntimeLibrary MUST MATCH across the entire project
        'Debug': {
          'defines': [ 'DEBUG', '_DEBUG' ],
          'cflags': [ '-Wall', '-Wextra', '-O0', '-g', '-ftrapv' ],
          'xcode_settings':{
              'OTHER_CPLUSPLUSFLAGS':['-O0']
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeLibrary': 1, # static debug
            },
          },
        },
        'Release': {
          'defines': [ 'NDEBUG' ],
          'cflags': [ '-Wall', '-Wextra', '-O3' ],
          'xcode_settings':{
              'OTHER_CPLUSPLUSFLAGS':['-Os']
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeLibrary': 0, # static release
            },
          },
        }
      },
      'msvs_settings': {
        'VCCLCompilerTool': {
        },
        'VCLibrarianTool': {
        },
        'VCLinkerTool': {
          'GenerateDebugInformation': 'true',
        },
      },
      'conditions': [
        ['OS == "win"', {
          'defines': [
            'WIN32'
          ],
        }]
      ],
    },

    'targets':[
        {
            'target_name' : 'client',
            'type' : 'executable',
            'dependencies': ['../../cloudio.gyp:cloudio'],
            'sources' : ['client.cpp'],
            'product_dir':'./',

        },

        {
            'target_name' : 'server',
            'type' : 'executable',
            'dependencies': ['../../cloudio.gyp:cloudio'],
            'sources' : ['server.cpp'],
            'product_dir':'./',
        },
    ]
}
