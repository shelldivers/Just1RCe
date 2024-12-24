## INDEX
- [CPPCHECK](#cppcheck)
	- [INSTALL   (for Cluster)](#install---for-cluster)
	- [INSTALL (for Personal Invironment)](#install-for-personal-invironment)
	- [HOW TO USE](#how-to-use)

---

# CPPCHECK

## INSTALL   (for Cluster)   
 

```bash
# 1. Install brew    
cd goinfre
mkdir brew
export HOMEBREW_PREFIX=~/goinfre/brew
export HOMEBREW_REPOSITORY=~/goinfre/brew/Homebrew

git clone https://github.com/Homebrew/brew.git ~/goinfre/brew/Homebrew
echo 'export PATH=/Users/$USER/goinfre/brew/homebrew/bin' >> ~/.bash_profile
source ~/.bash_profile
   
# 2. Install cppcheck   
brew install cppcheck   

# 3. Add brew PATH 
echo 'export PATH=/Users/$USER/goinfre/brew/homebrew/bin:$PATH' >> ~/.zshrc
```    

## INSTALL (for Personal Invironment)   

```bash
# 1. Install cppcheck 
brew install cppcheck

# 2. Add brew PATH
echo 'export PATH=/Users/$USER/goinfre/brew/homebrew/bin:$PATH' >> ~/.zshrc
```

## HOW TO USE

USE SCRIPT ./scripts/cppcheck.sh   
