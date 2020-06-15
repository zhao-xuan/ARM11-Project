## Subtree

/doc is an subtree connected to our [overleaf repository](https://www.overleaf.com/4289975884mvfmsjfdbbyf), to update run:

    git remote add overleaf https://git.overleaf.com/5ecce2a9cc5e2e0001344720
    git subtree pull --prefix doc overleaf master --squash

/extension/tetris-plus-plus is an subtree connected to [this repository](https://gitlab.doc.ic.ac.uk/zd419/tetris-plus-plus):

    git remote add extension git@gitlab.doc.ic.ac.uk:zd419/tetris-plus-plus.git
    git fetch extension master
    git subtree pull --prefix extension/tetris-plus-plus extension master

## Folder Structure

[Dependency Graph & Planning](https://drive.google.com/drive/folders/1grtk0Vnl6vUNuBGqPvI5pR31Esl_z2pb?usp=sharing)

    /arm11_04
    |
    |____/bin        (the final executable file)
    |
    |
    |____/doc        (project documentation, subtree of overleaf repository)
    |
    |
    |____/extension  (our extension, subtree of tetris-plus-plus repository)
    |
    |
    |____/lib        (any library dependences)
    |
    |
    |____/obj        (where the generated .o files will be)
    |
    |
    |____/programs   (Provided programs, DO NOT CONFUSE WITH /bin)
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
    |____Makefile    (make instructions file)
    |
    |____README      (general readme of the project)
    |
    |____TODO        (todo list)
