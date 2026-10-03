# Ideas

## Ownership
Ownership is always a problem in C. Therefore a common set of guidelines:
- The library should own whatever it uses, if it gets a string it should dup that to own it.
- Easy deallocation.
- Swappable allocator
- Okay designed api to use less memory. 
