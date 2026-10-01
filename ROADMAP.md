# Roadmap

## 2.0.0
- [x] nob compliant rebuild system
- [ ] recursive file walking
- [ ] append arguments to command type
- [ ] get last file modification

## 3.0.0
error handling
full implemented api:
- get last modification time of file
- pbs extended recipe function rebuild (a bit strange if only a function wrapper)
- build archive (static)
- build objects
- build shared library?

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
