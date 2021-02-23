suite = {
    "mxversion": "6.11.3",
    "name": "truffleruby",

    "imports": {
        "suites": [
            {
                "name": "regex",
                "subdir": True,
                "version": "9377addc6deb4a3f7808c7eb32a5d3a409fa86e4",
                "urls": [
                    {"url": "https://github.com/oracle/graal.git", "kind": "git"},
                    {"url": "https://curio.ssw.jku.at/nexus/content/repositories/snapshots", "kind": "binary"},
                ]
            },
            {
                "name": "sulong",
                "subdir": True,
                "version": "9377addc6deb4a3f7808c7eb32a5d3a409fa86e4",
                "urls": [
                    {"url": "https://github.com/oracle/graal.git", "kind": "git"},
                    {"url": "https://curio.ssw.jku.at/nexus/content/repositories/snapshots", "kind": "binary"},
                ]
            },
        ],
    },

    "licenses": {
        "EPL-2.0": {
            "name": "Eclipse Public License 2.0",
            "url": "https://opensource.org/licenses/EPL-2.0",
        },
        "BSD-simplified": {
            "name": "Simplified BSD License (2-clause BSD license)",
            "url": "http://opensource.org/licenses/BSD-2-Clause"
        },
        "MIT": {
            "name": "MIT License",
            "url": "http://opensource.org/licenses/MIT"
        },
    },

    "repositories": {
        "truffleruby-binary-snapshots": {
            "url": "https://curio.ssw.jku.at/nexus/content/repositories/snapshots",
            "licenses": [
                "EPL-2.0",          # JRuby (we're choosing EPL out of EPL,GPL,LGPL)
                "BSD-simplified",   # MRI
                "BSD-new",          # Rubinius, FFI
                "MIT",              # JCodings, minitest, did_you_mean, rake
            ]
        },
    },

    "libraries": {

        # ------------- Libraries -------------

        "JONI": {
            "maven": {
                "groupId": "org.jruby.joni",
                "artifactId": "joni",
                "version": "2.1.41"
            },
            "digest": "sha512:88728bb3b6cf8b385e3a4639981ab8a54e977291cbb1d8b60fddd3a3381d9d5f356faa46b6b15b9ccf6ed4a2a42552152a254bfeab60f8b2ddac4959300212ac",
            "sourceDigest": "sha512:8502efcc9ab4d31aa92eb9c279f02b9cb1ab2d9c65660635a84d9ef6e773b05ebae026043ed7749a41dc0ee8b5f822d2e88ba44b3f6e244b145d61f2095e8d77",
            "license": ["MIT"],
        },

        "JCODINGS": {
            "maven": {
                "groupId": "org.jruby.jcodings",
                "artifactId": "jcodings",
                "version": "1.0.55"
            },
            "digest": "sha512:791c1442ca0aad9132f5d2d641f9bacf5b461e11fa2f8d2010af0de77ee2629a330c739004699bb987e331798e1eea06b5b6b15b7e7dd6d41f3322edce805d6b",
            "sourceDigest": "sha512:8935def3d670972d1d82a981fe3fbd999abffac07350c46419f1e542c4529f00c949e3eef442321fce23941b648cb09de157114ec751c0ef85c7ccfa6b719445",
            "license": ["MIT"],
        },

        "NETBEANS-LIB-PROFILER": {
            "moduleName": "org.netbeans.modules.org-netbeans-lib-profiler",
            "digest": "sha512:1de81a0340c0266b41ba774600346ac977910663016a0afa22859cf1eb9d9e507de4f66e3f51d5bd9575b1d1f083765ecb9b30c4d43adb201f68b83257e8a17d",
            "sourceDigest": "sha512:92c50b8832e3a9afc93f9eaacdfc79cdf2487a74a9f5cf93c54bed50e904ef70ac6504018558d7183f074132c37fe57b21bc1e662a71c74c4201b75cdc5f8947",
            "maven": {
              "groupId": "org.netbeans.modules",
              "artifactId": "org-netbeans-lib-profiler",
              "version": "RELEASE120-1",
            },
        },
    },

    "externalProjects": {
        "truffleruby-root": {
            "type": "ruby",
            "path": '.',
            "source": [
                "lib/json",
                "lib/mri",
                "lib/truffle",
            ],
            "load_path": ["src/main/ruby/truffleruby/core"],
            "test": ["spec", "test"],
            "excluded": [
                "dumps",
                "logo",
                "mxbuild",
                "profiles",
                ".ext",
                "truffleruby-gem-test-pack",
                "lib/json/java",
                "lib/ruby",
                "test/truffle/ecosystem/blog6",
                "test/truffle/ecosystem/hello-world",
                "test/truffle/ecosystem/rails-app",
                "test/truffle/offline",
                "tool/docker",
                "rubyspec_temp",
            ]
        },
    },

    "projects": {

        # ------------- Projects -------------

        "org.truffleruby.annotations": {
            "dir": "src/annotations",
            "sourceDirs": ["java"],
            "jacoco": "include",
            "javaCompliance": "17+",
            "checkstyle": "org.truffleruby",
            "workingSets": "TruffleRuby",
            "checkPackagePrefix": "false",
            "license": ["EPL-2.0"],
        },

        "org.truffleruby.shared": {
            "dir": "src/shared",
            "sourceDirs": ["java"],
            "requires": ["java.management"],
            "dependencies": [
                "truffleruby:TRUFFLERUBY-ANNOTATIONS",
                "sdk:GRAAL_SDK",
            ],
            "annotationProcessors": [
                "TRUFFLERUBY-PROCESSOR",
            ],
            "jacoco": "include",
            "javaCompliance": "17+",
            "checkstyle": "org.truffleruby",
            "workingSets": "TruffleRuby",
            "license": ["EPL-2.0"],
        },

        "org.truffleruby.processor": {
            "dir": "src/processor",
            "sourceDirs": ["java"],
            "requires": ["java.compiler"],
            "dependencies": [
                "truffleruby:TRUFFLERUBY-ANNOTATIONS",
                "truffle:TRUFFLE_API",
            ],
            "jacoco": "include",
            "javaCompliance": "17+",
            "checkstyle": "org.truffleruby",
            "workingSets": "TruffleRuby",
            "license": ["EPL-2.0"],
        },

        "org.truffleruby.services": {
            "dir": "src/services",
            "sourceDirs": ["java"],
            "requires": ["java.scripting"],
            "dependencies": [
                "sdk:GRAAL_SDK",
            ],
            "jacoco": "include",
            "javaCompliance": "17+",
            "checkstyle": "org.truffleruby",
            "workingSets": "TruffleRuby",
            "license": ["EPL-2.0"],
        },

        "org.truffleruby.rubysignal": {
            "dir": "src/main/c/rubysignal",
            "native": "shared_lib",
            "deliverable": "rubysignal",
            "buildDependencies": [
                "org.truffleruby", # for the generated JNI header file
            ],
            "use_jdk_headers": True, # the generated JNI header includes jni.h
            "cflags": ["-g", "-Wall", "-Werror", "-pthread"],
            "ldflags": ["-pthr