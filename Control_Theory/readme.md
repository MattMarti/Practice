# Control Theory

Some Jupyter Notebooks that I used to explore concepts from my control theory classes.

## Setup

I used `pyenv` and `poetry` to manage dependencies. Install Python 3.10.11:

```bash
pyenv install 3.10.11
```

Provide poetry the path to the installation folder. `pyenv` will install python versions to the `.pyenv` folder in your home directory. On windows, the command will look something like this

```bash
poetry env use C:\Users\matth\.pyenv\pyenv-win\versions\3.10.11\python3.exe
```

You can then set up the virtual environment by running

```bash
poetry install
```
