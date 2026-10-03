# Roadmap

## 2.0.0
- [x] nob compliant rebuild system
- [ ] error handling
- [ ] memory arenas

### Possible additions to version
- [ ] get last file modification
- [ ] c99 compliant limits to PbsCmd (optionally)
- [ ] honest library??

## 3.0.0
- [ ] packages stuff
- [ ] pbs extended recipe function rebuild (a bit strange if only a function wrapper)
- [ ] recursive file walking
- [ ] build archive (static)
- [ ] build objects
- [ ] build shared library?

### Possible additions to version
- [ ] autogen documentation

### NOTES
Notes on rebuild system:
```
// A few ways to rebuild itself:
// nob default:
//  - only checking the current __FILE__ and compiles according to argv[0]
// nob extended:
//  - takes a few files as va args and checks them and compiles according to argv[0]
//  - OBS: ONLY compiles __FILE__, yourself have to #include the other .c sources in main
// pbs extended:
//  - takes a recipe function to compile build system
//  - api exposes functions to check files for last change
```
