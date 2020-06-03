### Project Structure
[Dependency Graph & Planning](https://drive.google.com/drive/folders/1grtk0Vnl6vUNuBGqPvI5pR31Esl_z2pb?usp=sharing)

    /arm11_04
    |
    |____/bin        (the final executable file)
    |
    |
    |____/doc        (project documentation)
    |
    |
    |____/programs   (Provided programs, DO NOT CONFUSE WITH /bin)
    |
    |
    |____/src        (every source file, .c and .h)
    |    |
    |    |__/emulate  (src files for part 1, including submodules and tests)
    |    |
    |    |__/assemble (src files for part 2, including submodules and tests)
    |    |
    |    |__/common   (common utilities used by both part 1 and 2)
    |
    |
    |____/obj        (where the generated .o files will be)
    |
    |____/lib        (any library dependences)
    |
    |____Makefile    (make instructions file)
    |
    |____README      (general readme of the project)
    |
    |____TODO        (todo list)