{
  'targets': [
    {
      'target_name': 'jsbrex-native',
      'sources': [ 'src/jsbrex.cc' ],
      'include_dirs': [
          "<!@(node -p \"require('node-addon-api').include\")", 
          "<(module_root_dir)/include/json/"
      ],
      'libraries': [
          "<(module_root_dir)/include/brex/libbrex.a"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags_cc': [ '-O0 -g -ggdb -fno-omit-frame-pointer -std=gnu++20', '-fexceptions' ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ]
    }
  ]
}