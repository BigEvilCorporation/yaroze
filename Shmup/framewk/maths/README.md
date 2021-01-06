# Welcome to MMath!
MMath is a single-header math library made in C for use with OpenGL and other similar graphics APIs.

###### Quick refernces:
- [Features](#features)
- [On the to-do list](#on-the-to-do-list)
- [License](#license)
- [How to use MMath](#how-to-use-mmath)

###### Additional information:

MMath has been lightly tested with both MSVC and GCC, but problems may still exist. Please report any bugs found in the [issues section](https://github.com/8bitslime/MMath/issues) so they can be dealt with quickly.

Some documentation is provided in the [wiki](https://github.com/8bitslime/MMath/wiki), but more will be added in the future.

---

### Features
- Vectors
- Square matrices
- Quaternions
- Transformations
- Easy appending to:
	- vectors
    - matrices

---

### On the to-do list
- Rectangular matrices
- SIMD optimizations
- Various profilings
- Renaming the library

---

### License
MMath is released, free of charge, under the [MIT license](./LICENSE). This means that you are allowed to use MMath for both private and commercial projects as well as modify and redistribute it. You are not obligated to credit MMath in your projects, but it would be greatly appreciated.

---

### How to use MMath
To add MMath to your project, simply put the [`MMath.h`](./MMath.h) header file in your project's include directory and use `#include "MMath.h"` anywhere math is required.

If you require *double precision*, add the line `#define MMATH_DOUBLE` before including [`MMath.h`](./MMath.h).
