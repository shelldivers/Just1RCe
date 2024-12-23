(cpplint)[https://github.com/cpplint/cpplint]   

## INDEX
- [CPPLINT](#cpplint)
	- [INSTALL (for Cluster)](#install-for-cluster)
	- [INSTALL (for Personal Invironment)](#install-for-personal-invironment)
	- [HOW TO USE](#how-to-use)
		- [COMMAND](#command)
		- [VSCODE EXTENSION](#vscode-extension)

---


# CPPLINT


Cpplint is a command-line tool to check C/C++ files for style issues according to (Google's C++ style guide)[https://google.github.io/styleguide/cppguide.html].   

## INSTALL (for Cluster)   

- Install cpplint by python virtual environmet   

```bash
# 1. Install cpplint by python virtual environmet
cd ~/goinfre
python3 -m venv ./venv
. ./venv/bin/activate
pip3 install cpplint
deactivate
cd ~

# 1. Add cpplint PATH
echo 'alias cpplint="~/goinfre/venv/bin/cpplint"' >> ./.zshrc
```   

## INSTALL (for Personal Invironment)   

```bash
# 1. Install cpplint
pip install cpplint
cpplint --version
```
   
## HOW TO USE   

### COMMAND     

USE SCRIPT ./scripts/cpplint.sh

### VSCODE EXTENSION   

This [extension](https://marketplace.visualstudio.com/items?itemName=mine.cpplint) check coding style of cpp and c, when open and save source file .  









